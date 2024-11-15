/***************************************************************************/ /**
 * @file
 * @brief Library Documentation
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

// This file is a centralized location for documentation on OEM functions.
// It contains no source.

/**************************************************************************/ /**
 * @defgroup efr32_iec60730 EFR32 IEC60730 Library
 * @brief Overview and documentation of IEC60730 library APIs
 * @{
 * @section introduction Introduction
 * The IEC60730 library for EFR32 provides a basic implementation required to
 * Support the requirements found in Table H.1 in the <a href="https://webstore.iec.ch/publication/23952">IEC60730
 * specification</a>. It includes all the Power On Self Test (POST) functions
 * executed when a device is first powered on, as well as Built In Self Test
 * (BIST) functions that are called periodically to ensure correct operation.
 * Certain portions of the requirements require a detailed understanding of
 * The system is under development.  Callback functions must be completed by
 * the developer to guarantee meeting the full specification. These include a
 * Safe State function used when validation detects an anomaly, properly
 * implemented communications channels (redundancy, error detection, periodic
 * communications), and Plausibility functions to validate system state (internal
 * variables and inputs/outputs).
 *
 * @section license License
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
 * Source code in this repo is covered by one of several different licenses. The default license is the Master Software License Agreement (MSLA) <a href="https://www.silabs.com/about-us/legal/master-software-license-agreement">MSLA</a>, which applies unless otherwise noted.
 *
 * During the unit test build process, some third-party code will be added to this repository and separated into another license. An example can be found in the build/_deps directory, where the Unity library uses the MIT license.
 *
 * @section certificate IEC60730 Certificate
 * The final certificate and detailed report shall be provided for the specific
 * devices.
 *
 * @section oem_testing OEM Testing
 *
 * Once OEMs have completed integrating their system with the IEC60730 Library,
 * they will need to certify their device with a qualified certification house.
 *
 * \subsection families_sec Supported Families
 *
 * This library supports all **EFR32MG** devices listed in the
 * <a href="https://www.silabs.com/wireless">Selector Guide</a>.
 *
 * @section software_requirements Software Requirements
 * The IEC60730 library dependencies:
 * -# CMSIS package.
 * -# Silicon Labs Devices header files.
 *
 * Users could get dependency source files from the GSDK suite (platform).
 * To get the latest version of GSDK, please refer to
 * <a href="https://github.com/SiliconLabs/gecko_sdk"> Gecko SDK.</a>
 *
 * @note The IEC60730 library support extension for GSDK 4.4.2 using
 * <a href="https://www.silabs.com/developers/simplicity-studio"> Simplicity Studio 5.</a>
 *
 * Details on the validation test setup used internally by Silicon Labs can
 * be found at \link IEC60730_VERIFICATION IEC60730 Test Specification\endlink.
 * Test results can be found inside each module.
 *
 * Currently tested on Windows and Linux platforms.
 *
 * @subsection installation_steps Building the IEC60730 Demo
 *
 * To use Simplicity Studio to generate and build a demo IEC60730 and OEM Customization, refer to the IEC60730 Safety Library Integration to SDK in the docs folder for more details.
 *
 * See \link oem_customization OEM Customization \endlink for details on
 * customizing the project and adding custom code.
 *
 * @warning While source code is provided for the IEC60730 library,
 * Silicon Labs recommends using the certified library file.
 * When applying for certification, the certified library file will
 * reduce the certification time.
 *
 * @note The Simplicity Studio debugger will allow breakpoints and code tracing
 * into the IEC60730 library, except for sl_iec60730_cpu_registers_bist(), since
 * that test requires special compiler commands. To set breakpoints or
 * trace inside sl_iec60730_cpu_registers_bist(), set a breakpoint inside
 * sl_iec60730_bist() at the line with sl_iec60730_cpu_registers_bist(), and
 * single-step into the function.
 *
 * @section document Generate document API
 *
 * @subsection doxygen Generate document
 *
 * Using Doxygen to generate HTML documentation, the documents will be generated in **docs/document_api_iec60730_library**
 *
 *```sh
 *  $ sudo apt-get install doxygen
 *  $ doxygen iec60730.doxygen
 *```
 *
 * Using MkDocs to support read mark-down files.
 *
 * @subsection mkdocs MkDocs
 *
 * MkDocs User Guide:
 *
 * - https://www.mkdocs.org/user-guide/
 *
 * MkDocs markdown fetaures and syntax reference:
 *
 * - https://squidfunk.github.io/mkdocs-material/reference/
 *
 * Search for icons and Emojis:
 *
 * - https://squidfunk.github.io/mkdocs-material/reference/icons-emojis/#search
 *
 * #### Install MkDocs
 *
 * *Step 1:** Install [mkdocs](https://www.mkdocs.org/)
 *
 * ```
 * pip3 install mkdocs
 * ```
 *
 * *Step 2:** Install [mkdocs-material](https://pypi.org/project/mkdocs-material-extensions/)
 *
 * ```
 * pip3 install mkdocs-material
 * ```
 *
 * *Step 3:** Install [mkdocs-material-extensions](https://pypi.org/project/mkdocs-material-extensions/)
 *
 * ```
 * pip3 install mkdocs-material-extensions
 * ```
 *
 * #### Verify and build your documentaion
 *
 * - Ensure you have `mkdocs.yml` file in place
 *
 * - Preview your documentation locally
 *
 * ```
 * python3 -m mkdocs serve
 * ```
 *
 * - If you want to use your browser to read your docs locally. Build the pages in the folder `./site`, which will be added to your project. It will starting from the `site/index.html`
 *
 * ```
 * python3 -m mkdocs build --no-directory-urls
 * ```
 *
 * @note To support running **Github Pages**, you need to run the command above.
 *
 * @section pre-commit Coding convention tool
 *
 * @subsection features Features
 * - Automatically fixes end-of-file issues.
 * - Removes trailing whitespace from lines.
 * - Identifies and suggests fixes for common spelling errors using codespell.
 * - Formats code according to specified Uncrustify rules base on Silabs's coding standard.
 * - Checks for identifiers naming style mismatch of function, definition, struct, variable using clang-tidy.
 * - Checks code for potential errors and style issues using cppcheck.
 *
 * @subsection installation Installation
 * Recommended version:
 * - Codespell 2.2.4
 * - Uncrustify 0.72.0
 * - Clang tidy 14.0.0
 * - Cppcheck 1.9
 *
 *```sh
 *  $ pip install pre-commit
 *  $ sudo apt install uncrustify clang-tidy cppcheck
 *```
 *
 * @subsection run Run
 *
 * Run pre-commit install to install pre-commit into your git hooks. pre-commit will now run on every commit:
 *
 *```sh
 *  $ pre-commit install
 *```
 *
 * Staging files need formatting. For example:
 *
 *```sh
 *  $ git add data_format_sample.c
 *```
 *
 * Run pre-commit hooks on a repository to check coding convention.
 *
 *```sh
 *  $ pre-commit run --all-files
 *```
 *
 * @section compilers Compiler specifications
 * The C compilers:
 *
 * - GNU GCC V12.2
 *
 * - IAR EW for ARM V9.20
 *
 * Tools specification
 *
 * - Test frame work tools:
 *
 *   - Python V3.11
 *
 * - Simplicity Studio Commander
 *
 *   - Simplicity Commander version 1v16p8b1613
 *
 * - Support CMake
 *
 *   - CMake version >=3.25
 *
 * - SEGGER JLink
 *
 *   - JLink version 17.0.11
 *
 * - SLC-CLI
 *
 *   - slc version 5.9.1.1
 *
 * - Source code control:
 *
 *   - GIT V2.39
 *
 *   - JIRA V8.22.2
 *
 * - SDK support:
 *
 *   - Gecko SDK Suite v4.4.2
 *
 * @note If you want this extension to match your geck_sdk change the sdk field in the `iec60730.slce` file.
 * For example, you want to use simplicity sdk version:
 * ```sh
 * sdk:
 * id: simplicity_sdk
 * version: 2024.6.1
 * ```
 *
 * @section system_architecture_spec System Architecture
 *
 * This library consists of two main components. The
 * \link sl_iec60730_post POST component \endlink operates immediately after the power is turned on, validating the system state before entering the main execution loop.
 *
 * The \link sl_iec60730_bist BIST component \endlink is executed periodically within the main execution loop.
 *
 * Validation for IEC60730 also requires external communications validation. Each OEM application has unique external communication requirements.
 * This library includes an example UART communications library that complies with IEC60730 standards. OEMs must adapt their communications to meet the requirements outlined in IEC60730.
 *
 * The file oem_iec60730.c contains functions and variables that OEMs should modify to suit their specific systems.
 *
 * This EFR32 IEC60730 library fulfills the requirements for a **Class B** device.
 * \link iec60730_table1 Table 1\endlink outlines the firmware requirements that must be met by controls, as specified in IEC60730, including new entries from Annex H.
 *
 * \link iec60730_tableh1 Table 2\endlink details the firmware-specific requirements for software-based controls, derived from Table H.1 in IEC60730.
 *
 * <center><b>Table 1 Requirements that must be met by controls </b></center>\anchor iec60730_table1
 * Information | Clause | Method [1] | Notes specific to this library [2]
 * ------------|--------|------------|-----------------------------------
 * 36 Limits of activating quantity | 11.3.2 H.11.4.15 H.17.14 H.18.1.5 H.27.1.1 H.28 | X | Not Applicable
 * 52 Minimum parameters of any heat dissipator | 14 | X | Not Applicable
 * 53 Type of output waveform if other than sinusoidal | H.25 | X | Not Applicable
 * 54 Details of the leakage current waveform produced after failure of the basic insulation | H.27 | X | Not Applicable
 * 55 Relevant parameters of electronic devices unlikely to fail | H.27 | X | Not Applicable
 * 56 Type of output waveform produced after failure of an electronic device | H.27 | X | Not Applicable
 * 57 The effect on controlled outputs after electronic circuit component failure | H.27 | X | To be provided by OEM
 * 58b The effect on controlled outputs after a failure to operate as a result of tests | H.26.2 H.26.15 | X | Not Applicable
 * 66 Software sequence documentation | H.11.12.2.9 | D | Covered by this documentation
 * 67 Program documentation | H.11.12.2.9 H.11.12.2.12 | D | Covered by this documentation
 * 68 Software fault analysis | H.11.12 H.27.1.1.4 | D | Covered by this documentation
 * 69 Software class(es) and structure | H.11.12.2 H.11.12.3 H.27.1.2.2.1 H.27.1.2.3.1 | D | Covered by this documentation
 * 70 Analytical measure and fault/error control techniques employed | H.11.12.1.2 | D | Covered by this documentation
 * 71 Software fault/error detection times for controls | H.2.17.10 | X | Provided by OEM timer ticks. Example uses 1 second.
 * 72 Control responses in case of detected fault/error | H.11.12.2.7 | X | Must be provided by OEM
 * 73 Controls subjected to a second fault analysis and declared condition as a result of the second fault | H.27.1.2.3 | X | Must be provided by OEM
 * 74. External load and emission control measures to be used for test purposes | H.23.1.1 | X | Not Applicable
 * 91 Fault reaction time |H.2.23.2 H.27.1.2.2.2 H.27.1.2.2.3 H.27.1.2.3.2 H.27.1.2.3.3 H.27.1.2.4.2 H.27.1.2.4.3 | X | Provided by OEM timer ticks. Example uses 1 second.
 * 92 Class or classes of control functions | H.6.18 H.27.1.2.2 H.27.1.2.3 | D | This library is for Class B control functions
 * 93 Maximum number of reset actions within a time period | H.11.12.4.3.6 H.11.12.4.3.4 | X | Not Applicable
 * 94 Number of remote reset actions | H.17.1.4.3 | X | Not Applicable
 *
 * 1. Methods (See IEC60730 documentation for additional details):
 *   - C - Marking
 *   - D - Hard copy
 *   - E - Documentation on electronic media, internal or external
 *   - X - Declaration on materials provided to the certification body
 * 2. OEMs are still responsible for providing any documentation for software outside the IEC60730 library.
 *
 * <center><b>Table 2 Measures to address software fault/errors</b></center> \anchor iec60730_tableh1
 *
 * Component | Measure Used | Notes
 * ----------|-------------|-----
 * \link IEC60730_CPU_REG_TEST 1.1 Registers\endlink | Periodic self-test using a Static memory test | Provided by library
 * 1.2 Instruction decoding and execution | None | Not required for Class B
 * \link IEC60730_PROGRAM_COUNTER 1.3 Program counter\endlink / \link IEC60730_WDOG_TEST Watchdog\endlink | Logical monitoring of the program sequence. Code execution sets flag that is periodically checked.  Watchdog timer prevents runaway program execution. | Provided by library, see example for integration sample
 * 1.4 Addressing | None | Not required for Class B
 * 1.5 Data paths Instruction decoding | None | Not required for Class B
 * \link IEC60730_IRQ_TEST 2 Interrupt handling and execution\endlink | Time-slot monitoring(upper and lower limit) | Provided by library, see example for integration sample
 * \link IEC60730_SYSTEM_CLOCK_TEST 3 Clock\endlink | Reciprocal Comparison. Use separate oscillator to monitor SYSCLK - Calculate ratio and determine range based on accuracy | Provided by library, see example for integration sample
 * \link IEC60730_INVARIABLE_MEMORY_TEST 4.1 Invariable memory\endlink | Periodic 16 bit CRC | Provided by library, see example for integration sample
 * \link IEC60730_VARIABLE_MEMORY_TEST 4.2 Variable memory\endlink | Periodic static memory test using March-C & stack guard. | Provided by library, see example for integration sample
 * 4.3 Addressing (variable and invariable memory) | | 4.1 and 4.2 provide coverage for this component
 * 5.1 Internal data path | | 4.1 and 4.2 provide coverage for this component
 * 5.2 Internal addressing | | 4.1 and 4.2 provide coverage for this component
 * \link IEC60730_OEM_COMM_TEST 6 External Communication \endlink | | Provided by OEM, UART example in library
 * \link IEC60730_OEM_COMM_TEST 6.1 External Communications - Data \endlink | 16 bit CRC | CRC check provided by library
 * \link IEC60730_OEM_COMM_TEST 6.2 External Communications - Addressing \endlink | 16 bit CRC including the address | OEM must include in protocol proper address verification - see UART example in library
 * \link IEC60730_OEM_COMM_TEST 6.3 External Communications - Timing (UART example)\endlink | Scheduled transmission |  OEM must include in protocol proper timing measures - see UART example in library
 * 7 Input/output periphery / 7.1 Digital I/O | Plausibility check | Provided by OEM
 * 7.2 Analog I/O / 7.2.1 A/D and D/A converter | Plausibility check | Provided by OEM
 * 7.2.2 Analog multiplexer  | Plausibility check | Provided by OEM
 * 8 Monitoring device and comparators | None | Not needed for class B
 * 9 Custom chips | None | Not Applicable
 *
 * \section test_specifications Test Specifications
 * \link IEC60730_VERIFICATION Test Specifications\endlink provide detailed
 * specifications for verification testing.
 *
 * @section coding_standard Coding Standard
 * The IEC60730 library follows Silicon Labs Coding Standard.
 *
 * @section support Support
 * Customers with support questions can begin with the
 * <a href="https://www.silabs.com/support">Silicon Labs support page. </a>
 * Customers can also post their questions to the Silicon Labs
 * <a href="https://community.silabs.com/s/?language=en_US">Community Forums</a>
 * for help from other Silicon Labs customers.
 *
 * If a customer needs help with a complex issue, create a Support Request on
 * the support page. The Silicon Labs MCU Applications Engineering team will assist
 * with duplicating the problem and understanding the root cause.
 *
 * @section update Change Control and Update process
 * The IEC60730 library is a complex software project that will require ongoing
 * support and updates in the future. This section outlines how Silicon Labs will assist
 * customers with their implementation and release updates for the IEC60730 library.
 *
 * Once an issue is confirmed to be a problem with the IEC60730 Library,
 * a JIRA ticket must be filed within the Developer Services Team project (DS),
 * starting with the prefix "IEC60730: ".The team lead will assess each issue and assign
 * a severity level, which can be categorized as follows:
 *   - Blocker (Customer cannot move forward/is lines-down)
 *   - Critical (Customer can proceed, but with major inconvenience/no workaround)
 *   - High (Customer can proceed with minor inconvenience/workaround)
 *   - Low (Customer can proceed with no inconvenience, no workaround necessary).
 * Each ticket will also include information about the impact of the change on the library
 * and an estimated timeline for implementation.
 *
 * The issue ticket will be assigned to the Developer Services Team manager later.
 * At their discretion, they may convene a meeting with the IEC60730 Change Control Board
 * (comprising the DS team lead) to discuss the open JIRA tickets for the IEC60730 Library.
 * After the meeting, Tickets on JIRA will be created and determine whether the issue ticket
 * will be fixed or not. The status of each ticket JIRA will be updated accordingly,
 * and work will be delegated to the appropriate firmware team members. Work will be conducted
 * in a separate branch from the main branch within the Git Version Control System.
 *
 * Once the updated firmware is complete, the developer will check it into the Silicon Labs Git version
 * control system and create a Code Review JIRA ticket. A review developer will then examine the code
 * to ensure compliance with the Coding Standard. If feasible, a Git Pull Request will be created
 * between the two revisions of the source code. Any comments from the review developer will be logged
 * in a Git branch of the IEC60730 project. The Git commit ID will be linked to the Code Review
 * JIRA ticket. The developer is responsible for addressing the comments from the code review
 * and providing explanations for any declined changes.
 *
 * Following these steps, the version of the IEC60730 library will be incremented. For any issues
 * resolved, an automated test will be created to replicate the previous issue and demonstrate that
 * it no longer exists in the new library. The developer must also confirm that the Python code
 * functions correctly when the original tests are executed.
 *
 * If an issue cannot be validated through automated testing, a special waiver must be obtained from
 * the Change Control Board, with instructions for manual testing included in the documentation for
 * the affected module. A complete automated and manual validation cycle must be completed before
 * the updated library is ready for release. This is done by initiating a Daily Firmware build job
 * in the Jenkins build and test environment, using the Git branch as the source.
 *
 * Once the Daily Firmware build job and any required manual tests confirm the correct operation of
 * the firmware, the branch will be merged back into the master branch. A Release Build will then be
 * triggered in Jenkins, using the new release version along with the master branch.
 *
 * The MCU FW Team Manager must sign off on the validation results via JIRA. After the library passes
 * validation, the updated library, along with a change list, will be submitted to the certification
 * house for re-certification. Upon completion of re-certification, an Errata document will be created
 * or updated to include a list of all resolved and unresolved issues, along with any available workarounds.
 *
 * The new library, along with the Errata document and an updated Readme file containing the new version
 * number, will be uploaded to the Silicon Labs downloads site and made available through Simplicity Studio.
 *
 * @section oem_customization OEM Customization and Integration
 * An OEM must be aware of these items when integrating the IEC60730 library
 * into their final product.
 *
 * @warning Any certification image must use the Release source files of the IEC60730
 * library.
 *
 * @subsection safe_state_integration Safe state
 * OEMs must customize the sl_iec60730_safe_state() function according to their system configuration.
 * Safe State must configure any external signals and communications channels such that the system
 * will not cause damage or unexpected operation. The function may attempt external notification,
 * however it must be done cautiously to prevent further deterioration of the system.
 *
 * @subsection post_integration POST
 * sl_iec60730_post() is normally called after system initialization is complete,
 * but before beginning primary operating mode. It includes a Watchdog test
 * that resets the system to verify Watchdog operation. OEMs must expect
 * initialization code before sl_iec60730_post() to execute twice. Initialization
 * code execution time must be short enough that the watchdog can be refreshed
 * in sl_iec60730_post() before expiration.
 *
 * @subsection bist_integration BIST
 * sl_iec60730_bist() is executed as part of the main system loop, typically at
 * the end. Systems with long execution times will require manual watchdog
 * refresh, and adjustment of sl_iec60730_test_clock_tick() frequency to ensure
 * sl_iec60730_program_counter_test() passes, or calling sl_iec60730_bist() at
 * multiple locations in the main system loop.
 *
 * When in safety-critical code where POST and BIST are executing, interrupts
 * should remain globally enabled.  If interrupts must be disabled for a
 * critical section of firmware, the critical section should follow best
 * practices and be as short as possible. The time in the critical section
 * must be shorter than the fastest clock timer interrupt (usually 10ms), or
 * sl_iec60730_sys_clock_test_disable() must be used to disable the timer tests.
 *
 * @subsection program_counter_integration Program counter
 * #IEC60730_GPIO_COMPLETE and #IEC60730_ANALOG_COMPLETE must be set by OEM
 * validation code.
 *
 * OEMs can use #IEC60730_OEM0_COMPLETE - #IEC60730_OEM7_COMPLETE or into
 * #sl_iec60730_program_counter_check to verify their own test algorithms are
 * executing at least once per every call to sl_iec60730_program_counter_test().
 * Unused flags must be set to 1.
 *
 * @warning sl_iec60730_bist() can take several ms to execute, see
 * \link resources_used Execution Time \endlink for details. If the
 * execution time is longer than the system can tolerate, customize the
 * sl_iec60730_bist() routine or split it up into individual calls in
 * the main system loop.
 *
 * @subsection watchdog_integration Watchdog
 * User should setup the internal watchdog as user's design expectation.
 *
 * When in a long latency loop, use sl_iec60730_restart_watchdogs() to prevent a
 * watchdog reset. Minimize the time in the loop as much as possible.
 *
 * @warning Call sl_iec60730_restart_watchdogs() to re-enable
 * the watchdog once the flash erase has completed. During the flash erase
 * time, any control signals must remain in a safe configuration.
 * \link IEC60730_WDOG_TEST Watchdog Test \endlink provides details.
 *
 * @subsection irq_integration IRQ
 * OEMs must update **oem_irq_freq_bounds** for their system use. Interrupt
 * service routines must include code for incrementing
 * **oem_irq_exec_count**. For an example, see **oem_iec60730.c**. OEMs
 * must choose and enumerate the index values for **oem_irq_exec_count**.
 *
 * @warning Interrupts are disabled during portions of the \link IEC60730_CPU_REG_TEST CPU Registers Check \endlink
 * , \link IEC60730_INVARIABLE_MEMORY_TEST
 * Invariable Memory Check \endlink and \link IEC60730_VARIABLE_MEMORY_TEST
 * Variable Memory Check \endlink. Interrupts must be tolerant of the latency
 * times these tests introduce to servicing an interrupt.
 *
 * @subsection clock_integration Clock
 * OEMs can modify the default System Clock and Timer Clock configurations
 * in **oem_iec60730_timer.c** according to their system requirements.
 *
 * @subsection invariable_memory_integration Invariable memory
 * OEMs must modify the sl_iec60730_imc_test_region_t structure to align with their
 * memory usage. **iec60730_cur_crc** or similar must be modified to store
 * the CRC values. The number of areas to be tested depends on the OEM
 *
 * During the CRC generation for a block of memory, the CPU is halted and
 * unable to respond to interrupt requests.
 *
 * For a typical system, the amount of invariable memory checked will
 * determine the fault reaction time.
 *
 * @subsection variable_memory_integration Variable memory
 * OEMs must modify the sl_iec60730_vmc_test_region_t structure to align with the
 * RAM they want to check. The number of areas to be tested depends on the OEM.
 *
 * During the CRC generation for a block of memory, the CPU is halted and
 * unable to respond to interrupt requests.
 *
 * @subsection external_communications_integration External communications
 * OEMs must write their safety-related communications protocol according to
 * the IEC60730 requirements. The demo provides an example protocol test for
 * **UART 0**.
 *
 * @subsection additional_oem_tests Additional OEM safety checks
 * OEMs can implement additional safety checks and call sl_iec60730_safe_state()
 * with custom define #SL_IEC60730_OEM_FAIL_1 - #SL_IEC60730_OEM_FAIL_4 settings.
 *
 * \subsection resources_used Execution Time
 * The sizes given below are for the example programs demo provided by extension with the SDK.
 *
 * OEM devices that do not use a communications channel(~400 bytes), or
 * have simple plausilbity functions (~200 bytes), will use less space.
 *
 * OEMs can use these numbers for planning purposes, but must verify against
 * their implementation.
 *
 *
 * @subsection bist_frequency BIST call frequency
 * All tests must complete execution before sl_iec60730_program_counter_test()
 * is called. The example code calls sl_iec60730_program_counter_test() once per
 * second.
 *
 * The test requiring the most iterations to complete is the \link IEC60730_INVARIABLE_MEMORY_TEST
 * Invariable Memory Check \endlink and \link IEC60730_VARIABLE_MEMORY_TEST
 * Variable Memory Check \endlink. It may require calls to
 * sl_iec60730_bist() to guarantee all the invariable memory areas and variable memory areas are checked
 * before sl_iec60730_program_counter_test() is called. This table assumes the
 * largest invariable memory size for each device, and a check across
 * the whole memory area.
 *
 * The number of blocks per BIST call #SL_IEC60730_INVAR_BLOCKS_PER_BIST is chosen to
 * get the time between BIST calls close to the watchdog timeout.
 *
 * Number of sl_iec60730_bist() calls/second = Size of memory / (Size of CRC block * number of blocks per BIST call)
 *
 * Maximum time between BIST calls= 1 / (Number of sl_iec60730_bist calls)
 *
 * Checking against smaller areas increase the time between BIST calls.
 *
 * @subsection fault_reaction_time Fault Reaction Time
 * The default Fault Reaction Time for the IEC60730 library is 1 second.
 * Because of the BIST call frequency configuration above, a full invariable
 * memory test can take 1 second to run on the largest memory EFR32 devices.
 * The \ref program_counter_integration check runs once per second by
 * default, and all completion bits must be set. OEMs can modify the
 * \ref clock_integration according to their needs to increase the
 * Fault Reaction Time.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup sl_iec60730_post POST
 * @{
 * @brief Power On Self Test - Executed once during power up.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup sl_iec60730_bist BIST
 * @{
 * @brief Built In Self Test - Executed periodically.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup IEC60730_PROGRAM_COUNTER Program Counter Check
 * @{
 * @brief Verifies all tests have completed on time.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup IEC60730_WDOG_TEST Watchdog Check
 * @{
 * @brief Monitors CPU execution.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup IEC60730_IRQ_TEST IRQ Check
 * @{
 * @brief Verifies interrupt frequency is within bounds.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup IEC60730_INVARIABLE_MEMORY_TEST Invariable Memory Check
 * @{
 * @brief Verifies contents of flash memory.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup IEC60730_VARIABLE_MEMORY_TEST Variable Memory Check
 * @{
 * @brief Verifies RAM is working correctly.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup IEC60730_CPU_REG_TEST CPU Registers Check
 * @{
 * @brief Verifies CPU registers are working correctly.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup IEC60730_SYSTEM_CLOCK_TEST System Clock Check
 * @{
 * @brief Verifies that system clocks are within expected frequencies.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup IEC60730_SAFE_STATE Safe State
 * @{
 * @brief When incorrect behavior is detected, this state prevents further execution.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @defgroup IEC60730_OEM_COMM_TEST OEM External Communications Example using UART
 * @{
 * @brief Verifies communication channel is operating as expected.
 * @}
 *****************************************************************************/

/** @} (end addtogroup efr32_iec60730) */

/**************************************************************************/ /**
 * @defgroup IEC60730_VERIFICATION IEC60730 Test Specification
 * @{
 * @brief Automated tests for validating correct firmware operation.
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @addtogroup IEC60730_VERIFICATION
 * @{
 * @defgroup IEC60730_INTEGRATION_TEST Integration Test Modules In Library IEC60730
 * @{
 * @brief Python script supports running test case integration tests for internal modules in library IEC60730.
 * @}
 * @}
 *****************************************************************************/

/**************************************************************************/ /**
 * @addtogroup IEC60730_VERIFICATION
 * @{
 * @defgroup IEC60730_UNIT_TEST UNIT Test Functions In Modules In Library IEC60730
 * @{
 * @brief C file combined with Unity framework.
 * @}
 * @}
 *****************************************************************************/
