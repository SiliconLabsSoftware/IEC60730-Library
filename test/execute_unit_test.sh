#!/bin/bash

#./execute_test.sh $1 $2 $3 $4 $5
# $1: BOARD_NAME: brd4187c
# $2: task: all, gen-only, run-only
# $3: components: all, unit_test_iec60730_bist, unit_test_iec60730_post, ...
# $4: ADAPTER_SN
# $5: compiler: GCC, IAR
# $6: "-DENABLE_CAL_CRC_32=ON -DENABLE_CRC_USE_SW=ON"

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
OPTION_UNIT_TEST=${6//"%20"/" "}
JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
TEST_PATH=$(pwd)/test_script
TEST_SCRIPT=$TEST_PATH/unit_test_iec60730_get_report.py
LOG_PATH=$(pwd)/../log
LOG_FILE=$LOG_PATH/build_unit_test_components.log
IMAGE_PATH=$(pwd)/../build/test/unit_test/build/$BOARD_NAME/$COMPILER
DEVICE_NAME=

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
        bash pre_build_iar.sh $BOARD_NAME "-DENABLE_UNIT_TESTING=ON $OPTION_UNIT_TEST" &> /dev/null
        echo "-- [I] Run pre_build_iar done!"
    fi
    cd $BASH_DIRECTION/..
    make prepare &> /dev/null
    cd $BASH_DIRECTION/../build
    cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=${BOARD_NAME} $OPTION_UNIT_TEST
    sleep 1s
    if [[ "$1" = "all" ]]; then
        printf "Generate all components!\n"
        printf "Clean component\n"
        make unit_test_clean &> /dev/null
        components=$(make unit_test_info) &> /dev/null
        components=${components//"Components list: "/}
        components=${components//"Built target unit_test_info"/}
        list_components=${components}
    else
        printf "Generate only components: $1\n"
        printf "Clean component: $1\n"
        make unit_test_clean &> /dev/null
        list_components="$1"
    fi
    printf "Components list: ${list_components}\n"

    # Get compiler and Arguments
    compiler=$2

    resultBuild=("\n======== Summariez the build results of all component ========\n")
    sumResultBuild=""
    touch $LOG_FILE
    for component in ${list_components}; do
        printf "\n======== Start build component: $component compiler: $compiler ========\\n"
        log=$(make $component -j4)
        echo "$log" | tee -a $LOG_FILE
        result=$(tail -n 1 $LOG_FILE)

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
    rm -rf $LOG_FILE
}

function flash_image
{
    component="$1"

    # Get compiler and Arguments
    compiler=$2
    string=$3

    if [[ "$string" == *"ENABLE_CAL_CRC_32=ON"* ]]; then
        crc="_crc32"
    else
        crc="_crc16"
    fi

    printf "Flash components: $component - Flash with compiler: $compiler\n"

    # Export PATH to commander directory
    COMMANDER="commander"
    if [ "$component" == "unit_test_iec60730_invariable_memory" ]; then
        file_out=$IMAGE_PATH/$component/$component$crc.s37
    else
        file_out=$IMAGE_PATH/$component/$component.s37
    fi
    printf "Erase chip\n"
    result=$($COMMANDER device masserase --serialno $ADAPTER_SN)
    #printf "Result: $result"
    printf "Flash file: $file_out\n"
    result=$($COMMANDER flash --serialno $ADAPTER_SN $file_out)
    if [ "" = "$result" ]; then
        return 1
    else
        return 0
    fi
}

function number_test_cases
{
    local numberTestCases
    log_file=$1
    tail -n 2 $log_file > $LOG_PATH/temp.txt
    result_test_cases=$(head -n 1 $LOG_PATH/temp.txt)
    numberTestCases=${result_test_cases:24:2}
    return $numberTestCases
}

function number_failure_test_cases
{
    local numberFailureTestCases
    testString="Tests "
    failureString=" Failures"
    log_file=$1
    tail -n 2 $log_file > $LOG_PATH/temp.txt
    result_test_cases=$(head -n 1 $LOG_PATH/temp.txt)
    positionFailureValue=${result_test_cases#*$testString}
    #printf "positionFailureValue:$positionFailureValue\n"
    numberFailureTestCases=${positionFailureValue%"$failureString"}
    #printf "numberFailureTestCases:$numberFailureTestCases\n"
    return $numberFailureTestCases
}

function run
{
    cd $BASH_DIRECTION/../build
    local numberTestCases
    local numberFailureTestCases

    if [[ "$1" = "all" ]]; then
        printf "Run unit test all components!\n"
        components=$(make unit_test_info) &> /dev/null
        components=${components//"Components list: "/}
        components=${components//"Built target unit_test_info"/}
        list_components=${components}
    else
        printf "Run unit test only components: $1\n"
        list_components="$1"
    fi
    printf "Components list: ${list_components}\n"

    # Get compiler and Arguments
    compiler=$2
    arg=$3
    printf "\n======= Argument: $arg\n"
    resultBuild=("\n======== Summariez the test results of all component ========\n")
    sumResultBuild=""
    printf "Run with compiler: $compiler\n"

    for component in ${list_components}; do
        if [[ -f $LOG_PATH/${component}.log ]]; then
            rm -rf $LOG_PATH/${component}.log
        fi
        flash_image $component $compiler "$arg"
        local flashResult=$?
        local LST_PATH=$IMAGE_PATH/$component/
        # echo "$LST_PATH"
        if [ "0" = "$flashResult" ];then
            echo "Flash result $component successful!"
            $COMMANDER device reset --serialno $ADAPTER_SN
            if [ -f "$TEST_SCRIPT" ] && [ -d "$LST_PATH" ];then
                printf "\n= Start run unit test: $component\n"
                echo $(pwd)
                log=$(CHIP=$DEVICE_NAME FILE_NAME=$component ADAPTER_SN=$ADAPTER_SN LST_PATH=$LST_PATH JLINK_PATH=$JLINK_PATH python3 $TEST_SCRIPT $compiler)
                if [ -f $LOG_PATH/${component}.log ]; then
                    echo "File ${component}.log is found in path $LOG_PATH\n!"
                    number_test_cases $LOG_PATH/${component}.log
                    numberTestCases=$?
                    number_failure_test_cases $LOG_PATH/${component}.log
                    numberFailureTestCases=$?
                    if [[ "$numberFailureTestCases" == 0 ]]; then
                        printf "\n= Finish run unit test $component - Success $numberTestCases/$numberTestCases - PASS\n"
                        resultBuild+=("Finish run unit test $component - Success $numberTestCases/$numberTestCases - PASS\n")
                    else
                        printf "\n= Finish run unit test $component - Failures $numberFailureTestCases/$numberTestCases - FAIL\n"
                        resultBuild+=("Finish run unit test $component - Failures $numberFailureTestCases/$numberTestCases - FAIL\n")
                    fi
                else
                    echo "File ${component}.log is not found in path $LOG_PATH!"
                    resultBuild+=("File ${component}.log is not found in path $LOG_PATH!")
                fi
            else
                if [ ! -f "$TEST_SCRIPT" ]; then
                  printf "File $$TEST_SCRIPT is not found in path $TEST_PATH!\n"
                  resultBuild+=(File $$TEST_SCRIPT is not found in path $TEST_PATH!\n)
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
    rm -rf $LOG_PATH/temp.*
}

case $TASK in
    gen-only)
        gen_image "$COMPONENT" "$COMPILER"
        ;;
    run-only)
        run "$COMPONENT" "$COMPILER" "$OPTION_UNIT_TEST"
        ;;
    all)
        gen_image "$COMPONENT" "$COMPILER"
        run "$COMPONENT" "$COMPILER" "$OPTION_UNIT_TEST"
        ;;
    *)
        echo "Please choose one of those options:  gen-only ; run-only; all"
esac
