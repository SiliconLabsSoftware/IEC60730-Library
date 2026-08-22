function(generate_unit_test relative_dir target_name source_file)
  set(FULL_DIR ${CMAKE_SOURCE_DIR}/${relative_dir})
  set(LINKER_PATH ${CMAKE_BINARY_DIR}/${SDK_LIB_CMAKE_RELATIVE_DIR}/${LIB_IEC60730_MAP})
  set(UNIT_TEST_NAME ${target_name})

  if(DEFINED ENV{TOOL_CHAINS})
      set(TOOL_CHAINS "$ENV{TOOL_CHAINS}")
  else()
      set(TOOL_CHAINS "GCC")
  endif()

  set(UNIT_TEST_NAME_BUILD_DIR "build/${BOARD_NAME}/${TOOL_CHAINS}/${target_name}")

	if(${target_name} STREQUAL "unit_test_iec60730_cpu_registers")
		set(UNIT_TEST_SOURCES
    	"${FULL_DIR}/src/main.c"
    	"${FULL_DIR}/src/app.c"
    	"${FULL_DIR}/src/unit_test_common.c"
    	"${FULL_DIR}/src/${target_name}.c"
    	"${FULL_DIR}/../../lib/src/${source_file}.c"
      "${FULL_DIR}/../../lib/src/${source_file}_ns.c"
      "${FULL_DIR}/../../lib/src/${source_file}_s.c"
	  )
  else()
    set(UNIT_TEST_SOURCES
      "${FULL_DIR}/src/main.c"
      "${FULL_DIR}/src/app.c"
      "${FULL_DIR}/src/unit_test_common.c"
      "${FULL_DIR}/src/${target_name}.c"
      "${FULL_DIR}/../../lib/src/${source_file}"
    )
  endif()
	string(TOUPPER ${UNIT_TEST_NAME} DEFINE_UNIT_TEST)
	set(UNIT_TEST_INCLUDES
    	"${FULL_DIR}/inc"
    	${UNITY_HEADER})

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

  if(${TOOL_CHAINS} STREQUAL "GCC")
    set(DEFINE_IAR_TESTING "GCC_TESTING")
  elseif(${TOOL_CHAINS} STREQUAL "IAR")
    set(DEFINE_IAR_TESTING "IAR_TESTING")
  endif()

  add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})
	target_include_directories(${UNIT_TEST_NAME} PUBLIC ${UNIT_TEST_INCLUDES})
  target_compile_definitions(${UNIT_TEST_NAME} PUBLIC ${DEFINE_UNIT_TEST}_ENABLE
                                                      ${DEFINE_ENABLE_CAL_CRC_32}
                                                      ${DEFINE_ENABLE_CRC_USE_SW}
                                                      ${DEFINE_ENABLE_SW_CRC_TABLE}
                                                      ${DEFINE_IAR_TESTING})

	target_link_libraries(${UNIT_TEST_NAME} PUBLIC unity slc_lib_iec60730)

	# Create .bin, .hex and .s37 artifacts after building the project
  if(${TOOL_CHAINS} STREQUAL "GCC")
	  add_custom_command(TARGET ${UNIT_TEST_NAME}
      	POST_BUILD
        COMMAND rm -rf ${UNIT_TEST_NAME_BUILD_DIR}
    	  COMMAND ${CMAKE_OBJCOPY} -O srec "$<TARGET_FILE:${UNIT_TEST_NAME}>" "$<TARGET_FILE_DIR:${UNIT_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${UNIT_TEST_NAME}>.s37"
    	  COMMAND ${CMAKE_OBJCOPY} -O ihex "$<TARGET_FILE:${UNIT_TEST_NAME}>" "$<TARGET_FILE_DIR:${UNIT_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${UNIT_TEST_NAME}>.hex"
    	  COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:${UNIT_TEST_NAME}>" "$<TARGET_FILE_DIR:${UNIT_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${UNIT_TEST_NAME}>.bin"
    	  COMMAND mkdir -p ${UNIT_TEST_NAME_BUILD_DIR}
    	  COMMAND mv ${UNIT_TEST_NAME}.out ${UNIT_TEST_NAME_BUILD_DIR}
    	  COMMAND mv ${UNIT_TEST_NAME}.bin ${UNIT_TEST_NAME_BUILD_DIR}
    	  COMMAND mv ${UNIT_TEST_NAME}.hex ${UNIT_TEST_NAME_BUILD_DIR}
    	  COMMAND mv ${UNIT_TEST_NAME}.s37 ${UNIT_TEST_NAME_BUILD_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${LINKER_PATH} ${UNIT_TEST_NAME_BUILD_DIR}
        COMMAND mv ${UNIT_TEST_NAME_BUILD_DIR}/${LIB_IEC60730_MAP} ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.map
	  )
  elseif (${TOOL_CHAINS} STREQUAL "IAR")
    add_custom_command(TARGET ${UNIT_TEST_NAME}
      POST_BUILD
      COMMAND rm -rf ${UNIT_TEST_NAME_BUILD_DIR}
      COMMAND ${CMAKE_OBJCOPY} --srec "$<TARGET_FILE:${UNIT_TEST_NAME}>" "$<TARGET_FILE_DIR:${UNIT_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${UNIT_TEST_NAME}>.s37"
      COMMAND ${CMAKE_OBJCOPY} --ihex "$<TARGET_FILE:${UNIT_TEST_NAME}>" "$<TARGET_FILE_DIR:${UNIT_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${UNIT_TEST_NAME}>.hex"
      COMMAND ${CMAKE_OBJCOPY} --bin "$<TARGET_FILE:${UNIT_TEST_NAME}>" "$<TARGET_FILE_DIR:${UNIT_TEST_NAME}>/$<TARGET_FILE_BASE_NAME:${UNIT_TEST_NAME}>.bin"
      COMMAND mkdir -p ${UNIT_TEST_NAME_BUILD_DIR}
      COMMAND mv ${UNIT_TEST_NAME}.out ${UNIT_TEST_NAME_BUILD_DIR}
      COMMAND mv ${UNIT_TEST_NAME}.bin ${UNIT_TEST_NAME_BUILD_DIR}
      COMMAND mv ${UNIT_TEST_NAME}.hex ${UNIT_TEST_NAME_BUILD_DIR}
      COMMAND mv ${UNIT_TEST_NAME}.s37 ${UNIT_TEST_NAME_BUILD_DIR}
      COMMAND ${CMAKE_COMMAND} -E copy ${LINKER_PATH} ${UNIT_TEST_NAME_BUILD_DIR}
      COMMAND mv ${UNIT_TEST_NAME_BUILD_DIR}/${LIB_IEC60730_MAP} ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.map
    )
  endif()

  if(${target_name} STREQUAL "unit_test_iec60730_invariable_memory")
    if(${TOOL_CHAINS} STREQUAL "GCC")
      if(ENABLE_CAL_CRC_32)
        set(post_build_command ${CMAKE_OBJDUMP} -t -h -d -S ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.out > ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.lst
        && bash ${CMAKE_SOURCE_DIR}/lib/crc/sl_iec60730_cal_crc32.sh '${CMAKE_CURRENT_BINARY_DIR}/${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}' '' 'C:/srecord/bin' '${TOOL_CHAINS}' "${FLASH_REGIONS_TEST}")
      else()
        set(post_build_command ${CMAKE_OBJDUMP} -t -h -d -S ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.out > ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.lst
        && bash ${CMAKE_SOURCE_DIR}/lib/crc/sl_iec60730_cal_crc16.sh '${CMAKE_CURRENT_BINARY_DIR}/${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}' '' 'C:/srecord/bin' '${TOOL_CHAINS}' "${FLASH_REGIONS_TEST}")
      endif()
    elseif (${TOOL_CHAINS} STREQUAL "IAR")
      if(ENABLE_CAL_CRC_32)
        set(post_build_command ${CMAKE_OBJDUMP} --all --source ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.out > ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.lst
        && bash ${CMAKE_SOURCE_DIR}/lib/crc/sl_iec60730_cal_crc32.sh '${CMAKE_CURRENT_BINARY_DIR}/${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}' '' 'C:/srecord/bin' '${TOOL_CHAINS}' "${FLASH_REGIONS_TEST}")
      else()
        set(post_build_command ${CMAKE_OBJDUMP} --all --source ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.out > ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.lst
        && bash ${CMAKE_SOURCE_DIR}/lib/crc/sl_iec60730_cal_crc16.sh '${CMAKE_CURRENT_BINARY_DIR}/${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}' '' 'C:/srecord/bin' '${TOOL_CHAINS}' "${FLASH_REGIONS_TEST}")
      endif()
    endif()
  else()
    if(${TOOL_CHAINS} STREQUAL "GCC")
	    set(post_build_command ${CMAKE_OBJDUMP} -t -h -d -S ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.out > ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.lst)
    elseif (${TOOL_CHAINS} STREQUAL "IAR")
    set(post_build_command ${CMAKE_OBJDUMP} --all --source ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.out > ${UNIT_TEST_NAME_BUILD_DIR}/${UNIT_TEST_NAME}.lst)
    endif()
  endif()
	# Run post-build pipeline to perform additional post-processing
	if(post_build_command)
	add_custom_command(TARGET ${UNIT_TEST_NAME}
    	POST_BUILD
    	COMMAND ${post_build_command}
	)
	endif()
endfunction(generate_unit_test)