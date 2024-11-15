#!/bin/bash

#./execute_test.sh $1 $2 $3 $4 $5
# $1: BOARD_NAME: brd4187c
# $2: task: all, gen-only, run-only
# $3: components: all, unit_test_iec60730_bist, unit_test_iec60730_post, ...
# $4: ADAPTER_SN
# $5: compiler: GCC, IAR
# $6: some option as "-DENABLE_CAL_CRC_32=ON -DENABLE_CRC_USE_SW=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON"

# Example
#  bash execute_unit_test.sh brd4187c all all 440111030 GCC

# Note:
# In case you want to build CRC32 run this command. For example
#  bash execute_unit_test.sh brd4187c all all 440111030 GCC "-DENABLE_CAL_CRC_32=ON"

BASH_DIRECTION=$(pwd)
BASH_PRE_IAR_BUILD=$(pwd)/../simplicity_sdk
BOARD_NAME=$1
TASK=$2
COMPONENT=$3
ADAPTER_SN=$4
COMPILER=$5
OPTION_INTEGRATION_TEST=${6//"%20"/" "}
JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
TEST_PATH=$(pwd)/test_script
LOG_PATH=$(pwd)/../log
LOG_FILE_TEMP=$(pwd)/../log/temp.log
LOG_BUILD=$LOG_PATH/build_integration_test_components.log
LOG_FILE=$LOG_PATH/build_integration_test_components.log
IMAGE_PATH=$(pwd)/../build/test/integration_test/build/$BOARD_NAME/$COMPILER
DEVICE_NAME=

if [ -d "$LOG_PATH" ];then
  mkdir -p $LOG_PATH
fi

#echo "OPTION_INTEGRATION_TEST: $OPTION_INTEGRATION_TEST"

if [[ "$OPTION_INTEGRATION_TEST" == *"-DTEST_SECURE_PERIPHERALS_ENABLE"* ]];then
  printf "\n===== Enable integration testing check secure peripherals =====\n"
else
  printf "\n===== Enable integration testing check non-secure peripherals =====\n"
fi

function get_device_name
{
    serialno=$1
    commander device info --serialno $serialno > temp.txt
    device_name=$(head -n 1 temp.txt)
    device_name=${device_name//"Part Number    : "/}
    #printf "$device_name\n"
    rm -rf temp.txt
    echo "${device_name}"
}

DEVICE_NAME=$(get_device_name $ADAPTER_SN)
#printf "$DEVICE_NAME\n"

#if  [[ "$DEVICE_NAME" == *"EFR32MG24B"* ]]; then
#    DEVICE_NAME=EFR32MG24BXXXF1536
#fi
#printf "===== $DEVICE_NAME\n"
cd $BASH_DIRECTION/..
if [ ! -e "log" ];then
    mkdir -p log
fi

function gen_image
{
    if [[ "$COMPILER" == "IAR" ]] ;then
        echo "-- [I] Start run pre_build_iar!"
        cd $BASH_PRE_IAR_BUILD
        bash pre_build_iar.sh $BOARD_NAME "-DENABLE_INTEGRATION_TESTING=ON $OPTION_INTEGRATION_TEST" &> /dev/null
        echo "-- [I] Run pre_build_iar done!"
    fi
    cd $BASH_DIRECTION/..
    make prepare &> /dev/null
    cd $BASH_DIRECTION/../build
    cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DBOARD_NAME=${BOARD_NAME} $OPTION_INTEGRATION_TEST
    sleep 1s
    if [[ "$1" = "all" ]]; then
        printf "Generate all components!\n"
        printf "Clean component\n"
        make integration_test_clean &> /dev/null
        components=$(make integration_test_info) &> /dev/null
        components=${components//"Components list: "/}
        components=${components//"Built target integration_test_info"/}
        list_components=${components}
    else
        printf "Generate only components: $1\n"
        printf "Clean component: $1\n"
        make integration_test_clean &> /dev/null
        list_components="$1"
    fi
    printf "Components list: ${list_components}\n"

    # Get compiler and Arguments
    compiler=$2

    resultBuild=("\n===== Summariez the build results of all component =====\n")
    sumResultBuild=""
    touch $LOG_BUILD
    for component in ${list_components}; do
        printf "\n===== Start build component: $component compiler: $compiler =====\\n"
        log=$(make $component -j4)
        echo "$log" | tee -a $LOG_BUILD
        result=$(tail -n 1 $LOG_BUILD)

        if [ "$result" = "[100%] Built target $component" ]; then
            printf "Build successfully component: $component\n"
            resultBuild+=("Success to build component: $component\n")
        else
            printf "ERROR!!! Build component: $component fail\n"
            resultBuild+=("Fail to build component: $component\n")
        fi
    done

    # Print summariez the build results
    for restBuild in "${resultBuild[@]}"
    do
        sumResultBuild="$sumResultBuild$restBuild\n"
    done
    printf "$sumResultBuild"
    rm -rf $LOG_BUILD
}

function flash_image
{
    component="$1"

    # Get compiler and Arguments
    compiler=$2
    string=$3

    #echo "string: $string"

    if [[ "$string" == *"ENABLE_CAL_CRC_32=ON"* ]]; then
        crc="_crc32"
    else
        crc="_crc16"
    fi

    printf "= Flash components: $component - Flash with compiler: $compiler\n"

    # Export PATH to commander directory
    COMMANDER="commander"
    if [ "$component" == "integration_test_iec60730_invariable_memory" ]; then
        file_out=$IMAGE_PATH/$component$crc.s37
    else
        file_out=$IMAGE_PATH/$component.s37
    fi
    printf "= Erase chip\n"
    result=$($COMMANDER device masserase --serialno $ADAPTER_SN)
    #printf "Result: $result"
    printf "= Flash file: $file_out\n"
    result=$($COMMANDER flash --serialno $ADAPTER_SN $file_out)
    if [ "" = "$result" ]; then
        return 1
    else
        return 0
    fi
}

function run
{
    cd $BASH_DIRECTION/../build

    if [[ "$1" = "all" ]]; then
        printf "Run integration test all components!\n"
        components=$(make integration_test_info) &> /dev/null
        components=${components//"Components list: "/}
        components=${components//"Built target integration_test_info"/}
        list_components=${components}
    else
        printf "Run integration test only components: $1\n"
        list_components="$1"
    fi
    printf "Components list: ${list_components}\n"

    # Get compiler and Arguments
    compiler=$2
    arg=$3
    printf "\n===== Argument: $arg\n"
    resultBuild=("\n===== Summariez the test results of all component =====\n")
    sumResultBuild=""
    printf "= Run with compiler: $compiler\n"

    for component in ${list_components}; do
        if [[ -f $LOG_FILE_TEMP ]]; then
            rm -rf $LOG_FILE_TEMP
        fi

        if [[ "$OPTION_INTEGRATION_TEST" == *"TEST_SECURE_PERIPHERALS_ENABLE=ON"* ]];then
          IMAGE_PATH=$(pwd)/../build/test/integration_test/build/$BOARD_NAME/$compiler/$component/S
        else
          IMAGE_PATH=$(pwd)/../build/test/integration_test/build/$BOARD_NAME/$compiler/$component/NS
        fi
        #echo "OPTION_INTEGRATION_TEST: $OPTION_INTEGRATION_TEST"
        flash_image $component $compiler "$arg"
        local flashResult=$?
        LST_PATH=$IMAGE_PATH/
        #printf "LST_PATH: $LST_PATH\n"
        LOG_FILE=$LOG_PATH/$component.log
        if [[ -f $LOG_FILE ]]; then
            rm -rf $LOG_FILE
        fi

        if [ "0" = "$flashResult" ];then
            echo "Flash result $component successful!"
            $COMMANDER device reset --serialno $ADAPTER_SN
            if [ -f "$TEST_PATH/$component.py" ] && [ -d "$LST_PATH" ];then
                #echo $(pwd)
                local successCount=0
                local totalTestcase=0
                TEST_SCRIPT=$TEST_PATH/$component.py

                if [[ "$component" == "integration_test_iec60730_watchdog" ]] && [[ "$arg" == *"INTEGRATION_TEST_WDOG1_ENABLE=ON"* ]];then
                  printf "\n= Start run integration test: $component with INTEGRATION_TEST_WDOG1_ENABLE=enable\n"
                  log=$(CHIP=$DEVICE_NAME ADAPTER_SN=$ADAPTER_SN LST_PATH=$LST_PATH JLINK_PATH=$JLINK_PATH INTEGRATION_TEST_WDOG1_ENABLE=enable python3 $TEST_SCRIPT $compiler $arg)
                elif [[ "$component" == "integration_test_iec60730_variable_memory" ]] && [[ "$arg" == *"INTEGRATION_TEST_USE_MARCHX_DISABLE=ON"* ]]; then
                  printf "\n= Start run integration test: $component with INTEGRATION_TEST_USE_MARCHX_DISABLE=disable\n"
                  log=$(CHIP=$DEVICE_NAME ADAPTER_SN=$ADAPTER_SN LST_PATH=$LST_PATH JLINK_PATH=$JLINK_PATH INTEGRATION_TEST_USE_MARCHX_DISABLE=disable python3 $TEST_SCRIPT $compiler $arg)
                elif [[ "$component" == "integration_test_iec60730_invariable_memory" ]] && [[ "$arg" == *"ENABLE_CAL_CRC_32=ON"* ]]; then
                  printf "\n= Start run integration test: $component with INTEGRATION_TEST_ENABLE_CAL_CRC_32=enable\n"
                  log=$(CHIP=$DEVICE_NAME ADAPTER_SN=$ADAPTER_SN LST_PATH=$LST_PATH JLINK_PATH=$JLINK_PATH INTEGRATION_TEST_ENABLE_CAL_CRC_32=enable python3 $TEST_SCRIPT $compiler $arg)
                else
                  printf "\n= Start run integration test: $component\n"
                  log=$(CHIP=$DEVICE_NAME ADAPTER_SN=$ADAPTER_SN LST_PATH=$LST_PATH JLINK_PATH=$JLINK_PATH python3 $TEST_SCRIPT $compiler $arg)
                fi
                #echo "============================"
                #echo $log
                #echo "============================"
                $(mv $LOG_FILE_TEMP $LOG_FILE)
                # Save output to log file
                for line in echo $log; do
                  if [ "True" == "$line" ]; then
                    let successCount+=1
                  fi
                  if [ "TC:" == "$line" ]; then
                    let totalTestcase+=1
                  fi
                done

                if [[ "$successCount" == "$totalTestcase" ]] && [ 0 != $totalTestcase ]; then
                  echo "= Finish run integration test: $component - Success $successCount/$totalTestcase - PASS" >> $LOG_FILE
                  resultBuild+=("\n= Finish run integration test: $component - Success $successCount/$totalTestcase - PASS\n")
                else
                  echo "= Finish run integration test: $component - Success $successCount/$totalTestcase - FAIL" $LOG_FILE
                  resultBuild+=("\n= Finish run integration test: $component - Success $successCount/$totalTestcase - FAIL\n")
                fi

            else
                if [ ! -f "$TEST_PATH/$component.py" ]; then
                  printf "File $TEST_PATH/$component.py is not found in path $TEST_PATH!\n"
                  resultBuild+=("File $TEST_PATH/$component.py is not found in path $TEST_PATH!\n")
                elif [ ! -d "$LST_PATH" ]; then
                  printf "File ${component}.lst is not found in path $LST_PATH!\n"
                  resultBuild+=("File ${component}.lst is not found in path $LST_PATH!")
                fi
            fi
        else
            echo "Flash result ${component} failed!"
            resultBuild+=("Flash Result ${component} failed!")
        fi
    done

    # Print summariez the build results
    for restBuild in "${resultBuild[@]}"
    do
        sumResultBuild="$sumResultBuild$restBuild\n"
    done
    printf "$sumResultBuild"
}

case $TASK in
    gen-only)
        gen_image "$COMPONENT" "$COMPILER"
        ;;
    run-only)
        run "$COMPONENT" "$COMPILER" "$OPTION_INTEGRATION_TEST"
        ;;
    all)
        gen_image "$COMPONENT" "$COMPILER"
        run "$COMPONENT" "$COMPILER" "$OPTION_INTEGRATION_TEST"
        ;;
    *)
        echo "Please choose one of those options:  gen-only ; run-only; all"
esac
