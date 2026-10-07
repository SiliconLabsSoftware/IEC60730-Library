# Guideline for Running Integration Tests

This document describes how to build and run IEC60730 integration tests with **IEC60730 Library SDK Extension v2.2.0**.

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
- SEGGER J-Link software
- SRecord (`srecord`)
- Simplicity CLI (`slc`) on `PATH`
- Ethernet connectivity to the target for watchdog integration tests (`HOST_IP`)

## Select SDK version

Apply the SDK profile that matches your target SDK, then load the environment.
The committed default is **`ssdk_2026_6`**.

```sh
make apply-sdk-profile PROFILE=ssdk_2026_6
# or
make apply-sdk-profile PROFILE=gecko_4_5

source script/set_env.sh
```

> [!IMPORTANT]
> `make apply-sdk-profile` **overwrites tracked files** (SLCPs, demos, `sdk.env`, etc.) from `sdk_profiles/<profile>/` snapshots and cleans `build/` / `autogen/` / `src/`. Do **not** commit those changes unless you mean to change the default profile. Restore with `make apply-sdk-profile PROFILE=ssdk_2026_6` before committing other work. Details: [README — Dual SDK support](../README.md#dual-sdk-support).

> [!NOTE]
> Prefer `source script/set_env.sh` over hard-coded machine paths.
> When configuring CMake manually, export the required variables before running CMake.
> `cmake/toolchain.cmake` can also discover `arm-none-eabi-gcc` from `PATH`.
> Prefer `make BOARD_NAME=... build-integration` so `FLASH_REGIONS_TEST` is set for BG21/BG24; if you invoke CMake directly, export `FLASH_REGIONS_TEST` yourself (see table below).

### Manual environment variables (reference only)

| Variable | Purpose |
| --- | --- |
| `SDK_PATH` | Root of GSDK 4.5.0 or Simplicity SDK 2026.6.0 |
| `TOOL_DIRS` | Directory containing `arm-none-eabi-gcc` |
| `TOOL_CHAINS` | `GCC` |
| `JLINK_PATH` | Path to `libjlinkarm.so` |
| `FLASH_REGIONS_TEST` | Flash start address used for IMC CRC calculation |
| `HOST_IP` | Target board IP address (watchdog tests) |
| `ADAPTER_SN` | J-Link / adapter serial number |
| `CHIP` | Board/device part number used by test scripts |
| `LST_PATH` | Path to the built watchdog test listing directory (`S` or `NS`) |
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

### Flash regions and board identity

| Device | Flash start | Example CHIP |
| --- | --- | --- |
| `EFR32BG24A010F1024IM40` | `0x8000000` | `EFR32BG24A010F1024IM40` |
| `EFR32BG21A010F1024IM32` | `0x0000000` | `EFR32BG21A010F1024IM32` |

```sh
export FLASH_REGIONS_TEST=0x8000000   # BG24
# or
export FLASH_REGIONS_TEST=0x0000000   # BG21

export CHIP=EFR32BG24A010F1024IM40
export HOST_IP=<board_ip>
export ADAPTER_SN=<adapter_serial>
```

> [!NOTE]
> The current integration test implementation supports CRC calculation for a **single** continuous flash region. Set `FLASH_REGIONS_TEST` to the device flash start address only.

### Watchdog test environment

Watchdog integration tests require Ethernet access to the device. Export `CHIP`, `ADAPTER_SN`, `JLINK_PATH`, `HOST_IP`, and `LST_PATH` before running the Python test script.

`LST_PATH` points at the built watchdog output directory under `build/`, for example:

```sh
# Non-secure peripherals (default)
export LST_PATH=$PWD/build/test/integration_test/build/EFR32BG21A010F1024IM32/GCC/integration_test_iec60730_watchdog/NS

# Secure peripherals
export LST_PATH=$PWD/build/test/integration_test/build/EFR32BG21A010F1024IM32/GCC/integration_test_iec60730_watchdog/S
```

Adjust the board name and `S`/`NS` suffix to match your build.

## Manually run integration tests

From the repository root:

```sh
make prepare
cd build

cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_INTEGRATION_TESTING=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON \
  -DBOARD_NAME=EFR32BG21A010F1024IM32

# or BG24
cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_INTEGRATION_TESTING=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON \
  -DBOARD_NAME=EFR32BG24A010F1024IM40
```

Build targets:

```sh
cmake --build . --target integration_test_iec60730_program_counter -j4
cmake --build . --target integration_test_iec60730_irq -j4
cmake --build . --target integration_test_iec60730_system_clock -j4
cmake --build . --target integration_test_iec60730_watchdog -j4
cmake --build . --target integration_test_iec60730_cpu_registers -j4
cmake --build . --target integration_test_iec60730_variable_memory -j4
cmake --build . --target integration_test_iec60730_invariable_memory -j4
```

### Optional CMake flags

| Option | Purpose |
| --- | --- |
| `TEST_SECURE_PERIPHERALS_ENABLE` | Test secure peripherals (TrustZone). Default is non-secure. |
| `INTEGRATION_TEST_WDOG1_ENABLE` | Enable Watchdog 1 testing when the device supports it |
| `INTEGRATION_TEST_USE_MARCHX_DISABLE` | Disable the MarchX algorithm for variable memory tests |

Examples:

```sh
cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_INTEGRATION_TESTING=ON \
  -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DBOARD_NAME=EFR32BG21A010F1024IM32

cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_INTEGRATION_TESTING=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON \
  -DBOARD_NAME=EFR32BG24A010F1024IM40
```

### Manual Python test helpers

From `test/test_script/` (after building images and exporting env vars):

```sh
INTEGRATION_TEST_WDOG1_ENABLE=enable python3 integration_test_iec60730_watchdog.py GCC
INTEGRATION_TEST_USE_MARCHX_DISABLE=disable python3 integration_test_iec60730_variable_memory.py GCC
INTEGRATION_TEST_ENABLE_CAL_CRC_32=enable python3 integration_test_iec60730_invariable_memory.py GCC
```

## Automatically run integration tests

Run the helper script from `test/` (recommended). Paths are anchored to the script location, so invoking `bash test/execute_integration_test.sh ...` from the repo root also works:

```sh
cd test
bash execute_integration_test.sh <BOARD_NAME> <TASK> <COMPONENTS> <ADAPTER_SN> <COMPILER> [OPTIONS]
```

| Argument | Values |
| --- | --- |
| `$1` BOARD_NAME | `EFR32BG21A010F1024IM32` or `EFR32BG24A010F1024IM40` |
| `$2` TASK | `all`, `gen-only`, `run-only` |
| `$3` COMPONENTS | `all`, or a single integration target |
| `$4` ADAPTER_SN | Adapter serial number |
| `$5` COMPILER | `GCC` |
| `$6` OPTIONS | Optional CMake flags (quoted) |

Supported component targets:

- `integration_test_iec60730_program_counter`
- `integration_test_iec60730_irq`
- `integration_test_iec60730_system_clock`
- `integration_test_iec60730_watchdog`
- `integration_test_iec60730_cpu_registers`
- `integration_test_iec60730_variable_memory`
- `integration_test_iec60730_invariable_memory`

Optional CMake flags for `$6`:

- `-DENABLE_CAL_CRC_32=ON`
- `-DENABLE_CRC_USE_SW=ON`
- `-DTEST_SECURE_PERIPHERALS_ENABLE=ON`
- `-DINTEGRATION_TEST_WDOG1_ENABLE=ON`
- `-DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON`

### Examples

```sh
cd test

bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all <ADAPTER_SN> GCC
bash execute_integration_test.sh EFR32BG24A010F1024IM40 all all <ADAPTER_SN> GCC

bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all <ADAPTER_SN> GCC "-DENABLE_CAL_CRC_32=ON"

bash execute_integration_test.sh EFR32BG24A010F1024IM40 all all <ADAPTER_SN> GCC \
  "-DTEST_SECURE_PERIPHERALS_ENABLE=ON -DINTEGRATION_TEST_WDOG1_ENABLE=ON -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON -DENABLE_CAL_CRC_32=ON"
```

For environment setup details, see also [Overview](./index.md).

## CRC calculation options

When building invariable-memory integration tests, CRC post-processing produces images with a `_crc16` or `_crc32` suffix. Flash the image that includes the CRC suffix.

Default CRC mode is CRC-16. Enable CRC-32 or software CRC with:

| Option | Description |
| --- | --- |
| `ENABLE_CAL_CRC_32` | Use CRC-32 instead of CRC-16 |
| `ENABLE_CRC_USE_SW` | Use software CRC instead of GPCRC hardware |
| `ENABLE_SW_CRC_TABLE` | Use a precomputed software CRC table (**requires** `ENABLE_CRC_USE_SW=ON`) |

Manual CMake example:

```sh
cd build

cmake --toolchain ../cmake/toolchain.cmake .. \
  -DENABLE_INTEGRATION_TESTING=ON \
  -DTEST_SECURE_PERIPHERALS_ENABLE=ON \
  -DINTEGRATION_TEST_WDOG1_ENABLE=ON \
  -DINTEGRATION_TEST_USE_MARCHX_DISABLE=ON \
  -DBOARD_NAME=EFR32BG21A010F1024IM32 \
  -DENABLE_CAL_CRC_32=ON
```

> [!NOTE]
> Enable `ENABLE_SW_CRC_TABLE` only when `ENABLE_CRC_USE_SW` is `ON`. Otherwise the build fails.
