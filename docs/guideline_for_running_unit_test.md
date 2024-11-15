# Guideline for running unit test

## Export Variable

> [!NOTE]
> Before running CMake, you need to export some variables first.

Export SDK_PATH=<path_to_sdk>, TOOL_DIRS=<path_to_toolchain>, TOOL_CHAINS, FLASH_REGIONS_TEST (flash start address support calculate CRC for module invariable memory), JLINK_PATH and path to `slc-cli` tool a before run config CMake.

If you want to calculate from the start address to the end address of Flash:

```sh
$ export SDK_PATH=~/SimplicityStudio/SDKs/gecko_sdk
$ export TOOL_DIRS=~/Downloads/SimplicityStudio_v5/developer/toolchains/gnu_arm/12.2.rel1_2023.7/bin
$ export TOOL_CHAINS=GCC
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
$ export PATH=$PATH:/media/slc_cli/slc_cli_linux_check/slc_cli
$ export FLASH_REGIONS_TEST=0x8000000
```

Or if you want to calculate multiple regions:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

with FLASH_REGIONS_TEST=0x8000000 is the flash start address of board name brd4187c (chip EFR32MG24)

## Build test for IAR tool

To build tests for the IAR tool, if you run a manual test, you must run the pre-build command below.

```sh
$ make prepare
$ cd build
$ cmake --toolchain ../cmake/toolchain.cmake .. -DPRE_BUILD_IAR_TOOL=ON -DBOARD_NAME=${BOARD_NAME} $OPTION_PRE_BUILD_IAR_TOOL
$ cd ..
$ make prepare
```

> [!NOTE]
> Keep `$OPTION_PRE_BUILD_IAR_TOOL` the same when running unit test CMake config

For example, build unit test:

```sh
$ make prepare
$ cd build
$ cmake --toolchain ../cmake/toolchain.cmake .. -DPRE_BUILD_IAR_TOOL=ON -DBOARD_NAME=brd4187c -DENABLE_UNIT_TESTING=ON -DENABLE_CRC_USE_SW=ON -DENABLE_CAL_CRC_32=ON
$ cd ..
$ make prepare
```

or you can run bash script `pre_build_iar.sh` in path [./../simplicity_sdk/pre_build_iar.sh]() with:

- $1: BOARD_NAME: brd4187c or EFR32MG24B220F1536IM48
- $2: OPTION_UNIT_TEST: -DENABLE_UNIT_TESTING=ON, etc...

```sh
$ bash pre_build_iar.sh $BOARD_NAME $OPTION_UNIT_TEST
```

For example:

```sh
$ bash pre_build_iar.sh brd4187c "-DENABLE_UNIT_TESTING=ON"
```

## Manually run unit tests

CMake config

```sh
$ make prepare
$ cd build
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=brd4187c
```

CMake Build

```sh
$ cmake --build . --target unit_test_info -j4
```

or

```sh
$ make unit_test_info -j4
```

## Automatically run unit tests

Command run

```sh
bash execute_test.sh $1 $2 $3 $4 $5 $6
```

With the input arguments, there is the following information.

- $1: BOARD_NAME: brd4187c or EFR32MG24B220F1536IM48
- $2: task: all, gen-only, run-only
- $3: components: all, unit_test_iec60730_bist, unit_test_iec60730_post, ...
- $4: ADAPTER_SN
- $5: compiler: GCC, IAR
- $6: OPTION_SUPPORT_UNIT_TEST: "-DENABLE_CAL_CRC_32=ON -DENABLE_CRC_USE_SW"

Which, components list that supports unit testing includes:

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
If the compiler is GCC
- If the compiler is GCC:

If you want to calculate from the start address to the end address of Flash:

```sh
$ export SDK_PATH=~/SimplicityStudio/SDKs/gecko_sdk
$ export TOOL_DIRS=~/Downloads/SimplicityStudio_v5/developer/toolchains/gnu_arm/12.2.rel1_2023.7/bin
$ export TOOL_CHAINS=GCC
$ export FLASH_REGIONS_TEST=0x8000000
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
```

Or if you want to calculate multiple regions:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

> [!NOTE]
> In the current unit test file, only enable computation in one region: from the start address of ​​the flash to the end of the flash. Therefore, just export the flash's starting address. For example, chip EFR32MG24:
>> $ export FLASH_REGIONS_TEST=0x8000000

### Example

- With GCC toolchain:

```sh
bash execute_unit_test.sh brd4187c all all 440111030 GCC
```

- With IAR toolchain:

```sh
bash execute_unit_test.sh brd4187c all all 440111030 IAR
```

## CRC calculation options

When running build CMake to run unit tests and integration tests for invariable memory modules, the CRC calculation image file will have the suffix _crc16 or _crc32, you must flash the image file with this suffix.

With the commands above, the default value supports the calculation CRC-16. If you want to change to calculate for CRC-32 bits, use the CMake config command below:

- With unit test:

by manually

```sh
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=brd4187c -DENABLE_CAL_CRC_32=ON
```

or by automatically

```sh
bash execute_unit_test.sh brd4187c all all 440111030 GCC "-DENABLE_CAL_CRC_32=ON"
```

Here are some options to support running tests of invariable memory modules:

- ENABLE_CAL_CRC_32

- ENABLE_CRC_USE_SW (if this option is ON, you can enable option: ENABLE_SW_CRC_TABLE for using the pre-defined table for calculating or not)

> [!NOTE]
> Only use the ENABLE_SW_CRC_TABLE option when the ENABLE_CRC_USE_SW option is ON, otherwise, an error will be reported during the build process.