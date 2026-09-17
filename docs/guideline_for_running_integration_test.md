# Guideline for Running Integration Tests

This document describes how to build and run IEC60730 integration tests using `IEC60730 Library SDK Extension v2.2.0`. The test framework supports both:

- Gecko SDK (GSDK) 4.5.0
- Simplicity SDK (SSDK) 2026.6.0

The same build and execution flow can be used for both SDKs after selecting the appropriate SDK environment.

## Select SDK Version

### GSDK 4.5.0

```sh
$ export SDK_PATH=<path_to_gsdk_4.5.0>
```

### SSDK 2026.6.0

```sh
$ export SDK_PATH=<path_to_ssdk_2026.6.0>
```

If dual SDK switching is supported in your workspace:

```sh
make apply-sdk-profile PROFILE=gecko_4_5
make apply-sdk-profile PROFILE=ssdk_2026_6
```

## Export Variables

> [!NOTE]
> Before running CMake, you need to export some variables first.

Export SDK_PATH=<path_to_sdk>, TOOL_DIRS=<path_to_toolchain>, TOOL_CHAINS, FLASH_REGIONS_TEST (flash start address used to calculate CRC for the Invariable Memory module), JLINK_PATH, and add the slc-cli path to PATH before running CMake configuration.

If you want to calculate the CRC from the flash start address to the end of flash, configure the required environment variables as follows.

### GSDK 4.5.0

```sh
$ export SDK_PATH=/home/Desktop/gecko-sdk
$ export TOOL_DIRS=/home/.local/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
$ export TOOL_CHAINS=GCC
$ export JLINK_PATH=/opt/SEGGER/JLink_V968a/libjlinkarm.so
$ export PATH=$HOME/.silabs/slt/installs/archive/slc-cli-v6.0.23/slc_cli:$PATH
```

### Simplicity SDK 2026.6.0

```sh
$ export SDK_PATH=/home/.silabs/slt/installs/conan/p/simpl508ee6c1a6569/p
$ export TOOL_DIRS=/home/.local/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
$ export TOOL_CHAINS=GCC
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export PATH=$PATH:/home/.silabs/slt/installs/archive/slc-cli-v6.0.23/slc_cli
```

For `EFR32BG24A010F1024IM40`, the flash start address is `0x8000000`, so set:

```sh
$ export FLASH_REGIONS_TEST=0x8000000
```

Or, if you want to calculate CRC for multiple flash regions on `EFR32BG24A010F1024IM40`:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

For `EFR32BG21A010F1024IM32`, the flash start address is `0x0000000`, so set:

```sh
$ export FLASH_REGIONS_TEST=0x0000000
```

Or, if you want to calculate CRC for multiple flash regions on `EFR32BG21A010F1024IM32`:

```sh
$ export FLASH_REGIONS_TEST="0x00000000 0x0000050 0x00000A0 0x00000F0 0x00000140 0x00000190"
```

To run integration tests manually, for the watchdog module, you need to connect the device to ethernet. Export CHIP, ADAPTER_SN, LST_PATH, JLINK_PATH, and the device's IP address and run the test script, for example:

```sh
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export HOST_IP=192.168.1.69
$ export ADAPTER_SN=440319421

# EFR32BG21A010F1024IM32
$ export CHIP=EFR32BG21A010F1024IM32

# EFR32BG24A010F1024IM40
$ export CHIP=EFR32BG24A010F1024IM40
```

If test secure peripherals or non-secure peripherals:

```sh
# EFR32BG21A010F1024IM32
$ export LST_PATH=/home/Desktop/IEC60730-Library/build/test/integration_test/build/EFR32BG21A010F1024IM32/GCC/integration_test_iec60730_watchdog/NS

# EFR32BG24A010F1024IM40
$ export LST_PATH=/home/Desktop/IEC60730-Library/build/test/integration_test/build/EFR32BG24A010F1024IM40/GCC/integration_test_iec60730_watchdog/NS
```
```sh
# EFR32BG21A010F1024IM32
$ export LST_PATH=/home/Desktop/IEC60730-Library/build/test/integration_test/build/EFR32BG21A010F1024IM32/GCC/integration_test_iec60730_watchdog/S

# EFR32BG24A010F1024IM40
$ export LST_PATH=/home/Desktop/IEC60730-Library/build/test/integration_test/build/EFR32BG24A010F1024IM40/GCC/integration_test_iec60730_watchdog/S
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

# EFR32BG21A010F1024IM32
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG21A010F1024IM32

# EFR32BG24A010F1024IM40
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG24A010F1024IM40
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

- INTEGRATION_TEST_WDOG1_ENABLE: enable Watchdog 1 testing on device if the device supports

To support running integration tests for the variable memory module, there is 1 option when running CMake config:

- INTEGRATION_TEST_USE_MARCHX_DISABLE: disable using MarchX algorithm

By default, the Variable Memory Check module uses the MarchX algorithm.
To disable MarchX, enable the following Cmake option. For example:

```sh
# EFR32BG21A010F1024IM32
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG21A010F1024IM32

# EFR32BG24A010F1024IM40
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG24A010F1024IM40
```

For devices that have a Trust zone implemented, secure and non-secure peripherals need to be tested.
Default enable checks non-secure peripherals. To check secure peripherals enable this option when running 
CMake config: TEST_SECURE_PERIPHERALS_ENABLE. For example:

```sh
# EFR32BG21A010F1024IM32
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON \
  -DTEST_SECURE_PERIPHERALS_ENABLE=ON -DBOARD_NAME=EFR32BG21A010F1024IM32

# EFR32BG24A010F1024IM40
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON \
  -DTEST_SECURE_PERIPHERALS_ENABLE=ON -DBOARD_NAME=EFR32BG24A010F1024IM40
```

For devices that support 2 watchdogs, if you want to test both watchdogs, enable option INTEGRATION_TEST_WDOG1_ENABLE to ON when running Cmake config:

```sh
# EFR32BG21A010F1024IM32
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DBOARD_NAME=EFR32BG21A010F1024IM32

# EFR32BG24A010F1024IM40
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DBOARD_NAME=EFR32BG24A010F1024IM40
```

To run Watchdog integration tests manually, connect the target device to an Ethernet network. Export `CHIP`, `ADAPTER_SN`, `JLINK_PATH`, and `HOST_IP` before running the test script.

```sh
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export HOST_IP=192.168.1.69
$ export ADAPTER_SN=440319421

# EFR32BG21A010F1024IM32
$ export CHIP=EFR32BG21A010F1024IM32

# EFR32BG24A010F1024IM40
$ export CHIP=EFR32BG24A010F1024IM40
```

If running Watchdog integration tests, export `LST_PATH` according to the target build configuration.

For secure peripherals:

```sh
# EFR32BG21A010F1024IM32
$ export LST_PATH=/home/Desktop/IEC60730-Library/build/test/integration_test/build/EFR32BG21A010F1024IM32/GCC/integration_test_iec60730_watchdog/S

# EFR32BG24A010F1024IM40
$ export LST_PATH=/home/Desktop/IEC60730-Library/build/test/integration_test/build/EFR32BG24A010F1024IM40/GCC/integration_test_iec60730_watchdog/S
```

For non-secure peripherals:

```sh
# EFR32BG21A010F1024IM32
$ export LST_PATH=/home/Desktop/IEC60730-Library/build/test/integration_test/build/EFR32BG21A010F1024IM32/GCC/integration_test_iec60730_watchdog/NS

# EFR32BG24A010F1024IM40
$ export LST_PATH=/home/Desktop/IEC60730-Library/build/test/integration_test/build/EFR32BG24A010F1024IM40/GCC/integration_test_iec60730_watchdog/NS
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
$ bash execute_integration_test.sh $1 $2 $3 $4 $5 $6
```

With the input arguments, there is the following information.

- `$1`: BOARD_NAME (`EFR32BG21A010F1024IM32` or `EFR32BG24A010F1024IM40`)
- `$2`: Task (`all`, `gen-only`, `run-only`)
- `$3`: Components (`all`, `integration_test_iec60730_program_counter`, `integration_test_iec60730_irq`, `integration_test_iec60730_system_clock`, `integration_test_iec60730_watchdog`, `integration_test_iec60730_cpu_registers`, `integration_test_iec60730_variable_memory`, `integration_test_iec60730_invariable_memory`)
- `$4`: ADAPTER_SN
- `$5`: Compiler (`GCC`)
- `$6`: OPTION_SUPPORT_INTEGRATION_TEST:
  - `-DENABLE_CAL_CRC_32=ON`
  - `-DENABLE_CRC_USE_SW=ON`
  - `-DTEST_SECURE_PERIPHERALS_ENABLE=ON`
  - `-DINTEGRATION_TEST_WDOG1_ENABLE=ON`
  - `-DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON`

Which, components list that support testing includes:

- integration_test_iec60730_program_counter

- integration_test_iec60730_irq

- integration_test_iec60730_system_clock

- integration_test_iec60730_watchdog

- integration_test_iec60730_cpu_registers

- integration_test_iec60730_variable_memory

- integration_test_iec60730_invariable_memory

Before running the bash file, you need to install Jlink, Srecord, and slc tool, refer [Overview](./index.md) to set up some environment variables as follows:
- If the compiler is GCC

If you want to calculate the CRC from the flash start address to the end of flash when using either GSDK 4.5.0 or Simplicity SDK 2026.6.0, configure the required environment variables as follows.

### GSDK 4.5.0

```sh
$ export SDK_PATH=/home/Desktop/gecko-sdk
$ export TOOL_DIRS=/home/.local/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
$ export TOOL_CHAINS=GCC
$ export JLINK_PATH=/opt/SEGGER/JLink_V968a/libjlinkarm.so
$ export PATH=$HOME/.silabs/slt/installs/archive/slc-cli-v6.0.23/slc_cli:$PATH
$ export HOST_IP=192.168.1.69
$ export ADAPTER_SN=440319421
```

### Simplicity SDK 2026.6.0

```sh
$ export SDK_PATH=/home/.silabs/slt/installs/conan/p/simpl508ee6c1a6569/p
$ export TOOL_DIRS=/home/.local/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
$ export TOOL_CHAINS=GCC
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export PATH=$PATH:/home/.silabs/slt/installs/archive/slc-cli-v6.0.23/slc_cli
$ export HOST_IP=192.168.1.69
$ export ADAPTER_SN=440319421
```

For `EFR32BG24A010F1024IM40`:

```sh
$ export CHIP=EFR32BG24A010F1024IM40
$ export FLASH_REGIONS_TEST=0x8000000
```

Or, if you want to calculate CRC for multiple flash regions on `EFR32BG24A010F1024IM40`:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

For `EFR32BG21A010F1024IM32`:

```sh
$ export CHIP=EFR32BG21A010F1024IM32
$ export FLASH_REGIONS_TEST=0x0000000
```

Or, if you want to calculate CRC for multiple flash regions on `EFR32BG21A010F1024IM32`:

```sh
$ export FLASH_REGIONS_TEST="0x0000000 0x0000050 0x00000A0 0x00000F0 0x00000140 0x00000190"
```

> [!NOTE]
> The current integration test implementation only supports CRC calculation for a single flash region, from the flash start address to the end of flash. Therefore, set `FLASH_REGIONS_TEST` to the flash start address of the target device.
>
> Examples:
>
> - EFR32BG24A010F1024IM40: `export FLASH_REGIONS_TEST=0x8000000`
> - EFR32BG21A010F1024IM32: `export FLASH_REGIONS_TEST=0x0000000`

### Example

With the GCC toolchain:

```sh
# EFR32BG21A010F1024IM32
$ bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC

# EFR32BG24A010F1024IM40
$ bash execute_integration_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC
```

To enable CRC-32 calculation:

```sh
# EFR32BG21A010F1024IM32
$ bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC "-DENABLE_CAL_CRC_32=ON"

# EFR32BG24A010F1024IM40
$ bash execute_integration_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC "-DENABLE_CAL_CRC_32=ON"
```

To enable additional integration test options:

```sh
# EFR32BG21A010F1024IM32
$ bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC \
  "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CAL_CRC_32=ON"

# EFR32BG24A010F1024IM40
$ bash execute_integration_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC \
  "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CAL_CRC_32=ON"
```

## CRC calculation options

When running build CMake to run unit tests and integration tests for invariable memory modules, the CRC calculation image file will have the suffix _crc16 or _crc32, you must flash the image file with this suffix.

With the commands above, the default value supports the calculation CRC-16. If you want to change to calculate for CRC-32 bits, use the CMake config command below:

- With integration test:

by manually

For integration tests, the following CMake options can be used to configure CRC calculation.

```sh
# EFR32BG21A010F1024IM32
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG21A010F1024IM32 -DENABLE_CAL_CRC_32=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG21A010F1024IM32 -DENABLE_CRC_USE_SW=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG21A010F1024IM32 -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG21A010F1024IM32 -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON

# EFR32BG24A010F1024IM40
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG24A010F1024IM40 -DENABLE_CAL_CRC_32=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG24A010F1024IM40 -DENABLE_CRC_USE_SW=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG24A010F1024IM40 -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_INTEGRATION_TESTING=ON -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DBOARD_NAME=EFR32BG24A010F1024IM40 -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON
```

or by automatically

### Example

With the GCC toolchain:

```sh
# EFR32BG21A010F1024IM32
$ bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC \
  "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CAL_CRC_32=ON"

$ bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC \
  "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CRC_USE_SW=ON"

$ bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC \
  "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON"

$ bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC \
  "-DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON"

# EFR32BG24A010F1024IM40
$ bash execute_integration_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC \
  "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CAL_CRC_32=ON"

$ bash execute_integration_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC \
  "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CRC_USE_SW=ON"

$ bash execute_integration_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC \
  "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON"

$ bash execute_integration_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC \
  "-DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON"
```


Here are some options to support running tests of invariable memory modules:

- ENABLE_CAL_CRC_32

- ENABLE_CRC_USE_SW (if this option is ON, you can enable option: ENABLE_SW_CRC_TABLE for using the pre-defined table for calculating or not)

> [!NOTE]
> Only use the ENABLE_SW_CRC_TABLE option when the ENABLE_CRC_USE_SW option is ON, otherwise, an error will be reported during the build process.