![Static Badge](https://img.shields.io/badge/Security_Support-Supported-green)
![Static Badge](https://img.shields.io/badge/SDK-Simplicity_SDK_v2026.6.0-green?style=flat-square)

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

> [!NOTE]: To be able to use the extension LibIEC60730. You need to add the LibIEC60730
> extension to your SDK in the extension folder and run the command: `slc signature trust -extpath <path_to_your_extension_sdk>`

##### For example

```sh
$ SDK=/home/svc_sqa_automation/.silabs/slt/installs/conan/p/simpl508ee6c1a6569/p
$ slc configuration --sdk=$SDK
$ slc signature trust --sdk $SDK
$ slc signature trust -extpath $SDK/extension/IEC60730_Libs
$ slc generate \
    $SDK/app/common/example/blink_baremetal \
    -np \
    -d blinky \
    -name=blinky \
    --with brd4264c
```

### Run unit test
  - Refer to the guideline link: [guideline_for_running_unit_test.md](./docs/guideline_for_running_unit_test.md)
### Run integration test
  - Refer to the guideline link: [guideline_for_running_integration_test.md](./docs/guideline_for_running_integration_test.md)

## Docker build (Simplicity SDK via SLT)

Reproducible compile-only builds use a thin Ubuntu image. Silicon Labs SDKs and toolchains are **not** baked into the image; they are installed by `make bootstrap` into `/root/.silabs` (Compose volume `silabs-root`). Package pins live in `recipe.toml`; SLT/SLC paths live in root `recipe.slconf` (refreshed by bootstrap).

**Prerequisites:** Docker Engine 24+, Docker Compose v2, network access to Silabs package servers.

```sh
# Layer A — build the image (once, or after Dockerfile changes)
docker compose build

# Layer B — install SLT + simplicity-sdk 2026.6.0 from recipe.toml (once)
make bootstrap

# Layer C — compile for brd4264c
make build-unit          # unit test targets
make build-integration   # integration test targets
make build               # unit + integration
# or: make all           # bootstrap + build

make clean               # removes build/ output; keeps the silabs-root volume
```

Optional CMake flags:

```sh
make build-unit BUILD_ARGS="-DENABLE_CAL_CRC_32=ON"
```

Inside an already-running container or CI (`docker run -v "$PWD":/workspace ...`):

```sh
make RUNNER=native bootstrap build
```

Artifacts appear on the host under `build/` (bind-mounted workspace). Hardware flash and on-device test execution stay on the host via `test/execute_unit_test.sh` / `test/execute_integration_test.sh`.
