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

Default Docker / CI / Makefile board names:

| Device ID | Role |
| --- | --- |
| `EFR32BG21A010F1024IM32` | Default (`BOARD_NAME`) |
| `EFR32BG24A010F1024IM40` | Alternate CI matrix board |

SDK pins: Simplicity SDK **2026.6.0** (`recipe.toml`) and Gecko SDK **4.5.0**. Full family / API docs:

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

### Select an SDK profile

The repository ships with **`ssdk_2026_6` as the committed default**. Apply a profile before generating or building:

```sh
make apply-sdk-profile PROFILE=ssdk_2026_6   # Simplicity SDK 2026.6.0 (SLT) — default
make apply-sdk-profile PROFILE=gecko_4_5     # Gecko SDK 4.5.0 (GitHub / local path)
```

Recommended order:

```sh
make apply-sdk-profile PROFILE=<ssdk_2026_6|gecko_4_5>
make bootstrap          # or: source script/set_env.sh after a prior bootstrap
make build-unit         # or build-integration / build
```

> [!IMPORTANT]
> **`make apply-sdk-profile` overwrites tracked project files** (extension metadata, SLCPs, demo/`main` sources, `sdk.env`) from snapshots under `sdk_profiles/<profile>/`. Those `*.patch` files are **full-file snapshots**, not git diffs.
>
> - Expect `git status` to show modified files after switching (especially to `gecko_4_5`).
> - The target also removes generated dirs: `build/`, `autogen/`, `src/`, and `*.slconf`.
> - **Do not commit** those modifications unless you intentionally change the default profile.
> - Before committing other work, restore the default:
>   ```sh
>   make apply-sdk-profile PROFILE=ssdk_2026_6
>   ```
> - CI applies the profile in a clean job checkout; local clones should treat profile switches as a temporary workspace state.

## Building with CMake and SLC

The repository includes a CMake template for building tests. Follow the steps below in order.

### 1. Add the IEC60730 extension to the SDK

See [docs/iec60730_safety_library_integration_to_sdk.md](./docs/iec60730_safety_library_integration_to_sdk.md).

### 2. Install Simplicity CLI (`slc`)

- Install Simplicity Studio and the Simplicity CLI: [Install Simplicity Studio](https://docs.silabs.com/ssv6ug/latest/install-ssv6/install-simplicity-studio).
- **Java:** Docker / SLT bootstrap provides **Java 21** (`java21` via SLT; image also installs `openjdk-21-jre-headless`). For Simplicity Studio–only workflows, install the JDK your Studio/`slc` package requires (often Amazon Corretto 17 on Linux): [Amazon Corretto 17 downloads](https://docs.aws.amazon.com/corretto/latest/corretto-17-ug/downloads-list.html).

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

Reproducible compile-only builds use a thin Ubuntu image. Silicon Labs tooling (`slc-cli`, `java21`, `gcc-arm-none-eabi`, `commander`, `cmake`, and `ninja`) is installed and managed by **SLT** during `make bootstrap`. Package pins live in `recipe.toml`. After bootstrap, SLT/SLC search paths are written to gitignored `recipe.slconf` at the repo root (not present in a clean clone).

**Supported host OS:** Documented Docker builds are validated on **Linux** (Ubuntu 24.04 recommended; used by CI). Windows and macOS can use Docker Desktop with the same Compose flow. Native host flash/run scripts are validated on Linux.

**Prerequisites:**

- Docker Engine 24+
- **Docker Compose v2** (the `docker compose` CLI plugin — required by the root `Makefile`; Docker Engine alone is not enough)
- Network access to Silicon Labs package servers (and GitHub, if GSDK must be downloaded)

`make bootstrap` / `make build` pre-create writable host dirs `~/.silabs` and `~/.cache/iec60730` (avoid a root-owned bind mount). If a previous failed run left `~/.silabs` owned by root, fix once with `sudo chown -R "$USER" ~/.silabs`.

Verify Compose v2 before the first bootstrap:

```sh
docker compose version
```

If that fails (for example `compose is not a docker command`, or `unknown shorthand flag: 'f' in -f` when running `make bootstrap` / `docker compose -f ...`), install the plugin, then re-check:

```sh
# Ubuntu / Debian (package name may vary by distro)
sudo apt-get update
sudo apt-get install docker-compose-v2
# or, from Docker's apt repository: docker-compose-plugin

docker compose version
```

CI reference: [`.github/workflows/02-Build-Firmware.yaml`](./.github/workflows/02-Build-Firmware.yaml).

```sh
# Layer A — build the image (once, or after Dockerfile changes)
docker compose build

# Layer B — select SDK profile (optional; clone default is ssdk_2026_6)
# make apply-sdk-profile PROFILE=ssdk_2026_6
# make apply-sdk-profile PROFILE=gecko_4_5

# Layer C — install / refresh Silicon Labs tooling and the selected SDK
make bootstrap

# Layer D — compile for BG21 (default) or BG24
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

After `make apply-sdk-profile`, `sdk.env` records `SDK_PROFILE=...`. Bootstrap / `script/set_env.sh` uses that to resolve `SDK_PATH` for the active profile. See [Select an SDK profile](#select-an-sdk-profile) for the overwrite / restore rules.

For Simplicity SDK, an explicit host install can be passed through Compose as `SDK_PATH` (preserved by `script/set_env.sh`). For Gecko SDK, either set a local install:

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

### Build artifacts

After `make build-unit` / `make build-integration`, firmware images land under the bind-mounted `build/` tree. Example (BG21, GCC):

```text
build/test/unit_test/build/EFR32BG21A010F1024IM32/GCC/unit_test_iec60730_post/unit_test_iec60730_post.s37
build/test/integration_test/build/EFR32BG21A010F1024IM32/GCC/integration_test_iec60730_irq/NS/integration_test_iec60730_irq.s37
```

Invariable-memory images may end with `_crc16.s37` or `_crc32.s37` — flash the CRC-suffixed file when those options are enabled. See the unit/integration guidelines for details.

### On-device test prerequisites (host)

Compile can stay in Docker; flash and on-device execution run on the **host**:

- Supported kit / device: `EFR32BG21A010F1024IM32` (default) or `EFR32BG24A010F1024IM40`
- SEGGER J-Link (library path used by scripts: `/opt/SEGGER/JLink/libjlinkarm.so`)
- Simplicity Commander (`commander`) on `PATH`
- Adapter serial number (`ADAPTER_SN`) from the debugger

### Flash and run on-device tests

```sh
cd test
bash execute_unit_test.sh EFR32BG21A010F1024IM32 all all <ADAPTER_SN> GCC
# or
bash execute_integration_test.sh EFR32BG21A010F1024IM32 all all <ADAPTER_SN> GCC
```

Scripts resolve paths from their own location, so `bash test/execute_unit_test.sh ...` from the repo root also works. Prefer `cd test` to match the guidelines.

**Success:** script exits `0` and writes reports under `log/` (for example `log/unit_test_iec60730_post.log`). Full options and CRC flags: [unit test guideline](./docs/guideline_for_running_unit_test.md), [integration test guideline](./docs/guideline_for_running_integration_test.md).

## Troubleshooting

| Symptom | What to try |
| --- | --- |
| Missing `slc` / SLT packages | Run `make bootstrap` before `make build-*`. |
| `slc generate` fails with a mysterious path error | Prefer SLT’s `slc-cli`, not Heimdal’s `/usr/bin/slc`. `source script/set_env.sh` after bootstrap. |
| Unexpected `git status` changes after profile switch | Expected: `apply-sdk-profile` overwrites tracked files. Restore with `make apply-sdk-profile PROFILE=ssdk_2026_6` before commit. |
| Build fails after switching SDK | Re-run `make bootstrap` (or `source script/set_env.sh`) for the new profile, then rebuild. |
| Wrong SDK / `SDK_PATH` | Check `sdk.env` (`SDK_PROFILE`), then `source script/set_env.sh`. For GSDK set `GSDK_PATH` or allow cache/download. |
| `RUNNER=compose` inside a container | Use `make RUNNER=native …` when already inside the image or CI. |
| Flash fails / no device | Ensure `commander` is on `PATH`, J-Link is installed, and `ADAPTER_SN` is correct. |
| Permission errors under `~/.silabs` | Pre-create as your user, or `sudo chown -R "$USER" ~/.silabs` after a root-owned bind mount. |
| Wrong `.s37` / missing CRC image | Use the artifact paths above; for invariable memory flash `*_crc16.s37` or `*_crc32.s37`. |
| BG24 CRC / flash address wrong | Prefer `make BOARD_NAME=EFR32BG24A010F1024IM40 …` (sets `FLASH_REGIONS_TEST`). If invoking CMake directly, export `FLASH_REGIONS_TEST=0x08000000` for BG24. |
