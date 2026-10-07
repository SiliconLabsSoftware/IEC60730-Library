![Static Badge](https://img.shields.io/badge/Security_Support-Supported-green)
![Static Badge](https://img.shields.io/badge/Supported-Simplicity_SDK_v2026.6.0-green?style=flat-square)
[![Static Badge](https://img.shields.io/badge/Supported-GeckoSDK_v4.5.0-green)](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0)

# IEC60730 Library

Platform code for EFR32 series devices that implements IEC 60730 Class B safety requirements.

## Introduction

The IEC60730 library for EFR32 provides a baseline implementation of the diagnostic requirements in Table H.1 of the IEC 60730 specification. It includes:

- **POST (Power-On Self-Test)** — runs when the device powers on
- **BIST (Built-In Self-Test)** — runs periodically during normal operation

Some requirements depend on the end-product design. You must implement the related callback functions to meet the full specification, including:

- Safe-state handling when a fault is detected
- Communications channels (redundancy, error detection, periodic traffic)
- Plausibility checks for system state (internal variables and I/O)

## License

See [LICENSE.md](LICENSE.md).

## Release notes

See [docs/release_note.md](./docs/release_note.md).

## IEC60730 certificate

The Silicon Labs Appliances homepage will host the final certificate and detailed report when they are available.

## OEM testing

After integrating the IEC60730 library into a product, OEMs must certify the complete device with a qualified certification body.

## Supported families and software requirements

API documentation (supported families, software requirements, demo build steps, compiler notes, and system architecture) is published on the project GitHub Pages site:

- [Supported Families](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [Software Requirements](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [Building the IEC60730 Demo](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [Generate document API](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [Compiler specifications](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [System Architecture](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)

## Coding convention tool

See [docs/coding_convention_tool.md](./docs/coding_convention_tool.md).

## Dual SDK support

The IEC60730 Library SDK Extension **v2.2.0** supports:

| SDK | Version | How it is obtained |
| --- | --- | --- |
| Simplicity SDK (SSDK / SimSDK) | 2026.6.0 | Clone/setup through **SLT** via `recipe.toml` / `make bootstrap` |
| Gecko SDK (GSDK) | 4.5.0 | Maintained on GitHub — resolve via `GSDK_PATH`, Docker mount, local cache, or download from the [Gecko SDK v4.5.0 release](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0) |

> [!NOTE]
> **SLT supports Simplicity SDK (SimSDK) 2026.6.0** clone/setup (`simplicity-sdk` in `recipe.toml`).
>
> **Gecko SDK (GSDK) 4.5.0 is maintained on GitHub**, so the `gecko_4_5` profile obtains it separately through `script/set_gsdk.sh` (explicit `GSDK_PATH`, `/opt/gecko_sdk` mount, `~/.cache/iec60730/gecko-sdk/4.5.0`, or download from the GitHub release).

Select the active SDK profile before generating or building projects:

```sh
make apply-sdk-profile PROFILE=ssdk_2026_6   # Simplicity SDK 2026.6.0 (SLT)
make apply-sdk-profile PROFILE=gecko_4_5     # Gecko SDK 4.5.0 (GitHub / local path)
```

## Building with CMake and SLC

The repository includes a CMake template for building tests. Follow the steps below in order.

### 1. Add the IEC60730 extension to the SDK

See [docs/iec60730_safety_library_integration_to_sdk.md](./docs/iec60730_safety_library_integration_to_sdk.md).

### 2. Install Simplicity CLI (`slc`)

- Install Simplicity Studio and the Simplicity CLI: [Install Simplicity Studio](https://docs.silabs.com/ssv6ug/latest/install-ssv6/install-simplicity-studio).
- On Linux, install Amazon Corretto 17 if required by your `slc` package: [Amazon Corretto 17 downloads](https://docs.aws.amazon.com/corretto/latest/corretto-17-ug/downloads-list.html).

#### Configure `slc`

Add the `slc` executable directory to `PATH`:

```sh
export PATH=$PATH:<path_to_slc>
```

Point `slc` at your SDK and trust it if needed:

```sh
slc configuration --sdk <path_to_sdk>
slc signature trust --sdk <path_to_sdk>
```

Configure the GCC toolchain:

```sh
slc configuration --gcc-toolchain=<path_to_gcc_toolchain>
```

Generate a project:

```sh
slc generate <path_to_example.slcp> \
    -np \
    -d <project_destination> \
    -name=<project_name> \
    --with <supported_board_or_device>
```

| Argument | Description |
| --- | --- |
| `-cp`, `--copy-sources` | Copy all files referenced by the project, selected components, and related tools. By default, no files are copied. |
| `-cpproj`, `--copy-proj-sources` | Copy project files and link SDK sources. Can be combined with `-cpsdk`. |
| `-cpsdk`, `--copy-sdk-sources` | Copy SDK component sources and link project sources. Can be combined with `-cpproj`. |

After copying the LibIEC60730 extension into the SDK `extension` directory, trust it:

```sh
slc signature trust -extpath <path_to_extension>
```

#### Example (Simplicity SDK via SLT install path)

```sh
# Replace with your local Simplicity SDK root (from `slt where simplicity-sdk`)
SDK=$(slt where simplicity-sdk)

slc configuration --sdk "$SDK"
slc signature trust --sdk "$SDK"
slc signature trust -extpath "$SDK/extension/IEC60730_Libs"

slc generate \
    "$SDK/app/common/example/blink_baremetal" \
    -np \
    -d blinky \
    -name=blinky \
    --with EFR32BG21A010F1024IM32
```

### 3. Run tests

- Unit tests: [docs/guideline_for_running_unit_test.md](./docs/guideline_for_running_unit_test.md)
- Integration tests: [docs/guideline_for_running_integration_test.md](./docs/guideline_for_running_integration_test.md)

## Docker / Makefile bootstrap build

Reproducible compile-only builds use a thin Ubuntu image. Silicon Labs tooling (`slc-cli`, `java21`, `gcc-arm-none-eabi`, `commander`, `cmake`, and `ninja`) is installed and managed by **SLT** during `make bootstrap`. Package versions are pinned in `recipe.toml`. SLT/SLC search paths are written to `recipe.slconf` during bootstrap.

**Prerequisites:** Docker Engine 24+, Docker Compose v2, and network access to Silicon Labs package servers (and GitHub, if GSDK must be downloaded).

```sh
# Layer A — build the image (once, or after Dockerfile changes)
docker compose build

# Layer B — install / refresh Silicon Labs tooling and Simplicity SDK
make bootstrap

# Layer C — compile for BG21 (default) or BG24
make build-unit          # unit test targets (BOARD_NAME=EFR32BG21A010F1024IM32)
make BOARD_NAME=EFR32BG24A010F1024IM40 build-unit
make build-integration   # integration test targets
make build               # unit + integration
# or: make all           # bootstrap + build

make clean               # removes build/ output; preserves SDK caches
```

Optional CMake flags:

```sh
make build-unit BUILD_ARGS="-DENABLE_CAL_CRC_32=ON"
```

### SDK profile resolution

| Profile | SDK | How it is resolved |
| --- | --- | --- |
| `ssdk_2026_6` (default) | Simplicity SDK 2026.6.0 | SLT installs and locates the SDK |
| `gecko_4_5` | Gecko SDK 4.5.0 | Maintained on GitHub; `script/set_gsdk.sh` resolves it |

For Gecko SDK, either set a local install:

```sh
export GSDK_PATH=/path/to/gecko-sdk
```

or allow bootstrap/`set_env.sh` to download and cache it under:

```text
~/.cache/iec60730
```

Inside an already-running container or CI environment:

```sh
make RUNNER=native bootstrap build
```

Artifacts appear on the host under `build/` (bind-mounted workspace). Hardware flashing and on-device test execution remain host-side via `test/execute_unit_test.sh` and `test/execute_integration_test.sh`.
