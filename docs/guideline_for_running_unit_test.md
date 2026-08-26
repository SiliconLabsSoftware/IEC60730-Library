# Guideline for running unit test

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

with FLASH_REGIONS_TEST=0x8000000 is the flash start address of board name brd4187c (chip EFR32MG24) and brd4264c (chip EFR32FG23)

## Manually run unit tests

Before building the unit tests, prepare the workspace and generate the build files. For example cmake config for board name brd4264c (chip EFR32FG23).

```sh
$ make prepare
$ cd build
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=brd4187c
```

CMake Build Individual components

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
$ bash execute_test.sh $1 $2 $3 $4 $5 $6
```

With the input arguments, there is the following information.

- $1: BOARD_NAME: brd4264c or EFR32FG23B020F512IM48
- $2: task: all, gen-only, run-only
- $3: components: all, unit_test_iec60730_bist, unit_test_iec60730_post, ...
- $4: ADAPTER_SN
- $5: compiler: GCC
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
$ export SDK_PATH=/home/svc_sqa_automation/.silabs/slt/installs/conan/p/simpl508ee6c1a6569/p
$ export TOOL_DIRS=/home/svc_sqa_automation/.local/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
$ export TOOL_CHAINS=GCC
$ export FLASH_REGIONS_TEST=0x8000000
$ export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
```

Or if you want to calculate multiple regions:

```sh
$ export FLASH_REGIONS_TEST="0x8000000 0x8000050 0x80000a0 0x80000f0 0x8000140 0x8000190"
```

> [!NOTE]
> In the current unit test file, only enable computation in one region: from the start address of ​​the flash to the end of the flash. Therefore, just export the flash's starting address. For example, chip EFR32MG24, chip EFR32MG23:
>> $ export FLASH_REGIONS_TEST=0x8000000

### Example

- With GCC toolchain:

```sh
$ bash execute_unit_test.sh brd4264C all all 440111030 GCC
```

## CRC calculation options

When running build CMake to run unit tests and integration tests for invariable memory modules, the CRC calculation image file will have the suffix _crc16 or _crc32, you must flash the image file with this suffix.

With the commands above, the default value supports the calculation CRC-16. If you want to change to calculate for CRC-32 bits, use the CMake config command below:

- With unit test:

by manually

```sh
$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=brd4187c -DENABLE_CAL_CRC_32=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=brd4187c -DENABLE_CRC_USE_SW=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=brd4187c -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON

$ cmake --toolchain ../cmake/toolchain.cmake .. -DENABLE_UNIT_TESTING=ON -DBOARD_NAME=brd4187c -DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON
```

or by automatically

```sh
$ bash execute_unit_test.sh brd4264c all all 440111030 GCC "-DENABLE_CAL_CRC_32=ON"

$ bash execute_unit_test.sh brd4264c all all 440111030 GCC "-DENABLE_CRC_USE_SW=ON"

$ bash execute_unit_test.sh brd4264c all all 440111030 GCC "-DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON"

$ bash execute_unit_test.sh brd4264c all all 440111030 GCC "-DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON"
```

Here are some options to support running tests of invariable memory modules:

- ENABLE_CAL_CRC_32

- ENABLE_CRC_USE_SW (if this option is ON, you can enable option: ENABLE_SW_CRC_TABLE for using the pre-defined table for calculating or not)

> [!NOTE]
> Only use the ENABLE_SW_CRC_TABLE option when the ENABLE_CRC_USE_SW option is ON, otherwise, an error will be reported during the build process.