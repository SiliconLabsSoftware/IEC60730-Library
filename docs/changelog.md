2.2.0 Release (SDK Extension)

- Package / SDK Extension version: **2.2.0** (`iec60730.slce`). Runtime library version remains **2.0.0** (`IE60730_LIBRARY_VERSION` in `lib/inc/sl_iec60730.h`).
- Add support for EFR32BG21 and EFR32BG24 devices.
- Supported compiler: GCC.
- Add dual SDK support using IEC60730 Library SDK Extension v2.2.0 with Gecko SDK (GSDK) 4.5.0 and Simplicity SDK (SSDK) 2026.6.0.
- Add unit test and integration test support for EFR32BG21 and EFR32BG24 devices.
- Add Makefile support for switching between GSDK 4.5.0 and Simplicity SDK 2026.6.0 using SDK profiles (`gecko_4_5`, `ssdk_2026_6`).
- SLT provides clone/setup for Simplicity SDK (SimSDK) 2026.6.0. GSDK 4.5.0 is maintained on GitHub and resolved via `GSDK_PATH`, Docker mount, local cache, or download (`script/set_gsdk.sh`).

2.1.0 Release

- Add supported devices: EFR32FG23 family (confirmed device: EFR32FG23B).
- Supported compilers: GCC.
- Update the IEC60730 Library Simplicity SDK extension to version 2.0.0 to align with Simplicity SDK 2026.6.0.
- Support unit testing and integration testing for EFR32FG23.

2.0.0 Release

- Supported devices: EFR32MG families.
- Supported compilers: GCC.
- Update project from Makefile to CMake.
- Rewrite the IEC60730 library code.
- Update the GSDK extension for the IEC60730 Library to follow source-library changes.
- Support unit test and integration test.
- Support coding-convention checks.
- Support MkDocs.

1.2.0 Release

- Supported devices: EFR32xG24, EFM32xG12.
- Supported compilers: GCC, IAR.
- Added GSDK extension for the IEC60730 Library.
- Integrated demo app into the GSDK extension.

1.1.0 Release

- Supported IEC60730 standard.
- Supported devices: EFR32xG22, EFM32PG22.
- Supported compilers: GCC, IAR.

1.0.0 Release

- Supported IEC60730 standard.
- Supported devices: EFR32xG21, EFR32xG23.
- Supported compilers: GCC, IAR.
