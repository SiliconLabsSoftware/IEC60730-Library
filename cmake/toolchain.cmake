set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(WIN32)
  set(EXE_SUFFIX ".exe")
  cmake_path(SET USER_DIR "$ENV{USERPROFILE}" NORMALIZE)
else()
  set(EXE_SUFFIX "")
  cmake_path(SET USER_DIR "$ENV{HOME}" NORMALIZE)
endif()

#
# Discover tools using SLT when available
#
execute_process(
  COMMAND slt where gcc-arm-none-eabi/12.2.rel1
  OUTPUT_VARIABLE ARM_GCC_SLT_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

execute_process(
  COMMAND slt where commander
  OUTPUT_VARIABLE COMMANDER_SLT_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

#
# Verify SLC CLI
#
find_program(SLC_EXE slc)

if(NOT SLC_EXE)
  message(FATAL_ERROR
    "slc not found in PATH.\n"
    "Please add Simplicity Studio SLC CLI to PATH.\n"
    "Example:\n"
    "export PATH=$HOME/.silabs/slt/installs/archive/slc-cli-v6.0.25/slc_cli:$PATH")
endif()

#
# Resolve GCC toolchain location
#
# Priority:
# 1. TOOL_DIRS
# 2. ARM_GCC_DIR
# 3. SLT
# 4. Windows fallback
# 5. Linux fallback
#
if(DEFINED ENV{TOOL_DIRS})

  set(TOOLCHAIN_DIR "$ENV{TOOL_DIRS}/")

elseif(DEFINED ENV{ARM_GCC_DIR})

  set(TOOLCHAIN_DIR "$ENV{ARM_GCC_DIR}/bin/")

elseif(ARM_GCC_SLT_PATH)

  set(TOOLCHAIN_DIR "${ARM_GCC_SLT_PATH}/bin/")

elseif(WIN32)

  set(TOOLCHAIN_DIR
      "${USER_DIR}/.silabs/slt/installs/conan/p/gcc-ab83b3403fdca6/p/bin/")

else()

  set(TOOLCHAIN_DIR "/bin/")

endif()

set(TARGET_TRIPLET "arm-none-eabi-")

if(NOT EXISTS
    "${TOOLCHAIN_DIR}${TARGET_TRIPLET}gcc${EXE_SUFFIX}")

  message(FATAL_ERROR
    "ARM GCC compiler not found.\n"
    "TOOLCHAIN_DIR=${TOOLCHAIN_DIR}\n"
    "Please set TOOL_DIRS, ARM_GCC_DIR, or install via SLT.")

endif()

#
# Commander
#
if(DEFINED ENV{POST_BUILD_EXE})

  set(POST_BUILD_EXE "$ENV{POST_BUILD_EXE}")

elseif(COMMANDER_SLT_PATH)

  if(WIN32)

    set(POST_BUILD_EXE
        "${COMMANDER_SLT_PATH}/commander.exe")

  elseif(APPLE)

    set(POST_BUILD_EXE
        "${COMMANDER_SLT_PATH}/Contents/MacOS/commander")

  else()

    set(POST_BUILD_EXE
        "${COMMANDER_SLT_PATH}/commander")

  endif()

elseif(WIN32)

  set(POST_BUILD_EXE
      "${USER_DIR}/.silabs/slt/installs/archive/Simplicity Commander/commander.exe")

else()

  set(POST_BUILD_EXE "")

endif()

#
# Toolchain executables
#
set(CMAKE_C_COMPILER
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}gcc${EXE_SUFFIX})

set(CMAKE_CXX_COMPILER
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}g++${EXE_SUFFIX})

set(CMAKE_ASM_COMPILER
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}gcc${EXE_SUFFIX})

set(CMAKE_LINKER
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}gcc${EXE_SUFFIX})

set(CMAKE_AR
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}ar${EXE_SUFFIX})

set(CMAKE_RANLIB
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}gcc-ranlib${EXE_SUFFIX})

set(CMAKE_OBJCOPY
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}objcopy${EXE_SUFFIX})

set(CMAKE_OBJDUMP
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}objdump${EXE_SUFFIX})

set(CMAKE_SIZE_UTIL
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}size${EXE_SUFFIX})

set(CMAKE_STRIP
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}strip${EXE_SUFFIX})

set(CMAKE_GCOV
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}gcov${EXE_SUFFIX})

set(CMAKE_NM_UTIL
    ${TOOLCHAIN_DIR}${TARGET_TRIPLET}gcc-nm${EXE_SUFFIX})

#
# Objcopy formats
#
set(OBJCOPY_SREC_CMD "-O;srec")
set(OBJCOPY_IHEX_CMD "-O;ihex")
set(OBJCOPY_BIN_CMD "-O;binary")

#
# Language standard options
#
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS_RELEASE "" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELEASE "" CACHE STRING "")

#
# Response file support
#
set(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)

set(CMAKE_C_RESPONSE_FILE_LINK_FLAG "@")
set(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")

set(CMAKE_NINJA_FORCE_RESPONSE_FILE 1 CACHE INTERNAL "")

#
# Find paths
#
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

#
# Output suffix
#
set(CMAKE_EXECUTABLE_SUFFIX .out)
set(CMAKE_EXECUTABLE_SUFFIX_C .out)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .out)

message(STATUS "Toolchain directory : ${TOOLCHAIN_DIR}")
message(STATUS "Commander          : ${POST_BUILD_EXE}")
message(STATUS "SLC                : ${SLC_EXE}")
message(STATUS "C Compiler         : ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler       : ${CMAKE_CXX_COMPILER}")