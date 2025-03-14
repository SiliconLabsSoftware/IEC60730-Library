#!/bin/bash

# This script runs integration tests for a specified board and compiler.
# Usage: ./run_integration_test.sh <BOARD_NAME> <ADAPTER_SN> <COMPILER> <HOST_IP> <OPTION_INTEGRATION_TEST>
# Example: bash run_integration_test.sh brd4187c 440111030 GCC 192.168.1.100 "INTEGRATION_TEST_WDOG1_ENABLE=ON"

BOARD_NAME=$1
ADAPTER_SN=$2
COMPILER=$3
HOST_IP=$4
OPTION_INTEGRATION_TEST=${5//"%20"/" "}
JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
LOG_PATH=$(pwd)/../log
IMAGE_PATH=$(pwd)/../build/test/integration_test/build/$BOARD_NAME/$COMPILER
TEST_SCRIPT_PATH=$(pwd)/test_script
DEVICE_NAME=

# Function to get the device name using the adapter serial number
function get_device_name {
    serial_no=$1
    commander device info --serialno $serial_no > temp.txt
    device_name=$(head -n 1 temp.txt)
    device_name=${device_name//"Part Number    : "/}
    rm -rf temp.txt
    echo "${device_name}"
}

DEVICE_NAME=$(get_device_name $ADAPTER_SN)

# Set FLASH_REGIONS_TEST for brd4187c or MG24
if [[ "$BOARD_NAME" == "brd4187c" || "$BOARD_NAME" == "MG24" ]]; then
    export FLASH_REGIONS_TEST=0x8000000
fi

export HOST_IP=$HOST_IP

# Create log directory if it doesn't exist
if [ ! -e "$LOG_PATH" ]; then
    mkdir -p "$LOG_PATH"
fi

# Ensure required Python packages are installed
pip install --user -r $(pwd)/requirements.txt

# Function to flash the image to the device
function flash_image {
    file_out=$1

    printf "\n==========\nFlash file: $file_out\n"

    COMMANDER="commander"

    printf "Erase chip\n"
    result=$($COMMANDER device masserase --serialno $ADAPTER_SN)
    printf "Flash file: $file_out\n"
    result=$($COMMANDER flash --serialno $ADAPTER_SN $file_out)
    if [ "" = "$result" ]; then
        return 1
    else
        return 0
    fi
}

# Function to run the image and execute the integration test
function run_image {
    file_out=$1
    component=$(basename $file_out .s37)
    
    if [[ "$component" == "integration_test_iec60730_invariable_memory" ]]; then
        file_out="${file_out%/*}/integration_test_iec60730_invariable_memory_crc16.s37"
    fi
    
    if [[ -f $LOG_PATH/${component}.log ]]; then
        rm -rf $LOG_PATH/${component}.log
    fi

    flash_image $file_out
    local flash_result=$?
    local lst_path=$(dirname $file_out)

    if [ "0" = "$flash_result" ]; then
        echo "Flash result $component successful!"
        $COMMANDER device reset --serialno $ADAPTER_SN
        if [ -f "$TEST_SCRIPT_PATH/$component.py" ] && [ -d "$lst_path" ]; then
            printf "\n= Start run integration test: $component\n"
            echo $(pwd)
            if [[ "$component" == "integration_test_iec60730_watchdog" ]] && [[ "$OPTION_INTEGRATION_TEST" == *"INTEGRATION_TEST_WDOG1_ENABLE=ON"* ]]; then                
                log=$(CHIP=$DEVICE_NAME FILE_NAME=$component ADAPTER_SN=$ADAPTER_SN LST_PATH=$lst_path JLINK_PATH=$JLINK_PATH INTEGRATION_TEST_WDOG1_ENABLE=enable python3 $TEST_SCRIPT_PATH/$component.py $COMPILER)
            elif [[ "$component" == "integration_test_iec60730_variable_memory" ]] && [[ "$OPTION_INTEGRATION_TEST" == *"INTEGRATION_TEST_USE_MARCHX_DISABLE=ON"* ]]; then
                log=$(CHIP=$DEVICE_NAME FILE_NAME=$component ADAPTER_SN=$ADAPTER_SN LST_PATH=$lst_path JLINK_PATH=$JLINK_PATH INTEGRATION_TEST_USE_MARCHX_DISABLE=disable python3 $TEST_SCRIPT_PATH/$component.py $COMPILER)
            elif [[ "$component" == "integration_test_iec60730_invariable_memory" ]] && [[ "$OPTION_INTEGRATION_TEST" == *"ENABLE_CAL_CRC_32=ON"* ]]; then
                log=$(CHIP=$DEVICE_NAME FILE_NAME=$component ADAPTER_SN=$ADAPTER_SN LST_PATH=$lst_path JLINK_PATH=$JLINK_PATH INTEGRATION_TEST_ENABLE_CAL_CRC_32=enable python3 $TEST_SCRIPT_PATH/$component.py $COMPILER)
            else
                log=$(CHIP=$DEVICE_NAME FILE_NAME=$component ADAPTER_SN=$ADAPTER_SN LST_PATH=$lst_path JLINK_PATH=$JLINK_PATH python3 $TEST_SCRIPT_PATH/$component.py $COMPILER)
            fi
            mv $LOG_PATH/temp.log $LOG_PATH/${component}.log
            if [ -f $LOG_PATH/${component}.log ]; then
                echo "File ${component}.log is found in path $LOG_PATH\n!"
                local success_count=0
                local total_testcase=0
                while IFS= read -r line; do
                    if [[ "$line" == *"True"* ]]; then
                        ((success_count++))
                    elif [[ "$line" == *"TC:"* ]]; then
                        ((total_testcase++))
                    fi
                done < $LOG_PATH/${component}.log
                if [[ "$success_count" == "$total_testcase" ]] && [ 0 != $total_testcase ]; then
                    printf "= Finish run integration test $component - Success $success_count/$total_testcase - PASS\n"
                    result_run+=("= Finish run integration test $component - Success $success_count/$total_testcase - PASS\n")
                else
                    printf "= Finish run integration test $component - Failures $((total_testcase - success_count))/$total_testcase - FAIL(Failed round)\n"
                    result_run+=("= Finish run integration test $component - Failures $((total_testcase - success_count))/$total_testcase - FAIL(Failed round)\n")
                fi
            else
                echo "File ${component}.log is not found in path $LOG_PATH!"
                result_run+=("File ${component}.log is not found in path $LOG_PATH!(Failed round)\n")
            fi
        else
            if [ ! -f "$TEST_SCRIPT" ]; then
                printf "File $TEST_SCRIPT is not found!\n"
                result_run+=("File $TEST_SCRIPT is not found!(Failed round)\n")
            elif [ ! -d "$lst_path" ]; then
                printf "File ${component}.lst is not found in path $lst_path!\n"
                result_run+=("File ${component}.lst is not found in path $lst_path!(Failed round)\n")
            fi
        fi
    else
        echo "Flash result ${component} failed!(Failed round)"
        result_run+=("Flash result ${component} failed!(Failed round)\n")
    fi
}

# Main script logic
result_run=("\n======== Summarize the test results of all components ========\n")
for file in $(find $IMAGE_PATH -name "*.s37"); do
    if [[ $file == *_crc16.s37 ]]; then
        continue
    fi
    run_image $file
done

# Print summary of the run results
sum_result_run=""
for res_run in "${result_run[@]}"; do
    sum_result_run="$sum_result_run$res_run\n"
done
printf "$sum_result_run"
