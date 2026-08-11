function(generate_integration_test relative_dir target_name)
  set(FULL_DIR ${CMAKE_SOURCE_DIR}/${relative_dir})
  set(LINKER_PATH ${CMAKE_BINARY_DIR}/${SDK_LIB_CMAKE_RELATIVE_DIR}/${LIB_IEC60730_MAP})
  set(INTEGRATION_TEST_NAME ${target_name})
  string(REPLACE "integration_test_" "" INTEGRATION_SOURCE_NAME "${INTEGRATION_TEST_NAME}")

  if(DEFINED ENV{TOOL_CHAINS})
      set(TOOL_CHAINS "$ENV{TOOL_CHAINS}")
  else()
      set(TOOL_CHAINS "GCC")
  endif()

  # enable test peripherals non-secure test
  if(TEST_SECURE_PERIPHERALS_ENABLE)
    set(INTEGRATION_TEST_NAME_BUILD_DIR "build/${BOARD_NAME}/${TOOL_CHAINS}/${target_name}/S")
    if(${target_name} STREQUAL "integration_test_iec60730_variable_memory")
      set(DEFINE_TEST_SECURE_PERIPHERALS_ENABLE "SL_IEC60730_NON_SECURE_ENABLE")
      set(DEFINE_SL_TRUSTZONE_SECURE_ENABLE "SL_TRUSTZONE_NON_SECURE")
    else()
      set(DEFINE_TEST_SECURE_PERIPHERALS_ENABLE "SL_IEC60730_SECURE_ENABLE")
      set(DEFINE_SL_TRUSTZONE_SECURE_ENABLE "SL_TRUSTZONE_SECURE")
    endif()
  else()
    set(INTEGRATION_TEST_NAME_BUILD_DIR "build/${BOARD_NAME}/${TOOL_CHAINS}/${target_name}/NS")
    set(DEFINE_TEST_SECURE_PERIPHERALS_ENABLE "SL_IEC60730_NON_SECURE_ENABLE")
    set(DEFINE_SL_TRUSTZONE_SECURE_ENABLE "SL_TRUSTZONE_NON_SECURE")
  endif()

  # enable integration test watchdog 1
  if(INTEGRATION_TEST_WDOG1_ENABLE)
    set(DEFINE_SL_IEC60730_WDOG1_ENABLE "SL_IEC60730_WDOG1_ENABLE=1")
  else()
    set(DEFINE_SL_IEC60730_WDOG1_ENABLE "SL_IEC60730_WDOG1_ENABLE=0")
  endif()

  # disable using MarchXC algorithm
  if(INTEGRATION_TEST_USE_MARCHX_DISABLE)
    set(DEFINE_SL_IEC60730_USE_MARCHX_ENABLE "SL_IEC60730_USE_MARCHX_ENABLE=0")
  else()
    set(DEFINE_SL_IEC60730_USE_MARCHX_ENABLE "SL_IEC60730_USE_MARCHX_ENABLE=1")
  endif()


	if(${target_name} STREQUAL "integration_test_iec60730_cpu_registers")
		set(INTEGRATION_TEST_SOURCES
    	"${FULL_DIR}/src/main.c"
    	"${FULL_DIR}/src/app.c"
    	"${FULL_DIR}/src/integration_test_common.c"
    	"${FULL_DIR}/src/${target_name}.c"
    	"${FULL_DIR}/../../lib/src/sl_iec60730_cpu_registers.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_cpu_registers_ns.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_cpu_registers_s.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_post.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_bist.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_watchdog.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_program_counter.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_system_clock.c"
	  )
  elseif(${target_name} STREQUAL "integration_test_iec60730_variable_memory")
    if(${TOOL_CHAINS} STREQUAL "GCC")
      set(INTEGRATION_TEST_SOURCES
        "${FULL_DIR}/src/main.c"
        "${FULL_DIR}/src/app.c"
        "${FULL_DIR}/src/integration_test_common.c"
        "${FULL_DIR}/src/${target_name}.c"
        "${FULL_DIR}/../../lib/src/sl_${INTEGRATION_SOURCE_NAME}.c"
        "${FULL_DIR}/../../lib/asm/sl_iec60730_vmc_marchc.S"
        "${FULL_DIR}/../../lib/asm/sl_iec60730_vmc_marchxc.S"
        "${FULL_DIR}/../../lib/src/sl_iec60730_post.c"
        "${FULL_DIR}/../../lib/src/sl_iec60730_bist.c"
        "${FULL_DIR}/../../lib/src/sl_iec60730_watchdog.c"
        "${FULL_DIR}/../../lib/src/sl_iec60730_program_counter.c"
        "${FULL_DIR}/../../lib/src/sl_iec60730_system_clock.c"
      )
    elseif(${TOOL_CHAINS} STREQUAL "IAR")
      set(INTEGRATION_TEST_SOURCES
        "${FULL_DIR}/src/main.c"
        "${FULL_DIR}/src/app.c"
        "${FULL_DIR}/src/integration_test_common.c"
        "${FULL_DIR}/src/${target_name}.c"
        "${FULL_DIR}/../../lib/src/sl_${INTEGRATION_SOURCE_NAME}.c"
        "${FULL_DIR}/../../lib/asm/sl_iec60730_vmc_marchc_iar.S"
        "${FULL_DIR}/../../lib/asm/sl_iec60730_vmc_marchxc_iar.S"
        "${FULL_DIR}/../../lib/src/sl_iec60730_post.c"
        "${FULL_DIR}/../../lib/src/sl_iec60730_bist.c"
        "${FULL_DIR}/../../lib/src/sl_iec60730_watchdog.c"
        "${FULL_DIR}/../../lib/src/sl_iec60730_program_counter.c"
        "${FULL_DIR}/../../lib/src/sl_iec60730_system_clock.c"
      )
    endif()
  else()
    set(INTEGRATION_TEST_SOURCES
      "${FULL_DIR}/src/main.c"
      "${FULL_DIR}/src/app.c"
      "${FULL_DIR}/src/integration_test_common.c"
      "${FULL_DIR}/src/${target_name}.c"
      "${FULL_DIR}/../../lib/src/sl_${INTEGRATION_SOURCE_NAME}.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_post.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_bist.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_watchdog.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_program_counter.c"
      "${FULL_DIR}/../../lib/src/sl_iec60730_system_clock.c"
    )
  endif()

	string(TOUPPER ${INTEGRATION_TEST_NAME} DEFINE_INTEGRATION_TEST)
	set(INTEGRATION_TEST_INCLUDES
    "${FULL_DIR}/inc")

  if(${TOOL_CHAINS} STREQUAL "GCC")
    set(DEFINE_IAR_TESTING "GCC_TESTING")
  elseif(${TOOL_CHAINS} STREQUAL "IAR")
    set(DEFINE_IAR_TESTING "IAR_TESTING")
  endif()

  # Set address start calculate crc
  if(DEFINED ENV{FLASH_REGIONS_TEST})
    set(FLASH_REGIONS_TEST "$ENV{FLASH_REGIONS_TEST}")
  else()
    set(FLASH_REGIONS_TEST "0x00000000")
  endif()

  if(ENABLE_CAL_CRC_32)
    set(DEFINE_ENABLE_CAL_CRC_32 "SL_IEC60730_USE_CRC_32_ENABLE=1")
  else()
    set(DEFINE_ENABLE_CAL_CRC_32 "SL_IEC60730_USE_CRC_32_ENABLE=0")
  endif()

  if(ENABLE_CRC_USE_SW)
    set(DEFINE_ENABLE_CRC_USE_SW "SL_IEC60730_CRC_USE_SW_ENABLE=1")
    if(ENABLE_SW_CRC_TABLE)
      set(DEFINE_ENABLE_SW_CRC_TABLE "SL_IEC60730_SW_CRC_TABLE_ENABLE=1")
    else() # ENABLE_SW_CRC_TABLE
      set(DEFINE_ENABLE_SW_CRC_TABLE "SL_IEC60730_SW_CRC_TABLE_ENABLE=0")
    endif() # ENABLE_SW_CRC_TABLE
  else() # ENABLE_CRC_USE_SW
    set(DEFINE_ENABLE_CRC_USE_SW "SL_IEC60730_CRC_USE_SW_ENABLE=0")
  endif() # ENABLE_CRC_USE_SW

	add_executable(${INTEGRATION_TEST_NAME} ${INTEGRATION_TEST_SOURCES})
	target_include_directories(${INTEGRATION_TEST_NAME} PUBLIC ${INTEGRATION_TEST_INCLUDES})
  target_compile_definitions(${INTEGRATION_TEST_NAME} PUBLIC ${DEFINE_INTEGRATION_TEST}_ENABLE
                                                      ${DEFINE_ENABLE_CAL_CRC_32}
                                                      ${DEFINE_ENABLE_CRC_USE_SW}
                                                      ${DEFINE_ENABLE_SW_CRC_TABLE}
                                                      ${DEFINE_TEST_SECURE_PERIPHERALS_ENABLE}
                                                      ${DEFINE_SL_TRUSTZONE_SECURE_ENABLE}
                                                      ${DEFINE_SL_IEC60730_WDOG1_ENABLE}
                                                      ${DEFINE_SL_IEC60730_USE_MARCHX_ENABLE}
                                                      ${DEFINE_IAR_TESTING})
  if(TEST_SECURE_PERIPHERALS_ENABLE AND (${target_name} STREQUAL "integration_test_iec60730_variable_memory"))
	  target_link_libraries(${INTEGRATION_TEST_NAME} PUBLIC slc_lib_iec60730_vmc_support)
  else()
    target_link_libraries(${INTEGRATION_TEST_NAME} PUBLIC slc_lib_iec60730)
  endif()

	# Create .bin, .hex and .s37 artifacts after building the project
  if(${TOOL_CHAINS} STREQUAL "GCC")
	  add_custom_command(TARGET ${INTEGRATION_TEST_NAME}
        POST_BUILD
        COMMAND rm -rf ${INTEGRATION_TEST_NAME_BUILD_DIR}
    	  COMMAND ${CMAKE_OBJCOPY} -O srec "$<TARGET_FILE:${INTEGRATION_TEST_NAME}>" "$<TARGET_FILE_DIR:${INTEGRATION_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${INTEGRATION_TEST_NAME}>.s37"
    	  COMMAND ${CMAKE_OBJCOPY} -O ihex "$<TARGET_FILE:${INTEGRATION_TEST_NAME}>" "$<TARGET_FILE_DIR:${INTEGRATION_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${INTEGRATION_TEST_NAME}>.hex"
    	  COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:${INTEGRATION_TEST_NAME}>" "$<TARGET_FILE_DIR:${INTEGRATION_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${INTEGRATION_TEST_NAME}>.bin"
    	  COMMAND mkdir -p ${INTEGRATION_TEST_NAME_BUILD_DIR}
    	  COMMAND mv ${INTEGRATION_TEST_NAME}.out ${INTEGRATION_TEST_NAME_BUILD_DIR}
    	  COMMAND mv ${INTEGRATION_TEST_NAME}.bin ${INTEGRATION_TEST_NAME_BUILD_DIR}
    	  COMMAND mv ${INTEGRATION_TEST_NAME}.hex ${INTEGRATION_TEST_NAME_BUILD_DIR}
    	  COMMAND mv ${INTEGRATION_TEST_NAME}.s37 ${INTEGRATION_TEST_NAME_BUILD_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${LINKER_PATH} ${INTEGRATION_TEST_NAME_BUILD_DIR}
        COMMAND mv ${INTEGRATION_TEST_NAME_BUILD_DIR}/${LIB_IEC60730_MAP} ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.map
	  )
  elseif (${TOOL_CHAINS} STREQUAL "IAR")
    add_custom_command(TARGET ${INTEGRATION_TEST_NAME}
          POST_BUILD
          COMMAND rm -rf ${INTEGRATION_TEST_NAME_BUILD_DIR}
          COMMAND ${CMAKE_OBJCOPY} --srec "$<TARGET_FILE:${INTEGRATION_TEST_NAME}>" "$<TARGET_FILE_DIR:${INTEGRATION_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${INTEGRATION_TEST_NAME}>.s37"
          COMMAND ${CMAKE_OBJCOPY} --ihex "$<TARGET_FILE:${INTEGRATION_TEST_NAME}>" "$<TARGET_FILE_DIR:${INTEGRATION_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${INTEGRATION_TEST_NAME}>.hex"
          COMMAND ${CMAKE_OBJCOPY} --bin "$<TARGET_FILE:${INTEGRATION_TEST_NAME}>" "$<TARGET_FILE_DIR:${INTEGRATION_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${INTEGRATION_TEST_NAME}>.bin"
          COMMAND mkdir -p ${INTEGRATION_TEST_NAME_BUILD_DIR}
          COMMAND mv ${INTEGRATION_TEST_NAME}.out ${INTEGRATION_TEST_NAME_BUILD_DIR}
          COMMAND mv ${INTEGRATION_TEST_NAME}.bin ${INTEGRATION_TEST_NAME_BUILD_DIR}
          COMMAND mv ${INTEGRATION_TEST_NAME}.hex ${INTEGRATION_TEST_NAME_BUILD_DIR}
          COMMAND mv ${INTEGRATION_TEST_NAME}.s37 ${INTEGRATION_TEST_NAME_BUILD_DIR}
          COMMAND ${CMAKE_COMMAND} -E copy ${LINKER_PATH} ${INTEGRATION_TEST_NAME_BUILD_DIR}
          COMMAND mv ${INTEGRATION_TEST_NAME_BUILD_DIR}/${LIB_IEC60730_MAP} ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.map
    )
  endif()

  if(${target_name} STREQUAL "integration_test_iec60730_invariable_memory")
    if(${TOOL_CHAINS} STREQUAL "GCC")
      if(ENABLE_CAL_CRC_32)
        set(post_build_command ${CMAKE_OBJDUMP} -t -h -d -S ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.out > ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.lst
        && bash ${CMAKE_SOURCE_DIR}/lib/crc/sl_iec60730_cal_crc32.sh '${CMAKE_CURRENT_BINARY_DIR}/${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}' '' 'C:/srecord/bin' '${TOOL_CHAINS}' "${FLASH_REGIONS_TEST}")
      else()
        set(post_build_command ${CMAKE_OBJDUMP} -t -h -d -S ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.out > ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.lst
        && bash ${CMAKE_SOURCE_DIR}/lib/crc/sl_iec60730_cal_crc16.sh '${CMAKE_CURRENT_BINARY_DIR}/${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}' '' 'C:/srecord/bin' '${TOOL_CHAINS}' "${FLASH_REGIONS_TEST}")
      endif()
    elseif (${TOOL_CHAINS} STREQUAL "IAR")
      if(ENABLE_CAL_CRC_32)
        set(post_build_command ${CMAKE_OBJDUMP} --all --source ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.out > ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.lst
        && bash ${CMAKE_SOURCE_DIR}/lib/crc/sl_iec60730_cal_crc32.sh '${CMAKE_CURRENT_BINARY_DIR}/${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}' '' 'C:/srecord/bin' '${TOOL_CHAINS}' "${FLASH_REGIONS_TEST}")
      else()
        set(post_build_command ${CMAKE_OBJDUMP} --all --source ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.out > ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.lst
        && bash ${CMAKE_SOURCE_DIR}/lib/crc/sl_iec60730_cal_crc16.sh '${CMAKE_CURRENT_BINARY_DIR}/${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}' '' 'C:/srecord/bin' '${TOOL_CHAINS}' "${FLASH_REGIONS_TEST}")
      endif()
    endif()
  else()
    if(${TOOL_CHAINS} STREQUAL "GCC")
	    set(post_build_command ${CMAKE_OBJDUMP} -t -h -d -S ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.out > ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.lst)
    elseif (${TOOL_CHAINS} STREQUAL "IAR")
      set(post_build_command ${CMAKE_OBJDUMP} --all --source ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.out > ${INTEGRATION_TEST_NAME_BUILD_DIR}/${INTEGRATION_TEST_NAME}.lst)
    endif()
  endif()
	# Run post-build pipeline to perform additional post-processing
	if(post_build_command)
	add_custom_command(TARGET ${INTEGRATION_TEST_NAME}
    	POST_BUILD
    	COMMAND ${post_build_command}
	)
	endif()
endfunction(generate_integration_test)