# Guideline for Running Unit Tests

This document describes how to build and run IEC60730 unit tests using `IEC60730 Library SDK Extension v2.2.0`. The test framework supports both:

- Gecko SDK (GSDK) 4.5.0
- Simplicity SDK (SSDK) 2026.6.0

The same build and execution flow can be used for both SDKs after selecting the appropriate SDK environment.

## Select SDK Version

### GSDK 4.5.0

```sh
export SDK_PATH=<path_to_gsdk_4.5.0>
```

### SSDK 2026.6.0

```sh
export SDK_PATH=<path_to_ssdk_2026.6.0>
```

If dual SDK switching is supported in your workspace:

```sh
make apply-sdk-profile PROFILE=gecko_4_5
make apply-sdk-profile PROFILE=ssdk_2026_6
```

---

## Export Variables

> [!NOTE]
> Before running CMake, you need to export some variables first.

Export SDK_PATH=<path_to_sdk>, TOOL_DIRS=<path_to_toolchain>, TOOL_CHAINS, FLASH_REGIONS_TEST (flash start address used to calculate CRC for the Invariable Memory module), JLINK_PATH, and add the slc-cli path to PATH before running CMake configuration.

If you want to calculate the CRC from the flash start address to the end of flash when using either GSDK 4.5.0 or Simplicity SDK 2026.6.0, configure the required environment variables as follows.

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
Or, if you want to calculate CRC for multiple flash regions on EFR32BG24A010F1024IM40:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```
For `EFR32BG21A010F1024IM32`, the flash start address is `0x0000000`, so set:
```sh
$ export FLASH_REGIONS_TEST=0x0000000
```

Or, if you want to calculate CRC for multiple flash regions on EFR32BG21A010F1024IM32:

```sh
$ export FLASH_REGIONS_TEST="0x00000000 0x00000050 0x000000a0 0x000000f0 0x00000140 0x00000190"
```

## Manually run unit tests

Before building the unit tests, prepare the workspace and generate the build files. The following examples show how to configure CMake for both supported SDK versions.

```sh
$ make prepare
$ cd build

# EFR32BG21A010F1024IM32
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=EFR32BG21A010F1024IM32

# EFR32BG24A010F1024IM40
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=EFR32BG24A010F1024IM40
```

CMake Build Individual Components

```sh
$ cmake --build . --target unit_test_iec60730_post -j4
$ cmake --build . --target unit_test_iec60730_bist -j4
$ cmake --build . --target unit_test_iec60730_program_counter -j4
$ cmake --build . --target unit_test_iec60730_safety_check -j4
$ cmake --build . --target unit_test_iec60730_irq -j4
$ cmake --build . --target unit_test_iec60730_system_clock -j4
$ cmake --build . --target unit_test_iec60730_watchdog -j4
$ cmake --build . --target unit_test_iec60730_cpu_registers -j4
$ cmake --build . --target unit_test_iec60730_variable_memory -j4
$ cmake --build . --target unit_test_iec60730_invariable_memory -j4
```

## Automatically run unit tests

Command run

```sh
$ bash execute_unit_test.sh $1 $2 $3 $4 $5 $6
```

With the input arguments, there is the following information.

- $1: BOARD_NAME (`EFR32BG21A010F1024IM32` or `EFR32BG24A010F1024IM40`)
- $2: Task (`all`, `gen-only`, `run-only`)
- $3: Components (`all`, `unit_test_iec60730_bist`, `unit_test_iec60730_post`, ...)
- $4: ADAPTER_SN
- $5: Compiler (`GCC`)
- $6: OPTION_SUPPORT_UNIT_TEST:
  - `-DENABLE_CAL_CRC_32=ON`
  - `-DENABLE_CRC_USE_SW=ON`
  - `-DENABLE_SW_CRC_TABLE=ON`

The list of supported unit test components includes:

- unit_test_iec60730_post

- unit_test_iec60730_bist

- unit_test_iec60730_program_counter

- unit_test_iec60730_safety_check

- unit_test_iec60730_irq

- unit_test_iec60730_system_clock

- unit_test_iec60730_watchdog

- unit_test_iec60730_cpu_registers

- unit_test_iec60730_variable_memory

- unit_test_iec60730_invariable_memory

Before running the bash file, you need to install Jlink, Srecord, and slc tool, refer [Overview](./index.md) to set up some environment variables as follows:
- If the compiler is GCC

If you want to calculate the CRC from the flash start address to the end of flash, configure the required environment variables and set `FLASH_REGIONS_TEST` to the flash start address of the target device.

### Gecko SDK 4.5.0

```sh
$ export SDK_PATH=/home/Desktop/gecko-sdk
$ export TOOL_DIRS=/home/.local/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
$ export TOOL_CHAINS=GCC
$ export JLINK_PATH=/opt/SEGGER/JLink_V968a/libjlinkarm.so
$ export PATH=$HOME/.silabs/slt/installs/archive/slc-cli-v6.0.23/slc_cli:$PATH
```

For `EFR32BG24A010F1024IM40`, the flash start address is `0x8000000`, so set:

```sh
$ export FLASH_REGIONS_TEST=0x8000000
```

Or, if you want to calculate CRC for multiple flash regions on `EFR32BG24A010F1024IM40`, set:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

For `EFR32BG21A010F1024IM32`, the flash start address is `0x0000000`, so set:

```sh
$ export FLASH_REGIONS_TEST=0x0000000
```

Or, if you want to calculate CRC for multiple flash regions on `EFR32BG21A010F1024IM32`, set:

```sh
$ export FLASH_REGIONS_TEST="0x00000000 0x00000050 0x000000a0 0x000000f0 0x00000140 0x00000190"
```

### Simplicity SDK 2026.6.0

```sh
$ export SDK_PATH=/home/.silabs/slt/installs/conan/p/simpl508ee6c1a6569/p
$ export TOOL_DIRS=/home/.local/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
$ export TOOL_CHAINS=GCC
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export PATH=$PATH:/home/.silabs/slt/installs/archive/slc-cli-v6.0.23/slc_cli
```

```sh
$ export FLASH_REGIONS_TEST=0x8000000
```

Or, if you want to calculate CRC for multiple flash regions on `EFR32BG24A010F1024IM40`, set:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

For `EFR32BG21A010F1024IM32`, the flash start address is `0x0000000`, so set:

```sh
$ export FLASH_REGIONS_TEST=0x0000000
```

Or, if you want to calculate CRC for multiple flash regions on `EFR32BG21A010F1024IM32`, set:

```sh
$ export FLASH_REGIONS_TEST="0x00000000 0x00000050 0x000000a0 0x000000f0 0x00000140 0x00000190"
```

> [!NOTE]
> The current unit test implementation only supports CRC calculation for a single flash region, from the flash start address to the end of flash. Therefore, set `FLASH_REGIONS_TEST` to the flash start address of the target device.
>
> Examples:
>
> - EFR32BG24A010F1024IM40: `$ export FLASH_REGIONS_TEST=0x8000000`
> - EFR32BG21A010F1024IM32: `$ export FLASH_REGIONS_TEST=0x0000000`


### Example

The following examples demonstrate how to automatically build and run all supported unit tests using the GCC toolchain for both GSDK 4.5.0 and Simplicity SDK 2026.6.0:

```sh
# EFR32BG21A010F1024IM32
$ bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC

# EFR32BG24A010F1024IM40
$ bash execute_unit_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC
```

## CRC calculation options

When running build CMake to run unit tests and integration tests for invariable memory modules, the CRC calculation image file will have the suffix _crc16 or _crc32, you must flash the image file with this suffix.

With the commands above, the default value supports the calculation CRC-16. If you want to change to calculate for CRC-32 bits, use the CMake config command below:

- With unit test:

by manually

For unit tests, the following CMake options can be used to configure CRC calculation.

```sh
# EFR32BG21A010F1024IM32
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=EFR32BG21A010F1024IM32 -DENABLE_CAL_CRC_32=ON
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=EFR32BG21A010F1024IM32 -DENABLE_CRC_USE_SW=ON
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON \
  -DBOARD_NAME=EFR32BG21A010F1024IM32 -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON \
  -DBOARD_NAME=EFR32BG21A010F1024IM32 -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON

# EFR32BG24A010F1024IM40
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=EFR32BG24A010F1024IM40 -DENABLE_CAL_CRC_32=ON
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=EFR32BG24A010F1024IM40 -DENABLE_CRC_USE_SW=ON
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON \
  -DBOARD_NAME=EFR32BG24A010F1024IM40 -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON \
  -DBOARD_NAME=EFR32BG24A010F1024IM40 -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON
```

or by automatically

### Example

With the GCC toolchain:

```sh
# EFR32BG21A010F1024IM32
$ bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC
$ bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC "-DENABLE_CAL_CRC_32=ON"
$ bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC "-DENABLE_CRC_USE_SW=ON"
$ bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC "-DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON"
$ bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all 440319421 GCC "-DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON"

# EFR32BG24A010F1024IM40
$ bash execute_unit_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC
$ bash execute_unit_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC "-DENABLE_CAL_CRC_32=ON"
$ bash execute_unit_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC "-DENABLE_CRC_USE_SW=ON"
$ bash execute_unit_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC "-DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON"
$ bash execute_unit_test.sh EFR32BG24A010F1024IM40 all all 440319421 GCC "-DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON"
```

Here are some options to support running tests of invariable memory modules:

- ENABLE_CAL_CRC_32

- ENABLE_CRC_USE_SW (if this option is ON, you can enable option: ENABLE_SW_CRC_TABLE for using the pre-defined table for calculating or not)

> [!NOTE]
> Only use the ENABLE_SW_CRC_TABLE option when the ENABLE_CRC_USE_SW option is ON, otherwise, an error will be reported during the build process.