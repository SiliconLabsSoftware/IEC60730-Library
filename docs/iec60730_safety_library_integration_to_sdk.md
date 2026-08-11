# User Guide: The Library IEC60730 Integration

This document provides instructions for integrating the Library IEC60730 into a project.

This will guide the developer to install the required software. Then guide the configuration of a project and integrate the source code into the project.

![Key Features](./images/key_feature_for_sim6.png)


![Silicon Labs](./images/silicon_lab_doc_integration_sdk.png)

SILICON LABS

## Contents

[Contents](#contents)

[Table of pictures](#table-of-pictures)

[1. Background](#1-background)

[2. Install the required software](#2-install-the-required-software)

[3. Add an extension to Simplicity Studio](#3-add-an-extension-to-simplicity-studio)

[4. Generate an example project](#4-generate-an-example-project)

[5. Edit the post-build steps](#5-edit-the-post-build-steps)

[6. Add the source code to the project](#6-add-the-source-code-to-the-project)

[7. Integrate code into the project](#7-integrate-code-into-the-project)

[8. Revision history](#8-revision-history)

## Table of pictures

[Figure 1 Adding Extension to SDK](#figure-1-adding-extension-to-sdk)

[Figure 2 Browse to extension location](#figure-2-browse-to-extension-location)

[Figure 3 Create new project](#figure-3-create-new-project)

[Figure 4 Example Project Selection](#figure-4-example-project-selection)

[Figure 5 Project Configuration](#figure-5-project-configuration)

[Figure 6 Project generation in workspace](#figure-6-project-generation-in-workspace)

[Figure 7 Build Project](#figure-7-build-project)

[Figure 8 CRC-16 and CRC-32 scripts](#figure-8-crc-16-and-crc-32-scripts)

[Figure 9 Output command from the Post-build Steps](#figure-9-output-command-from-the-post-build-steps)

[Figure 10 Result after Post-build complete](#figure-10-result-after-post-build-complete)

[Figure 11 Components support library IEC60730](#figure-11-components-support-library-iec60730)

[Figure 12 Add source code library IEC60730](#figure-12-add-source-code-library-iec60730)

[Figure 13 Assembly code algorithm MARCHC for GCC compiler](#figure-13-assembly-code-algorithm-marchc-for-gcc-compiler)

[Figure 14 Flow chart of the library IEC60730](#figure-14-flow-chart-of-the-library-iec60730)

[Figure 15 Demo OEM files integrated with Library IEC60730](#figure-15-demo-oem-files-integrated-with-library-iec60730)

[Figure 16 Configuration for watchdog module](#figure-16-configuration-for-watchdog-module)

[Figure 17 Configuration for system clock module](#figure-17-configuration-for-system-clock-module)

## 1. Background

The IEC60730 is a safety standard used in household applications. It defines the test and diagnostic method that ensures the safe operation of devices. We provide the test of the following components: CPU registers, variable memory check, invariable memory check, program counter check, clock check, and interrupt check.

At the time of this writing, the library IEC60730 has been tested on two devices EFR32xG23 and EFR32xG24 on Simplicity Studio 6 (SS6) with toolchain GNU ARM v12.2.1 and SDK version 2026.6.0.

## 2. Install the required software.

We use the third-party software [SRecord](http://srecord.sourceforge.net/) to calculate CRC value. Firstly, you need to install this software. If you're using Windows OS, you can go to the link of this software (link above), download the installation, and run the installer to install. If you're using Ubuntu OS, please follow the installation instructions below.

```sh
$ sudo apt update
$ sudo apt install srecord
```

## 3. Add an extension to Simplicity Studio

The safety library IEC60730 is supported by adding the IEC60730 extension, which is built using the software environment.:

- OS-Ubuntu 20.04

- Simplicity Studio 6

- Simplicity SDK Suite v2026.6.0

This project is organized as an extension of Simplicity Studio. This project is built upon SSDK version 2026.6.0, GNU toolchain V12.2.1. The user can download the same version of SSDK from `PACKAGES > PACKAGES MANAGER > Simplicity SDKs > Add new SDK > Simplicity SDK 2026.6.0` and Simplicity Studio V6 download link [Simplicity Studio V6](https://www.silabs.com/software-and-tools/simplicity-studio?tab=getting-started).

To create and build demo projects, the user must add the IEC60730 extension to Simplicity Studio. The procedure would be `SETTING > SDKs > Simplicity SDK Suite v2026.6.0 > Add Extension`.

![Figure 1 Adding Extension to SDK](./images/figure_1_adding_extension_to_sdk.png)
###### Figure 1 Adding Extension to SDK


Press `Browse` to find the directory of this extension. Then choose the folder that has the file name `iec60730.slce`. Simplicity Studio will detect SDK extensions automatically. Click `OK` then `Apply and Close`

![ Figure 2 Browse to extension location](./images/figure_2_brows_to_extension_location.png)
###### Figure 2 Browse to extension location

## 4. Generate an example project.

Before using Simplicity Studio to generate the project, you need to add the IEC60730 extension. Please remember the following text:
> `"This extension supports a demo example for EFR32MG families"`

Start a project, select `DEVICES` and choose `Devices you can connect`. For example, you can select `EFR32FG23 2.4GHz 20 dBm Radio Board (Rev A00)` in the Target Boards section, with the Target Device set to `EFR32FG23B020F512IM48` as shown in the image below.

![Figure 3 Create new project](./images/figure_3_create_new_project.png)
######  Figure 3 Create new project

To create a new Simplicity Studio® 6 (SSv6) project, follow these three dialog steps:

- Target, SDK, and Toolchain

- Examples

- Configuration

An indicator at the top of the dialog will show you your current position in the process. You can click `Back` at any time to return to a previous dialog if you need to make changes.

In `Example Project Selection`, use the checkboxes or keywords to find the example of interest. To create a radio board example IEC60730 Demo, search the keyword `iec60730` in the search box, related examples will show. Choose `IEC60730 Example Demo`. Click `Create`

![Figure 4 Example Project Selection](./images/figure_4_example_project_selection.png)
###### Figure 4 Example Project Selection

In `Project Configuration Selection`, rename and location your project if you want. For the three selections under `Copy contents`, you can choose any of the selections you want.

![Figure 5 Project Configuration](./images/figure_5_project_configuration.png)
###### Figure 5 Project Configuration

Once you finish project creation, the Simplicity IDE perspective opens. There may be a slight delay in the initial configuration.

The project typically opens `README tab`, which contains an example project description, and `OVERVIEW tab`.

![Figure 6 Project generation in workspace](./images/figure_6_project_generation_in_workspace.png)
###### Figure 6 Project generation in workspace

For building the project, select `Open in VS Code` to synchronize the project with Visual Studio Code. Then, open the Extensions view in VS Code and install the `Simplicity Studio for VS Code` extension. After installation, click the hammer icon to build the project, or right-click the project and select Build Project.

![Figure 7 Build Project](./images/figure_7_build_project.png)
###### Figure 7 Build Project


## 5. Edit the post-build steps.

By default, after building the project, firmware files in `*.bin`, `*.hex`, and `*.s37` formats will be created.

Modify the post-build steps so new firmware images are generated with a CRC value written into FLASH. Scripts `sl_iec60730_cal_crc16.sh` and `sl_iec60730_cal_crc32.sh` produce `*_crc16` / `*_crc32` images (documented generically as `*_crcNN`). They run on Windows and Ubuntu and live under `iec60730_<version>/lib/crc/`.

![Figure 8 CRC-16 and CRC-32 scripts](./images/figure_8_crc_16_and_crc_32_scripts.png)
###### Figure 8 CRC-16 and CRC-32 scripts

Detailed parameters are described in **Support calculate CRC**.

### Studio-style example (GCC)

```bash
arm-none-eabi-objdump -t -h -d -S '${BuildArtifactFileBaseName}.axf' >'${BuildArtifactFileBaseName}.lst' && bash ${ProjDirPath}/iec60730_1.1.0/lib/crc/sl_iec60730_cal_crc16.sh ${BuildArtifactFileBaseName} "<path_build_dir>" "<path_srecord_bin>" GCC "0x8000000"
```

![Figure 9 Output command from the Post-build Steps](./images/figure_9_add_the_command_to_post_build_steps.png)
###### Figure 9 Output command from the Post-build Steps

### Add post-build CRC (CMake)

Apply CRC post-build in `cmake_gcc/CMakeLists.txt`. Append to the existing `POST_BUILD` (after `.s37` / `.hex` / `.bin`). Do **not** wrap the CRC invocation in `bash -c "..."` — nested quotes break under Windows `cmd.exe`.

```cmake
add_custom_command(TARGET iec60730_demo_cpp
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_SREC_CMD} "$<TARGET_FILE:iec60730_demo_cpp>" "$<TARGET_FILE_DIR:iec60730_demo_cpp>/$<TARGET_FILE_BASE_NAME:iec60730_demo_cpp>.s37"
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_IHEX_CMD} "$<TARGET_FILE:iec60730_demo_cpp>" "$<TARGET_FILE_DIR:iec60730_demo_cpp>/$<TARGET_FILE_BASE_NAME:iec60730_demo_cpp>.hex"
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_BIN_CMD}  "$<TARGET_FILE:iec60730_demo_cpp>" "$<TARGET_FILE_DIR:iec60730_demo_cpp>/$<TARGET_FILE_BASE_NAME:iec60730_demo_cpp>.bin"
    # .lst + CRC-16 (*_crc16.bin/hex/s37). Use bash on Windows (MSYS/MinGW/Cygwin).
    COMMAND ${CMAKE_OBJDUMP} -t -h -d -S "$<TARGET_FILE:iec60730_demo_cpp>" > "$<TARGET_FILE_DIR:iec60730_demo_cpp>/$<TARGET_FILE_BASE_NAME:iec60730_demo_cpp>.lst"
    COMMAND bash "${CMAKE_CURRENT_LIST_DIR}/../iec60730_2.0.0/lib/crc/sl_iec60730_cal_crc16.sh"
            "$<TARGET_FILE_BASE_NAME:iec60730_demo_cpp>"
            "$<TARGET_FILE_DIR:iec60730_demo_cpp>"
            "C:/Program Files/srecord/bin"
            GCC
            "0x8000000"
)
```

Use `sl_iec60730_cal_crc32.sh` for CRC-32. On Linux, pass `""` for the srecord path field `"<path_srecord_bin>"`. On Windows, use MSYS/Git Bash. Flash `*_crcNN.*`, not the plain image.

> **Note**
>- In the default configuration, `SL_IEC60730_CRC_DEBUG_ENABLE` (debugging the CRC invariable-memory test on the demo) is enabled. To produce a CRC-calculated image for testing, run `sl_iec60730_cal_crc16.sh` or `sl_iec60730_cal_crc32.sh` from `lib/crc/` and flash the matching `*_crc16` or `*_crc32` image.
>- If, during project configuration, you choose **Link to SDK and Copy Project Structures** or **Link to Source**, copy `sl_iec60730_cal_crc16.sh` or `sl_iec60730_cal_crc32.sh` into the `lib/crc` folder of your project directory (or point the CMake path at `iec60730_<version>/lib/crc/`).
>- After the build of a project is complete, it will call the command in the Post-build steps to create <project_name>_crc16 or <project_name >_crc32 files with the extension *.bin, *.hex, and *.s37.
---

![Figure 10 Result after Post-build complete](./images/figure_10_result_after_post_build_complete.png)
###### Figure 10 Result after Post-build complete

## 6. Add the source code to the project.

In our example, after adding the SDK extension, the software component will have a few components that support adding code files (*.c, *. s) of Library IEC60730 to the project:

![Figure 11 Components support library IEC60730](./images/figure_11_components_support_library_iec60730.png)
###### Figure 11 Components support library IEC60730

When you install these components, the source code library IEC60730 will be added. For example:

![Figure 12 Add source code library IEC60730](./images/figure_12_add_source_code_library_iec60730.png)
######  Figure 12 Add source code library IEC60730

![Figure 13 Assembly code algorithm MARCHC for GCC compiler](./images/figure_13_assembly_code_alorithm_marchc_for_gcc_compiler.png)
###### Figure 13 Assembly code algorithm MARCHC for GCC compiler

## 7. Integrate code into the project.

The library IEC60730 has been divided into 2 main test phases: Power on Self-Test (POST) and Build In Self-Test (BIST). [Figure 13 Flow chart of the library IEC60730](#figure-13-flow-chart-of-the-library-iec60730) shows the basics of the library IEC60730 integration into a user software solution.

![Figure 14 Flow chart of the library IEC60730](./images/figure_14_flow_chart_of_the_lib_iec60730.png)
###### Figure 14 Flow chart of the library IEC60730

In our example, we have added a demo `oem` foler  (Original equipment manufacturer) to integrate with the library IEC60730 to test steps such as flow charts fully.

![Figure 15 Demo OEM files integrated with Library IEC60730](./images/figure_15_demo_oem_files_intergrade_with_lib_iec.png)
###### Figure 15 Demo OEM files integrated with Library IEC60730

If you want to add your OEM code and don't want to use our OEM files demo, you COULD add your OEM code via the following steps below:

### 1. Requires declaration and initialize variables for IEC60730 library with constant values. Refer function `oem_iec60730_init` in file `oem_iec60730.c`.

```sh
sl_iec60730_vmc_test_multiple_regions_t oem_vmc_test;
sl_iec60730_imc_test_multiple_regions_t oem_imc_test __CLASSB_RAM;
sl_iec60730_irq_cfg_t oem_irq_config;
```


These three variables are used for interrupt, invariable memory check and variable memory check.

### 2. To perform an invariable memory check, the library uses CRC (Cyclic Redundancy Check) calculations for Flash memory. If the user calculates using hardware, the user will need to initialize the GPCRC module to support the calculation. Users can also enable `#define SL_IEC60730_CRC_USE_SW_ENABLE` for software calculation. To calculate the CRC value, It will have two options:

To perform an invariable memory check, the IEC60730 library uses CRC (Cyclic Redundancy Check) calculations for Flash memory.

CRC calculation can be performed using either the GPCRC hardware peripheral or a software implementation by enabling:

```c
#define SL_IEC60730_CRC_USE_SW_ENABLE
```

If hardware CRC calculation is used, the GPCRC module must be initialized before running the Flash memory test.

The library supports two CRC calculation methods:

- Calculate CRC from a user-defined Flash start address to the end of the application image.
- Calculate CRC for multiple Flash regions by specifying the start and end addresses of each region.

For more details, refer to:

```text
oem_iec60730.c
```

The calculated CRC value is stored at the end of the application image and is used by the IEC60730 Flash memory verification routine during runtime.

For GCC-based projects, the CRC value is automatically generated during the post-build process using the provided CRC calculation scripts or the Silicon Labs IEC60730 CRC Tool described in Section 5.

### 3. Configure Watchdog Test: this configuration determines which watchdog unit will be checked.The library does not initialize the watchdog units, the user should do the initialization. We support configuration for watchdog module

![Figure 16 Configuration for watchdog module](./images/figure_16_configuration_for_watchdog_module.png)
###### Figure 16 Configuration for watchdog module

The two define used to enable watchdog testing are used in the application:

```C
#define SL_IEC60730_WDOG0_ENABLE
#define SL_IEC60730_WDOG1_ENABL
```

If these macros are not enabled, it will show an error saying watchdog checking is not enabled.

- To clear reset cause flags in the RSTCASUES register after watchdog testing is completed. Enable configuration of the definition of macro `#define SL_IEC60730_RSTCAUSES_CLEAR_ENABLE`. In our demo, this feature is enabled.

- The static variable `iec60730_watchdog_count` must be located at a memory location that is not cleared when system startup `(section".ram_no_clear")`.

- The global variable `iec60730_watchdog_state` must be located at a memory location that is not cleared when system startup `(section ".ram_no_clear")`. To enable saving `iec60730_watchdog_state` to backup RAM on Series 2, enable the macro `#define SL_IEC60730_SAVE_STAGE_ENABLE`. By default, it will be disabled.Define macro `SL_IEC60730_BURAM_IDX` to select which register of the BURAM will be used. The default value is `0x0UL`.

### 4. Before calling the `sl_iec60730_post` function,we need to do the following steps:

- Configure the clock for the timers. You can refer to these configurations in our demo examples, file `oem_iec60730_init_device.c`.

- Create two timers with 10 milliseconds (ms) and 100 milliseconds (ms) interrupt periods (parameters 10ms and 100ms are recommended values) to test the clock and the clock switch. You can refer our demo example, file `oem_iec60730_timer.c` for more details. Note that adjusting the 10ms and 100ms values will require adjusting other configuration `IEC60730_SYS_CLK`:

![Figure 17 Configuration for system clock module](./images/figure_16_configuration_for_system_clock_module.png)
###### Figure 17 Configuration for system clock module

### 5. To perform a variable memory check, the library uses MarchC and MarchXC algorithms. It will have two options:

   - Start from the user's specified address in RAM and continue to the end of the RAM region.

   - Calculate multiple RAM regions by providing the starting and ending addresses for each one. For additional details, refer to the file `oem_iec60730.c`.

### 6. Before calling the `sl_iec60730_bist` function, we `SHOULD` set the flag for the `sl_iec60730_program_counter_check` variable. Some of the following flags are set by the Library IEC60730: `IEC60730_VMC_COMPLETE,IEC60730_IMC_COMPLETE,IEC60730_CPU_CLOCKS_COM PLETE,and IEC60730_INTERRUPT_COMPLETE`.

Other flags (IEC60730_GPIO_COMPLETE, IEC60730_ANALOG_COMPLETE,etc.) are up to you to develop additional test functions. The `sl_iec60730_program_counter_check` variable `SHOULD` set the flags corresponding to the unavailable test to ensure that the Program Counter Check is guaranteed.

In the demo examples,you will often see the following code.

```C
sl_iec60730_program_counter_check   |=IEC60730_GPIO_COMPLETE
                                    | IEC60730_ANALOG_COMPLETE
                                    | IEC60730_OEM0_COMPLETE
                                    | IEC60730_OEM1_COMPLETE
                                    | IEC60730_OEM2_COMPLETE
                                    | IEC60730_OEM3_COMPLETE
                                    | IEC60730_OEM4_COMPLETE
                                    | IEC60730_OEM5_COMPLETE
                                    | IEC60730_OEM6_COMPLETE
                                    | IEC60730_OEM7_COMPLETE;
```

- On demo eample, executes the external communication test that sets the `IEC60730_COMMS_COMPLETE` flag by itself.

- The function `sl_iec60730_bist` SHOULD be called in periodical task or a supper loop while(1).

### 7.  Remember to increment the IRQ counter variable every time the interrupt to test occurs. You can refer to the `oem_irq_exec_count_tick` function in our demo examples.


```C
void TIMERO_IRQHandler(void) {

...

oem_irq_exec_count_tick();

}

void oem_irq_exec_count_tick(void){

oem_irq_exec_count[0]++;

}

```

### 8. Create the function `sl_iec60730_safe_state`. The purpose of this function is to handle when an error occurs. An example of handling of this function refer files `oem_iec60730_functions.c`. After generating our demo example successfully, you also `COULD` add your code in file app.c and app.h

## 8. Revision history

| Revision | Date | Description |
| -- | -- | -- |
| 0.1.0 | Oct 2021 | Initial Revision |
| 0.2.0 | Nov 2021 | Section 7: Added description for Watchdog |
| 0.3.0 | Mar 2023 | Remove Section 8. |
| 0.4.0 | Apr 2023 | Update document |
| 1.0.0 | Sep 2023 | Section 1: Added mention aboutEFM32PG22 and EFR32xG22devices. |
| 1.1.0 | June 2024 | Adding Section 3 and Section 4 for support creates a Library Extension Updated other sections for suit with the released package EFR32xG12 and EFR32xG24 devices. |
| 2.0.0 | Nov 2024 | Rewrite the documentation by the re-factory code of the library support device EFR32MG families. |
| 2.1.0 | Aug 2026 | Update the documentation to reflect the extension configuration changes that add support for the EFR32FG23 device family. |




