# Define minimal required version of CMake.
cmake_minimum_required(VERSION "3.25")

# Project definition
project(
	IE60730_LIBRARY
	VERSION 2.0.0
	LANGUAGES C ASM CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBRARY_NAME iec60730)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
set(LIB_IEC60730_MAP "lib_iec60730.map")
set(SDK_CMAKE_RELATIVE_DIR "simplicity_sdk")
set(SDK_LIB_CMAKE_RELATIVE_DIR  "${SDK_CMAKE_RELATIVE_DIR}/src/lib_iec60730_cmake")

# choose toolchain
if(DEFINED ENV{TOOL_CHAINS})
  set(TOOL_CHAINS "$ENV{TOOL_CHAINS}")
else()
set(TOOL_CHAINS "GCC")
endif()
message("-- [I] Toolchains build: ${TOOL_CHAINS}")

# enable unit test
option(ENABLE_UNIT_TESTING "Enable a Unit Testing Build" OFF)
# enable integration test
option(ENABLE_INTEGRATION_TESTING "Enable a Integration Testing Build" OFF)
# enable test peripherals non-secure test
option(TEST_SECURE_PERIPHERALS_ENABLE "Check non-secure peripherals" OFF)
# enable integration test watchdog 1
option(INTEGRATION_TEST_WDOG1_ENABLE "Enable integration test watchdog 1" OFF)
# disable using MarchXC algorithm
option(INTEGRATION_TEST_USE_MARCHX_DISABLE "Disable using MarchXC algorithm" OFF)
# pre-build iar tool
option(PRE_BUILD_IAR_TOOL "pre-build iar tool" OFF)
# Coding convention
option(CHECK_CODING_CONVETION_ENABLE "check coding convention" OFF)

# choose board name
option(BOARD_NAME "choose board name" brd4187c)
message("-- [I] Board name: ${BOARD_NAME}")

#Add external libraries with Fetch Content
include(FetchContent)

if(ENABLE_UNIT_TESTING AND (NOT PRE_BUILD_IAR_TOOL))
    FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity
        GIT_TAG v2.6.0
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(unity)
endif()

# Support calculate CRC
option(ENABLE_CAL_CRC_32 "Enable calculate crc 32" OFF)
option(ENABLE_CRC_USE_SW "Enable using software calculate CRC" OFF)
option(ENABLE_SW_CRC_TABLE "Enable software calculate CRC using table CRC" OFF)
set(OPTION_SUPPORT_TEST_IMC_MODULE "ENABLE_CAL_CRC_32 ENABLE_CRC_USE_SW ENABLE_SW_CRC_TABLE")

# Set address start calculate crc
if(DEFINED ENV{FLASH_REGIONS_TEST})
  set(FLASH_REGIONS_TEST "$ENV{FLASH_REGIONS_TEST}")
else()
  set(FLASH_REGIONS_TEST "0x00000000")
endif()

message("-- [I] Options to support running tests of invariable memory modules: ${OPTION_SUPPORT_TEST_IMC_MODULE}")

if(ENABLE_CAL_CRC_32)
  message("-- [I] Support crc calculation for invariable memory modules: CRC-32")
else()
  message("-- [I] Support crc calculation for invariable memory modules: CRC-16")
endif()

if(ENABLE_CRC_USE_SW)
  message("-- [I] Enable using software to calculate CRC")
  if(ENABLE_SW_CRC_TABLE)
    message("-- [I] Enable software using CRC pre-defined table to calculate CRC")
  else() # ENABLE_SW_CRC_TABLE
    message("-- [I] Disable software using CRC pre-defined table to calculate CRC")
  endif() # ENABLE_SW_CRC_TABLE
else() # ENABLE_CRC_USE_SW
  message("-- [I] Enable using hardware GPCRC to calculate CRC")
endif() # ENABLE_CRC_USE_SW

message("-- [I] Start address calculate crc value of flash: ${FLASH_REGIONS_TEST}")

# Run generate_lib_iec60730
include(generate_lib_iec60730)

if(${TOOL_CHAINS} STREQUAL "GCC")
  if(ENABLE_UNIT_TESTING)
    generate_lib_iec60730(${SDK_CMAKE_RELATIVE_DIR})
    get_target_property(MAIN_CFLAGS_LIB_IEC60730 slc_lib_iec60730 COMPILE_OPTIONS)
    string(REPLACE "$<$<COMPILE_LANGUAGE:C>:SHELL:-imacros sl_gcc_preinclude.h>;" "" MAIN_CFLAGS_LIB_IEC60730 "${MAIN_CFLAGS_LIB_IEC60730}")
    string(REPLACE "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-imacros sl_gcc_preinclude.h>;" "" MAIN_CFLAGS_LIB_IEC60730 "${MAIN_CFLAGS_LIB_IEC60730}")
    string(REPLACE "$<$<COMPILE_LANGUAGE:ASM>:SHELL:-imacros sl_gcc_preinclude.h>;" "" MAIN_CFLAGS_LIB_IEC60730 "${MAIN_CFLAGS_LIB_IEC60730}")
    target_compile_options(unity PUBLIC
                          ${MAIN_CFLAGS_LIB_IEC60730})

  endif()

  if(ENABLE_INTEGRATION_TESTING)

    if(TEST_SECURE_PERIPHERALS_ENABLE)
      message("-- [I] Enable integration testing check secure peripherals")
    else()
      message("-- [I] Enable integration testing check non-secure peripherals")
    endif()

    if(INTEGRATION_TEST_WDOG1_ENABLE)
      message("-- [I] Watchdog 1 testing: enable")
    else()
      message("-- [I] Watchdog 1 testing: disable")
    endif()

    if(INTEGRATION_TEST_USE_MARCHX_DISABLE)
      message("-- [I] Using MarchXC algorithm: disable")
    else()
      message("-- [I] Using MarchXC algorithm: enable ")
    endif()

    generate_lib_iec60730(${SDK_CMAKE_RELATIVE_DIR})
    get_target_property(COMPILE_DEFINITIONS_IEC60730 slc_lib_iec60730 COMPILE_DEFINITIONS)
  endif()

elseif (${TOOL_CHAINS} STREQUAL "IAR")

  if(PRE_BUILD_IAR_TOOL)
    generate_lib_iec60730(${SDK_CMAKE_RELATIVE_DIR})
    get_target_property(MAIN_CFLAGS_LIB_IEC60730 slc_lib_iec60730 COMPILE_OPTIONS)
    string(FIND "${MAIN_CFLAGS_LIB_IEC60730}" "cortex-m33" CORE_TYPE)
    if(${CORE_TYPE} GREATER 0)
      #message("-- [I] CORE_TYPE: cortex-m33")
      set(PRE_IAR_CORE_TYPE "CORE_TYPE=m33")
    else()
      #message("-- [I] CORE_TYPE: cortex-m4")
      set(PRE_IAR_CORE_TYPE "")
    endif()

    if(ENABLE_CAL_CRC_32)
      set(PRE_IAR_CAL_CRC_32  "CAL_CRC_32=enable")
    else()
      set(PRE_IAR_CAL_CRC_32  "")
    endif()

    if(TEST_SECURE_PERIPHERALS_ENABLE)
      set(PRE_IAR_SECURE_PERIPHERALS_ENABLE  "SECURE_PERIPHERALS_ENABLE=enable")
    else()
      set(PRE_SECURE_PERIPHERALS_ENABLE  "")
    endif()

    execute_process(COMMAND python3 ${CMAKE_SOURCE_DIR}/${SDK_CMAKE_RELATIVE_DIR}/pre_build_iar.py ${PRE_IAR_CORE_TYPE} ${PRE_IAR_CAL_CRC_32} ${PRE_IAR_SECURE_PERIPHERALS_ENABLE})
  endif()

  if(ENABLE_UNIT_TESTING)

    add_subdirectory(${SDK_CMAKE_RELATIVE_DIR})
    get_target_property(MAIN_CFLAGS_LIB_IEC60730 slc_lib_iec60730 COMPILE_OPTIONS)
    string(REPLACE "$<$<COMPILE_LANGUAGE:C>:SHELL:-imacros sl_gcc_preinclude.h>;" "" MAIN_CFLAGS_LIB_IEC60730 "${MAIN_CFLAGS_LIB_IEC60730}")
    string(REPLACE "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-imacros sl_gcc_preinclude.h>;" "" MAIN_CFLAGS_LIB_IEC60730 "${MAIN_CFLAGS_LIB_IEC60730}")
    string(REPLACE "$<$<COMPILE_LANGUAGE:ASM>:SHELL:-imacros sl_gcc_preinclude.h>;" "" MAIN_CFLAGS_LIB_IEC60730 "${MAIN_CFLAGS_LIB_IEC60730}")
    target_compile_options(unity PUBLIC
                          ${MAIN_CFLAGS_LIB_IEC60730})
    set_property(TARGET unity PROPERTY C_STANDARD_REQUIRED OFF)

  endif()

  if(ENABLE_INTEGRATION_TESTING)

    add_subdirectory(${SDK_CMAKE_RELATIVE_DIR})
    if(TEST_SECURE_PERIPHERALS_ENABLE)
      message("-- [I] Enable integration testing check secure peripherals")
    else()
      message("-- [I] Enable integration testing check non-secure peripherals")
    endif()

    if(INTEGRATION_TEST_WDOG1_ENABLE)
      message("-- [I] Watchdog 1 testing: enable")
    else()
      message("-- [I] Watchdog 1 testing: disable")
    endif()

    if(INTEGRATION_TEST_USE_MARCHX_DISABLE)
      message("-- [I] Using MarchXC algorithm: disable")
    else()
      message("-- [I] Using MarchXC algorithm: enable ")
    endif()

    get_target_property(COMPILE_DEFINITIONS_IEC60730 slc_lib_iec60730 COMPILE_DEFINITIONS)
  endif()
endif()

# Start run test
add_subdirectory(lib)

if(ENABLE_INTEGRATION_TESTING OR ENABLE_UNIT_TESTING)
  add_subdirectory(test)
endif()

# Start check coding convention
# Run coding_convention
include(coding_convention)

if(CHECK_CODING_CONVETION_ENABLE)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  # Disable response files
  set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES ON)
  message("-- [I] Check coding convention: enable")
  generate_coding_convention(${SDK_CMAKE_RELATIVE_DIR})
endif()
