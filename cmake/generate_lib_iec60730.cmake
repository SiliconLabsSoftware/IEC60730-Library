# choose toolchain
if(DEFINED ENV{TOOL_CHAINS})
  set(TOOL_CHAINS "$ENV{TOOL_CHAINS}")
else()
set(TOOL_CHAINS "GCC")
endif()

function(generate_lib_iec60730 relative_dir)

    set(FULL_DIR ${CMAKE_SOURCE_DIR}/${relative_dir})

    if(TEST_SECURE_PERIPHERALS_ENABLE AND ENABLE_INTEGRATION_TESTING)
      execute_process(COMMAND make prepare
        WORKING_DIRECTORY ${FULL_DIR})
      execute_process(COMMAND make vmc_support
        WORKING_DIRECTORY ${FULL_DIR})
    else()
      execute_process(COMMAND make prepare
        WORKING_DIRECTORY ${FULL_DIR})
    endif()

    if(${TOOL_CHAINS} STREQUAL "GCC")
      if(TEST_SECURE_PERIPHERALS_ENABLE AND ENABLE_INTEGRATION_TESTING)
        execute_process(COMMAND slc generate -p
          ${FULL_DIR}/lib_iec60730.slcp   -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730_vmc_support -o cmake --with ${BOARD_NAME}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
        execute_process(COMMAND slc generate -p
          ${FULL_DIR}/lib_iec60730_secure.slcp   -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730 -o cmake --with ${BOARD_NAME}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
      else()
        execute_process(COMMAND slc generate -p
          ${FULL_DIR}/lib_iec60730.slcp  -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730 -o cmake --with ${BOARD_NAME}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
      endif()
    elseif (${TOOL_CHAINS} STREQUAL "IAR")
      if(TEST_SECURE_PERIPHERALS_ENABLE AND ENABLE_INTEGRATION_TESTING)
        execute_process(COMMAND slc generate -p
          ${FULL_DIR}/lib_iec60730.slcp   -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730_vmc_support -o cmake --with ${BOARD_NAME}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
        execute_process(COMMAND slc generate -p
          ${FULL_DIR}/lib_iec60730_secure.slcp   -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730 -o cmake --with ${BOARD_NAME}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
        execute_process(COMMAND slc generate -p
          ${FULL_DIR}/lib_iec60730_secure.slcp   -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730 -o iar --with ${BOARD_NAME}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
      else()
        execute_process(COMMAND slc generate -p
          ${FULL_DIR}/lib_iec60730.slcp  -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730 -o cmake --with ${BOARD_NAME}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
        execute_process(COMMAND slc generate -p
          ${FULL_DIR}/lib_iec60730.slcp  -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730 -o iar --with ${BOARD_NAME}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
      endif()
    endif()

    if (EXISTS ${FULL_DIR}/CMakeLists.txt)
        message("-- [I] ${SDK_CMAKE_RELATIVE_DIR} path have CMake File: ${FULL_DIR}/CMakeLists.txt")
        add_subdirectory(${FULL_DIR})
    else()
        message("-- [I] ${SDK_CMAKE_RELATIVE_DIR} path don't have CMake File: ${FULL_DIR}")
    endif()

endfunction(generate_lib_iec60730)