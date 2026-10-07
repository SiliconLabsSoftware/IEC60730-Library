# Guideline for Running Unit Tests

This document describes how to build and run IEC60730 unit tests with **IEC60730 Library SDK Extension v2.2.0**.

Supported SDKs:

| SDK | Version | Notes |
| --- | --- | --- |
| Simplicity SDK (SSDK / SimSDK) | 2026.6.0 | Clone/setup through SLT (`make bootstrap` / `recipe.toml`) |
| Gecko SDK (GSDK) | 4.5.0 | Maintained on GitHub — set `GSDK_PATH` or allow cache/download |

> [!NOTE]
> **SLT supports Simplicity SDK (SimSDK) 2026.6.0** clone/setup.
> **GSDK 4.5.0 is maintained on GitHub**, so resolve it with `GSDK_PATH`, a Docker mount, or `script/set_gsdk.sh` (see the repository [README](../README.md)).

The build and execution flow is the same for both SDKs after you select the matching profile and environment.

## Prerequisites

- GCC Arm Embedded Toolchain (`arm-none-eabi-gcc`)
- Semtech/SEGGER J-Link software (for on-device execution)
- SRecord (`srecord`) for CRC image post-processing
- Simplicity CLI (`slc`) on `PATH`
- Simplicity Commander (`commander`) on `PATH` when the script queries device info

## Select SDK version

Apply the SDK profile that matches your target SDK, then load the environment:

```sh
make apply-sdk-profile PROFILE=gecko_4_5
# or
make apply-sdk-profile PROFILE=ssdk_2026_6

source script/set_env.sh
```

`script/set_env.sh` configures `SDK_PATH`, toolchain paths, and related variables for the active profile.

> [!NOTE]
> Prefer `source script/set_env.sh` over hard-coded machine paths.
> When configuring CMake manually, export the required variables before running CMake.
> `cmake/toolchain.cmake` can also discover `arm-none-eabi-gcc` from `PATH`.

### Manual environment variables (reference only)

If you configure the environment manually:

| Variable | Purpose |
| --- | --- |
| `SDK_PATH` | Root of GSDK 4.5.0 or Simplicity SDK 2026.6.0 |
| `TOOL_DIRS` | Directory containing `arm-none-eabi-gcc` |
| `TOOL_CHAINS` | `GCC` |
| `JLINK_PATH` | Path to `libjlinkarm.so` |
| `FLASH_REGIONS_TEST` | Flash start address used for IMC CRC calculation |
| `PATH` | Must include `slc` |

Example shapes (replace paths with your local installs):

```sh
# GSDK 4.5.0
export SDK_PATH=/path/to/gecko-sdk
export TOOL_DIRS=/path/to/arm-gnu-toolchain/bin
export TOOL_CHAINS=GCC
export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
export PATH=/path/to/slc_cli:$PATH

# Simplicity SDK 2026.6.0 (typically from SLT)
export SDK_PATH=$(slt where simplicity-sdk)
export TOOL_DIRS=/path/to/arm-gnu-toolchain/bin
export TOOL_CHAINS=GCC
export JLINK_PATH=/opt/SEGGER/JLink/libjlinkarm.so
export PATH=/path/to/slc_cli:$PATH
```

### Flash regions (`FLASH_REGIONS_TEST`)

| Device | Flash start |
| --- | --- |
| `EFR32BG24A010F1024IM40` | `0x8000000` |
| `EFR32BG21A010F1024IM32` | `0x0000000` |

```sh
# BG24
export FLASH_REGIONS_TEST=0x8000000

# BG21
export FLASH_REGIONS_TEST=0x0000000
```

> [!NOTE]
> The current unit test implementation supports CRC calculation for a **single** continuous flash region (start address to end of flash). Set `FLASH_REGIONS_TEST` to the device flash start address only.

## Manually run unit tests

From the repository root:

```sh
make prepare
cd build

# BG21
cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_UNIT_TESTING=ON \
  -DBOARD_NAME=EFR32BG21A010F1024IM32

# BG24
cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_UNIT_TESTING=ON \
  -DBOARD_NAME=EFR32BG24A010F1024IM40
```

Build individual components:

```sh
cmake --build . --target unit_test_iec60730_post -j4
cmake --build . --target unit_test_iec60730_bist -j4
cmake --build . --target unit_test_iec60730_program_counter -j4
cmake --build . --target unit_test_iec60730_safety_check -j4
cmake --build . --target unit_test_iec60730_irq -j4
cmake --build . --target unit_test_iec60730_system_clock -j4
cmake --build . --target unit_test_iec60730_watchdog -j4
cmake --build . --target unit_test_iec60730_cpu_registers -j4
cmake --build . --target unit_test_iec60730_variable_memory -j4
cmake --build . --target unit_test_iec60730_invariable_memory -j4
```

## Automatically run unit tests

Run the helper script **from the `test/` directory** (it resolves paths relative to that location):

```sh
cd test
bash execute_unit_test.sh <BOARD_NAME> <TASK> <COMPONENTS> <ADAPTER_SN> <COMPILER> [OPTIONS]
```

| Argument | Values |
| --- | --- |
| `$1` BOARD_NAME | `EFR32BG21A010F1024IM32` or `EFR32BG24A010F1024IM40` |
| `$2` TASK | `all`, `gen-only`, `run-only` |
| `$3` COMPONENTS | `all`, or a single target such as `unit_test_iec60730_bist` |
| `$4` ADAPTER_SN | J-Link / adapter serial number |
| `$5` COMPILER | `GCC` (IAR is not supported) |
| `$6` OPTIONS | Optional CMake flags (quoted) |

Supported component targets:

- `unit_test_iec60730_post`
- `unit_test_iec60730_bist`
- `unit_test_iec60730_program_counter`
- `unit_test_iec60730_safety_check`
- `unit_test_iec60730_irq`
- `unit_test_iec60730_system_clock`
- `unit_test_iec60730_watchdog`
- `unit_test_iec60730_cpu_registers`
- `unit_test_iec60730_variable_memory`
- `unit_test_iec60730_invariable_memory`

### Examples

```sh
cd test

# Build and run all unit tests
bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all <ADAPTER_SN> GCC
bash execute_unit_test.sh EFR32BG24A010F1024IM40 all all <ADAPTER_SN> GCC

# CRC / software CRC options
bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all <ADAPTER_SN> GCC "-DENABLE_CAL_CRC_32=ON"
bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all <ADAPTER_SN> GCC "-DENABLE_CRC_USE_SW=ON"
bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all <ADAPTER_SN> GCC "-DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON"
bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all <ADAPTER_SN> GCC "-DENABLE_CRC_USE_SW=ON -DENABLE_SW_CRC_TABLE=ON -DENABLE_CAL_CRC_32=ON"
```

For environment setup details, see also [Overview](./index.md).

## CRC calculation options

When building invariable-memory unit tests, CRC post-processing produces images with a `_crc16` or `_crc32` suffix. Flash the image that includes the CRC suffix.

Default CRC mode is CRC-16. Enable CRC-32 or software CRC with CMake options:

| Option | Description |
| --- | --- |
| `ENABLE_CAL_CRC_32` | Use CRC-32 instead of CRC-16 |
| `ENABLE_CRC_USE_SW` | Use software CRC instead of GPCRC hardware |
| `ENABLE_SW_CRC_TABLE` | Use a precomputed software CRC table (**requires** `ENABLE_CRC_USE_SW=ON`) |

Manual CMake examples:

```sh
cd build

cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_UNIT_TESTING=ON \
  -DBOARD_NAME=EFR32BG21A010F1024IM32 \
  -DENABLE_CAL_CRC_32=ON

cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_UNIT_TESTING=ON \
  -DBOARD_NAME=EFR32BG21A010F1024IM32 \
  -DENABLE_CRC_USE_SW=ON \
  -DENABLE_SW_CRC_TABLE=ON
```

> [!NOTE]
> Enable `ENABLE_SW_CRC_TABLE` only when `ENABLE_CRC_USE_SW` is `ON`. Otherwise the build fails.
