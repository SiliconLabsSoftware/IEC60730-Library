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

- Refer section [Supported Families](./docs/html/EFR32_IEC60730_Libraries/group__efr32__iec60730.html)

## Software Requirements

- Refer section [Software Requirements](./docs/html/EFR32_IEC60730_Libraries/group__efr32__iec60730.html)

## Building the IEC60730 Demo

- Refer section [Building the IEC60730 Demo](./docs/html/EFR32_IEC60730_Libraries/group__efr32__iec60730.html)

## Generate document API

- Refer section [Generate document API](./docs/html/EFR32_IEC60730_Libraries/group__efr32__iec60730.html)

## Coding convention tool

- Refer file: [coding_convention_tool.md](./docs/coding_convention_tool.md).

## Compiler specifications

- Refer section [Compiler specifications](./docs/html/EFR32_IEC60730_Libraries/group__efr32__iec60730.html)

## System Architecture

- Refer section [System Architecture](./docs/html/EFR32_IEC60730_Libraries/group__efr32__iec60730.html)

## CMake

The project has a CMake template that supports running tests. Follow the steps below one by one to build and run tests.

### Add the IEC60730 Library extension to the SDK

- Refer to the [IEC60730 safety library integration to SDK](./docs/iec60730_safety_library_integration_to_sdk.md) in the [docs](./docs) folder for more details.

### Install Dependencies

#### Install slc-cli

- Follow this link to Install slc: [Install slc](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-tools-slc-cli/02-installation)
- Follow this link to [Install Amazon Corretto 17 on Debian-Based Linux](https://docs.aws.amazon.com/corretto/latest/corretto-17-ug/downloads-list.html)

##### How to use slc

Add the path to the expanded slc-cli to your PATH sh export PATH=$PATH:~/SimplicityStudio/slc_cli_linux/slc_cli/

Configure SDK. For example sh slc configuration --sdk ~/SimplicityStudio/SDKs/gecko_sdk/

Run slc signature trust --sdk <path_to_the_gecko_sdk> if you have not yet trusted your SDK.

For example your SDK locate at ~/SimplicityStudio/SDKs/gecko_sdk/. Run `slc signature trust --sdk ~/SimplicityStudio/SDKs/gecko_sdk/`

Set toolchain For example sh slc configuration -gcc=~/SimplicityStudio-5/SimplicityStudio_v5/developer/toolchains/gnu_arm/12.2.rel1_2023.7/

Generate the project sh slc generate \path\to\example.slcp -np -d <project_destination> -name=<new_name> --with <board_or_device_that_supports_project>

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
$ GSDK=~/SimplicityStudio/SDKs/gecko_sdk
$ slc configuration --sdk=$GSDK --gcc-toolchain=/Applications/ARM
$ slc generate $GSDK/app/common/example/blink_baremetal -np -d blinky -name=blinky -o makefile
   --with brd4166a
$ slc signature trust -extpath $GSDK/extension/IEC60730_Libs
```

### Run unit test
  - Refer to the guideline link: [guideline_for_running_unit_test.md](./docs/guideline_for_running_unit_test.md)
### Run integration test
  - Refer to the guideline link: [guideline_for_running_integration_test.md](./docs/guideline_for_running_integration_test.md)
