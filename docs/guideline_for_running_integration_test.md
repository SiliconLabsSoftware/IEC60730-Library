# Guideline for running integration test

## Export Variable

> [!NOTE]
> Before running CMake, you need to export some variables first.

Export SDK_PATH=<path_to_sdk>, TOOL_DIRS=<path_to_toolchain>, TOOL_CHAINS, FLASH_REGIONS_TEST (flash start address support calculate CRC for module invariable memory), JLINK_PATH and path to `slc-cli` tool a before run config CMake.

If you want to calculate from the start address to the end address of Flash:

```sh
$ export SDK_PATH=/home/svc_sqa_automation/.silabs/slt/installs/conan/p/simpl508ee6c1a6569/p
$ export TOOL_DIRS=/home/svc_sqa_automation/.local/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
$ export TOOL_CHAINS=GCC
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export PATH=$PATH:/home/svc_sqa_automation/.silabs/slt/installs/archive/slc-cli-v6.0.23/slc_cli
$ export FLASH_REGIONS_TEST=0x8000000
```

Or if you want to calculate multiple regions:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

with FLASH_REGIONS_TEST=0x8000000 is the flash start address of board name brd4264c (chip EFR32FG23) or board name brd4187C (chip EFR32MG24)

To run integration tests manually, for the watchdog module, you need to connect the device to ethernet. Export CHIP, ADAPTER_SN, LST_PATH, JLINK_PATH, and the device's IP address and run the test script, for example:

```sh
$ export CHIP=EFR32FG23B020F512IM48 ADAPTER_SN=440111030
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export HOST_IP=192.168.1.69
```

If test secure peripherals or non-secure peripherals:

```sh
$ export LST_PATH=/home/svc_sqa_automation/SQA/Hieu/IEC60730-Library/build/test/integration_test/build/brd4264c/GCC/integration_test_iec60730_watchdog/NS

```
```sh
$ export LST_PATH=/home/svc_sqa_automation/SQA/Hieu/IEC60730-Library/build/test/integration_test/build/brd4264c/GCC/integration_test_iec60730_watchdog/S
```

> [!NOTE]
> Environment variables need to be exported during test execution:
>> export TOOL_CHAINS= GCC
>>
>> export TOOL_DIRS= <path_to_tool_chains>
>>
>> export FLASH_REGIONS_TEST= <start_address_flash_board>
>>
>> export HOST_IP= <ip_board>
>>
>> export JLINK_PATH= <path_to_jlink>

## Manually run integration tests

CMake config

```sh
$ make prepare
$ cd build
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DBOARD_NAME=brd4264c
```

CMake Build

```sh
$ cmake --build . --target integration_test_iec60730_program_counter -j4
$ cmake --build . --target integration_test_iec60730_irq -j4
$ cmake --build . --target integration_test_iec60730_system_clock -j4
$ cmake --build . --target integration_test_iec60730_watchdog -j4
$ cmake --build . --target integration_test_iec60730_cpu_registers -j4
$ cmake --build . --target integration_test_iec60730_variable_memory -j4
$ cmake --build . --target integration_test_iec60730_invariable_memory -j4
```

To support running integration tests for the watchdog module, there are 2 options when running the CMake config:

- TEST_SECURE_PERIPHERALS_ENABLE: enable test secure peripherals

- INTEGRATION_TEST_WDOG1_ENABLE: enable watchdog 1 to test if the device supports

To support running integration tests for the variable memory module, there is 1 option when running CMake config:

- INTEGRATION_TEST_USE_MARCHX_DISABLE: disable using MarchX algorithm

By default, the Variable Memory Check module uses the MarchX algorithm.
To disable MarchX, enable the following Cmake option. For example:

```sh
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=brd4264c
```

For devices that have a Trust zone implemented, secure and non-secure peripherals need to be tested.
Default enable checks non-secure peripherals. To check secure peripherals enable this option when running CMake config: TEST_SECURE_PERIPHERALS_ENABLE. For example:

```sh
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON -DBOARD_NAME=brd4264c
```

For devices that support 2 watchdogs, if you want to test both watchdogs, enable option INTEGRATION_TEST_WDOG1_ENABLE to ON when running Cmake config:

```sh
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DBOARD_NAME=brd4264c
```

To run integration tests for the watchdog module you need to connect the device to ethernet. Export CHIP, ADAPTER_SN, LST_PATH, JLINK_PATH, and the device's IP address and run the test script, for example:

```sh
$ export CHIP=EFR32FG23B020F512IM48
$ export ADAPTER_SN=440111030
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export HOST_IP=192.168.1.69
```

If test secure peripherals or non-secure peripherals:

```sh
$ export LST_PATH=~/devs_safety_lib/build/test/integration_test/build/brd4264c/integration_test_iec60730_watchdog/S

or

$ export LST_PATH=~/devs_safety_lib/build/test/integration_test/build/brd4264c/integration_test_iec60730_watchdog/NS
```

By default, the device enables watchdog 0 and test watchdog 0. If you want to test Watchdog 1 use this command:

```sh
$ INTEGRATION_TEST_WDOG1_ENABLE=enable python3 integration_test_iec60730_watchdog.py GCC
```

If you want to test the variable memory module with disable using the MarchX algorithm:

```sh
$ INTEGRATION_TEST_USE_MARCHX_DISABLE=disable python3 integration_test_iec60730_variable_memory.py GCC
```

If you want to test the invariable memory module with calculate crc32:

```sh
$ INTEGRATION_TEST_ENABLE_CAL_CRC_32=enable python3 integration_test_iec60730_variable_memory.py GCC
```

## Automatically run integration tests

Command run

```sh
bash execute_test.sh $1 $2 $3 $4 $5 $6
```

With the input arguments, there is the following information.

- $1: BOARD_NAME: brd4264c or EFR32FG23B020F512IM48
- $2: task: all, gen-only, run-only
- $3: components: all, unit_test_iec60730_bist, unit_test_iec60730_post, ...
- $4: ADAPTER_SN
- $5: compiler: GCC
- $6: OPTION_SUPPORT_INTEGRATION_TEST: "-DENABLE_CAL_CRC_32=ON -DENABLE_CRC_USE_SW=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON - INTEGRATION_TEST_USE_MARCHX_DISABLE=ON"

Which, components list that support testing includes:

- integration_test_iec60730_program_counter

- integration_test_iec60730_irq

- integration_test_iec60730_system_clock

- integration_test_iec60730_watchdog

- integration_test_iec60730_cpu_registers

- integration_test_iec60730_variable_memory

- integration_test_iec60730_invariable_memory

Before running the bash file, you need to install Jlink, Srecord, and slc tool, refer [Overview](./index.md) to set up some environment variables as follows:
If the compiler is GCC
- If the compiler is GCC:

If you want to calculate from the start address to the end address of Flash:

```sh
$ export SDK_PATH=/home/svc_sqa_automation/.silabs/slt/installs/conan/p/simpl508ee6c1a6569/p
$ export TOOL_DIRS=/home/svc_sqa_automation/.local/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
$ export TOOL_CHAINS=GCC
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export PATH=$PATH:/home/svc_sqa_automation/.silabs/slt/installs/archive/slc-cli-v6.0.23/slc_cli
$ export FLASH_REGIONS_TEST=0x8000000
$ export HOST_IP=192.168.1.69
$ export CHIP=EFR32FG23B020F512IM48
$ export ADAPTER_SN=440111030
```

Or if you want to calculate multiple regions:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

> [!NOTE]
> In the current integration test file, only enable computation of one region: from the start address of ​​the flash to the end of the flash. Therefore, export the flash's starting address. For example, chip EFR32MG24, ERF32FG23:
>> $ export FLASH_REGIONS_TEST=0x8000000

### Example

- With GCC toolchain:

```sh
$ bash execute_integration_test.sh brd4264c all all 440111030 GCC
```
### Note:
In case you want to build CRC32 run this command. For example:

```sh
$ bash execute_integration_test.sh brd4264c all all 440111030 GCC "-DENABLE_CAL_CRC_32=ON"
```

Or case you want to use the above integration test support options, run this command. For example:

```sh
$ bash execute_integration_test.sh brd4264c all all 440111030 GCC "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CAL_CRC_32=ON"
```

## CRC calculation options

When running build CMake to run unit tests and integration tests for invariable memory modules, the CRC calculation image file will have the suffix _crc16 or _crc32, you must flash the image file with this suffix.

With the commands above, the default value supports the calculation CRC-16. If you want to change to calculate for CRC-32 bits, use the CMake config command below:

- With integration test:

by manually

```sh
$ cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_INTEGRATION_TESTING=ON \
  -DBOARD_NAME=brd4264c \
  -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON \
  -DENABLE_CAL_CRC_32=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_INTEGRATION_TESTING=ON \
  -DBOARD_NAME=brd4264c \
  -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON \
  -DENABLE_CRC_USE_SW=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_INTEGRATION_TESTING=ON \
  -DBOARD_NAME=brd4264c \
  -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON \
  -DENABLE_CRC_USE_SW=ON \
  -DENABLE_SW_CRC_TABLE=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_INTEGRATION_TESTING=ON \
  -DBOARD_NAME=brd4264c \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON \
  -DENABLE_CRC_USE_SW=ON \
  -DENABLE_SW_CRC_TABLE=ON \
  -DENABLE_CAL_CRC_32=ON
```

or by automatically

```sh
$ bash execute_integration_test.sh brd4264c all all 440111030 GCC "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CAL_CRC_32=ON"

$ bash execute_integration_test.sh brd4264c all all 440111030 GCC "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CRC_USE_SW=ON"

$ bash execute_integration_test.sh brd4264c all all 440111030 GCC "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON"

$ bash execute_integration_test.sh brd4264c all all 440111030 GCC "-DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON"
```


Here are some options to support running tests of invariable memory modules:

- ENABLE_CAL_CRC_32

- ENABLE_CRC_USE_SW (if this option is ON, you can enable option: ENABLE_SW_CRC_TABLE for using the pre-defined table for calculating or not)

> [!NOTE]
> Only use the ENABLE_SW_CRC_TABLE option when the ENABLE_CRC_USE_SW option is ON, otherwise, an error will be reported during the build process.