# Guideline using IEC60730 Safety Library

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

See [License](./license.md).

## Release notes

See [release_note.md](./release_note.md).

## Versioning

| Artifact | Version | Where |
| --- | --- | --- |
| SDK Extension (package) | **2.2.0** | `iec60730.slce` |
| Runtime library | **2.0.0** | `IE60730_LIBRARY_VERSION` / `SL_IEC60730_LIBRARY_VERSION` in `lib/inc/sl_iec60730.h` |

This release updates the **SDK Extension** to 2.2.0 (BG21/BG24, dual-SDK profiles, tests). The library API/runtime version stays at **2.0.0**.

## IEC60730 certificate

The Silicon Labs Appliances homepage will host the final certificate and detailed report when they are available.

## OEM testing

After integrating the IEC60730 library into a product, OEMs must certify the complete device with a qualified certification body.

## Supported families and software requirements

API documentation (supported families, software requirements, demo build steps, compiler notes, and system architecture) is published with the generated Doxygen site on GitHub Pages:

- [Supported Families](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [Software Requirements](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [Building the IEC60730 Demo](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [Generate document API](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [Compiler specifications](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)
- [System Architecture](https://github.com/SiliconLabsSoftware/IEC60730-Library/blob/gh-pages/docs/document_api_iec60730_library/group__efr32__iec60730.html)

## Coding convention tool

See [coding_convention_tool.md](./coding_convention_tool.md).

## Dual SDK support

The IEC60730 Library SDK Extension **v2.2.0** supports:

| SDK | Version | How it is obtained |
| --- | --- | --- |
| Simplicity SDK (SSDK / SimSDK) | 2026.6.0 | Clone/setup through **SLT** via `recipe.toml` / `make bootstrap` |
| Gecko SDK (GSDK) | 4.5.0 | Maintained on GitHub — resolve via `GSDK_PATH`, Docker mount, local cache, or download |

> [!NOTE]
> **SLT supports Simplicity SDK (SimSDK) 2026.6.0** clone/setup.
>
> **Gecko SDK (GSDK) 4.5.0 is maintained on GitHub**, so the `gecko_4_5` profile obtains it separately through `script/set_gsdk.sh`.

The repository ships with **`ssdk_2026_6` as the committed default**. Apply a profile before generating or building:

```sh
make apply-sdk-profile PROFILE=ssdk_2026_6   # default
make apply-sdk-profile PROFILE=gecko_4_5
```

Recommended order: `apply-sdk-profile` → `bootstrap` / `source script/set_env.sh` → `build-*`.

> [!IMPORTANT]
> **`make apply-sdk-profile` overwrites tracked project files** from snapshots under `sdk_profiles/<profile>/` (the `*.patch` files are full-file snapshots, not git diffs). It also removes `build/`, `autogen/`, `src/`, and `*.slconf`.
>
> - Expect a dirty `git status` after switching (especially to `gecko_4_5`).
> - **Do not commit** those changes unless you intentionally change the default profile.
> - Restore before commit: `make apply-sdk-profile PROFILE=ssdk_2026_6`.
>
> Full workflow notes: [README — Dual SDK support](../README.md#dual-sdk-support).

## CMake and SLC setup

The project includes a CMake template for building and running tests.

### Add the IEC60730 Library extension to the SDK

See [IEC60730 safety library integration to SDK](./iec60730_safety_library_integration_to_sdk.md).

### Install dependencies

#### Install Simplicity CLI (`slc`)

- Follow the Simplicity Studio 6 User Guide: [Install Simplicity Studio](https://docs.silabs.com/ssv6ug/latest/install-ssv6/install-simplicity-studio).
- **Java:** Docker / SLT bootstrap uses **Java 21**. For Simplicity Studio–only workflows, install the JDK your Studio/`slc` package requires (often Amazon Corretto 17 on Linux): [Amazon Corretto 17 downloads](https://docs.aws.amazon.com/corretto/latest/corretto-17-ug/downloads-list.html).

#### Configure `slc`

Add `slc` to your `PATH`:

```sh
export PATH=$PATH:<path_to_slc>
```

Configure and trust the SDK:

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

Copy the LibIEC60730 extension into the SDK `extension` directory, then trust it:

```sh
slc signature trust -extpath <path_to_extension>
```

#### Example

```sh
# Prefer resolving the SLT-managed Simplicity SDK dynamically
SDK=$(slt where simplicity-sdk)

slc configuration --sdk "$SDK"
slc signature trust --sdk "$SDK"
slc signature trust -extpath "$SDK/extension/IEC60730_Libs"

slc generate \
    <path_to_project>.slcp \
    -np \
    -d iec60730_demo \
    -name=iec60730_demo \
    --with EFR32BG21A010F1024IM32
```

### Run tests

- [Guideline for running unit tests](./guideline_for_running_unit_test.md)
- [Guideline for running integration tests](./guideline_for_running_integration_test.md)
