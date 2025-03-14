#!/bin/bash

# This script runs unit tests for a specified board and compiler.
# Usage: ./run_unit_test.sh <BOARD_NAME> <ADAPTER_SN> <COMPILER>
# Example: bash run_unit_test.sh brd4187c 440111030 GCC

BOARD_NAME=$1
ADAPTER_SN=$2
COMPILER=$3
JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
LOG_PATH=$(pwd)/../log
IMAGE_PATH=$(pwd)/../build/test/unit_test/build/$BOARD_NAME/$COMPILER
TEST_SCRIPT=$(pwd)/test_script/unit_test_iec60730_get_report.py
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

# Function to get the number of test cases from the log file
function get_number_test_cases {
    local log_file=$1
    local number_test_cases
    tail -n 2 $log_file > $LOG_PATH/temp.txt
    result_test_cases=$(head -n 1 $LOG_PATH/temp.txt)
    number_test_cases=${result_test_cases:24:2}
    echo $number_test_cases
}

# Function to get the number of failed test cases from the log file
function get_number_failure_test_cases {
    local log_file=$1
    local number_failure_test_cases
    test_string="Tests "
    failure_string=" Failures"
    tail -n 2 $log_file > $LOG_PATH/temp.txt
    result_test_cases=$(head -n 1 $LOG_PATH/temp.txt)
    position_failure_value=${result_test_cases#*$test_string}
    number_failure_test_cases=${position_failure_value%"$failure_string"}
    echo $number_failure_test_cases
}

# Function to run the image and execute the unit test
function run_image {
    file_out=$1
    component=$(basename $file_out .s37)

    if [[ "$component" == "unit_test_iec60730_invariable_memory" ]]; then
        file_out="${file_out%/*}/unit_test_iec60730_invariable_memory_crc16.s37"
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
        if [ -f "$TEST_SCRIPT" ] && [ -d "$lst_path" ]; then
            printf "\n= Start run unit test: $component\n"
            echo $(pwd)
            log=$(CHIP=$DEVICE_NAME FILE_NAME=$component ADAPTER_SN=$ADAPTER_SN LST_PATH=$lst_path JLINK_PATH=$JLINK_PATH python3 $TEST_SCRIPT $COMPILER)
            if [ -f $LOG_PATH/${component}.log ]; then
                echo "File ${component}.log is found in path $LOG_PATH\n!"
                number_test_cases=$(get_number_test_cases $LOG_PATH/${component}.log)
                number_failure_test_cases=$(get_number_failure_test_cases $LOG_PATH/${component}.log)
                if [[ "$number_failure_test_cases" == 0 ]]; then
                    printf "=> Finish run unit test $component - Success $number_test_cases/$number_test_cases - PASS\n"
                    result_run+=("Finish run unit test $component - Success $number_test_cases/$number_test_cases - PASS\n")
                else
                    printf "= Finish run unit test $component - Failures $number_failure_test_cases/$number_test_cases - FAIL(Failed round)\n"
                    result_run+=("=Finish run unit test $component - Failures $number_failure_test_cases/$number_test_cases - FAIL(Failed round)\n")
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
