![Static Badge](https://img.shields.io/badge/Security_Support-Supported-green)
![Static Badge](https://img.shields.io/badge/Supported-Simplicity_SDK_v2026.6.0-green?style=flat-square)
[![Static Badge](https://img.shields.io/badge/Supported-GeckoSDK_v4.5.0-green)](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0)

# IEC60730_Libs
Platform codes for EFR32 series chips which complies to IEC60730 safety standard

## Introduction
The IEC60730 library for EFR32 provides a basic implementation required to support the necessary requirements found in Table H.1 in the IEC60730 specification. It includes all the Power On Self Test (POST) functions executed when a device is first powered on, as well as Built In Self Test (BIST) functions that are called periodically to ensure correct operation. Certain portions of the requirements require a detailed understanding of the system under development. Callback functions must be completed by the developer to guarantee meeting the full specification. These include a Safe State function used when validation detects an anomaly, properly implemented communications channels (redundancy, error detection, periodic communications), and Plausibility functions to validate system state (internal variables and inputs/outputs).

## License

Please refer [License](LICENSE.md)

## Release Notes

Please refer document in [release_note.md](./docs/release_note.md)

## IEC60730 Certificate

The Silicon Labs Appliances homepage will contain the final certificate and detailed report when it is completed.

## OEM Testing

Once OEMs have completed integrating their system with the IEC60730 Library, they will need to certify their device with a qualified certification house.

## Supported Families

- Refer section [Supported Families](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)

## Software Requirements

- Refer section [Software Requirements](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)

## Building the IEC60730 Demo

- Refer section [Building the IEC60730 Demo](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)

## Generate document API

- Refer section [Generate document API](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)

## Coding convention tool

- Refer file: [coding_convention_tool.md](./docs/coding_convention_tool.md).

## Compiler specifications

- Refer section [Compiler specifications](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)

## System Architecture

- Refer section [System Architecture](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)

## CMake

The project has a CMake template that supports running tests. Follow the steps below one by one to build and run tests.

### Add the IEC60730 Library extension to the SDK

Refer to the IEC60730 Safety Library Integration Guide in the `docs` folder for more details.

### Install Dependencies

#### Install Simplicity CLI (slc)

- Follow the Simplicity Studio 6 User Guide to install Simplicity Studio and the Simplicity CLI (slc): [Install Simplicity Studio](https://docs.silabs.com/ssv6ug/latest/install-ssv6/install-simplicity-studio).
- Follow this guide to install Amazon Corretto 17 on Linux: [Install Amazon Corretto 17](https://docs.aws.amazon.com/corretto/latest/corretto-17-ug/downloads-list.html).

##### How to use slc

Add the path to the expanded slc executable to your PATH.

```sh
$ export PATH=$PATH:<path_to_slc>
```

Configure the SDK. For example:

```sh
$ slc configuration --sdk <path_to_sdk>
```

Run the following command if you have not yet trusted your SDK:

```sh
$ slc signature trust --sdk <path_to_sdk>
```

For example:

```sh
$ slc signature trust --sdk $SDK
```

Configure the GCC toolchain. For example:

```sh
$ slc configuration --gcc-toolchain=<path_to_gcc_toolchain>
```
Generate the project:

```sh
$ slc generate <path_to_example.slcp> \
    -np \
    -d <project_destination> \
    -name=<project_name> \
    --with <supported_board_or_device>
```

Choose one of the options below to generate the project

| Operation | Arguments | Description |
|---|---|---|
|generate | -cp, --copy-sources | Copies all files referenced by this project, selected components, and any other running tools (Pin Tool, etc.). By default, no files are copied. |
|^ | -cpproj, --copy-proj-sources | Copies all files referenced by the project and links any SDK sources. This can be combined with -cpsdk. |
|^ | -cpsdk, --copy-sdk-sources | Copies all files referenced by the selected components and links any project sources. This can be combined with -cpproj. |

> [!NOTE]
>
> The LibIEC60730 extension supports:
>
> - Gecko SDK (GSDK) 4.5.0
> - Simplicity SDK (SSDK) 2026.6.0
>
> To use the LibIEC60730 extension, copy it into the SDK `extension` directory and trust the extension:
>
> - `slc signature trust -extpath <path_to_extension>`
>
> If your workspace supports SDK profile switching, select the desired SDK profile before generating the project:
>
> - `make apply-sdk-profile PROFILE=gecko_4_5`
> - `make apply-sdk-profile PROFILE=ssdk_2026_6`


##### For example

```sh
# Configure SDK
$ SDK=/home/.silabs/slt/installs/conan/p/simpl508ee6c1a6569/p

# Trust the SDK and the IEC60730 extension
$ slc configuration --sdk $SDK
$ slc signature trust --sdk $SDK
$ slc signature trust -extpath $SDK/extension/IEC60730_Libs

# Generate a project
$ slc generate \
    $SDK/app/common/example/blink_baremetal \
    -np \
    -d blinky \
    -name=blinky \
    --with EFR32BG21A010F1024IM32
```

### Run unit test
  - Refer to the guideline link: [guideline_for_running_unit_test.md](./docs/guideline_for_running_unit_test.md)
### Run integration test
  - Refer to the guideline link: [guideline_for_running_integration_test.md](./docs/guideline_for_running_integration_test.md)

## Docker build

Reproducible compile-only builds use a thin Ubuntu image. Silicon Labs tooling (`slc-cli`, `java21`, `gcc-arm-none-eabi`, `commander`, `cmake`, and `ninja`) is installed and managed by SLT during `make bootstrap`. Package versions are pinned in `recipe.toml`; SLT/SLC search paths are maintained in the repository `recipe.slconf` file and refreshed during bootstrap.

SDK resolution depends on the selected SDK profile. Simplicity SDK profiles use the SDK installed and managed by SLT. Gecko SDK profiles automatically resolve the SDK from an explicit `GSDK_PATH`, an optional `/opt/gecko_sdk` Docker mount, the local cache under `~/.cache/iec60730/gecko-sdk/<version>`, or by downloading the configured Gecko SDK release when no local installation is available. Downloaded Gecko SDK archives and extracted SDKs are cached and automatically reused across subsequent builds.

**Prerequisites:** Docker Engine 24+, Docker Compose v2, and network access to Silicon Labs package servers.

```sh
# Layer A — build the image (once, or after Dockerfile changes)
docker compose build

# Layer B — install / refresh Silicon Labs tooling and SDKs
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

If a Gecko SDK is already installed locally, it may be used directly:

```sh
export GSDK_PATH=/path/to/gecko-sdk
```

Otherwise, the SDK is downloaded automatically during bootstrap and stored under:

```text
~/.cache/iec60730
```

Inside an already-running container or CI environment:

```sh
make RUNNER=native bootstrap build
```

Artifacts appear on the host under `build/` (bind-mounted workspace). Hardware flashing and on-device test execution remain host-side via `test/execute_unit_test.sh` and `test/execute_integration_test.sh`.
