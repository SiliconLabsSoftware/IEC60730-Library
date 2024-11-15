function(generate_coding_convention relative_dir)

  set(FULL_DIR ${CMAKE_SOURCE_DIR}/${relative_dir})

  if(DEFINED ENV{TOOL_CHAINS})
    set(TOOL_CHAINS "$ENV{TOOL_CHAINS}")
  else()
    set(TOOL_CHAINS "GCC")
  endif()
  execute_process(COMMAND make prepare
  WORKING_DIRECTORY ${FULL_DIR})

  if(${TOOL_CHAINS} STREQUAL "GCC")
    execute_process(COMMAND slc generate -p
      ${FULL_DIR}/lib_iec60730_coding_convention.slcp  -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730 -o cmake --with ${BOARD_NAME}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  elseif (${TOOL_CHAINS} STREQUAL "IAR")
    execute_process(COMMAND slc generate -p
      ${FULL_DIR}/lib_iec60730_coding_convention.slcp  -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730 -o cmake --with ${BOARD_NAME}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    execute_process(COMMAND slc generate -p
      ${FULL_DIR}/lib_iec60730_coding_convention.slcp  -np -d ${FULL_DIR}/src -cpproj -name=lib_iec60730 -o iar --with ${BOARD_NAME}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  endif()

  if (EXISTS ${FULL_DIR}/CMakeLists.txt)
    message("-- [I] ${SDK_CMAKE_RELATIVE_DIR} path have CMake File: ${FULL_DIR}/CMakeLists.txt")
    add_subdirectory(${FULL_DIR})
  else()
    message("-- [I] ${SDK_CMAKE_RELATIVE_DIR} path don't have CMake File: ${FULL_DIR}")
  endif()

  set(LINKER_PATH ${CMAKE_BINARY_DIR}/${SDK_LIB_CMAKE_RELATIVE_DIR}/${LIB_IEC60730_MAP})
  set(CODING_CONVENTION "coding_convention")
  set(CODING_CONVENTION_BUILD_DIR "build/${BOARD_NAME}/${TOOL_CHAINS}/${CODING_CONVENTION}")

  if(${TOOL_CHAINS} STREQUAL "GCC")
    set(SOURCE_LIBIEC60730
      "${CMAKE_SOURCE_DIR}/${SDK_CMAKE_RELATIVE_DIR}/app.c"
      "${CMAKE_SOURCE_DIR}/lib/asm/sl_iec60730_vmc_marchc.S"
      "${CMAKE_SOURCE_DIR}/lib/asm/sl_iec60730_vmc_marchxc.S"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_bist.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_cpu_registers.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_cpu_registers_ns.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_cpu_registers_s.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_invariable_memory.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_irq.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_post.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_program_counter.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_system_clock.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_variable_memory.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_watchdog.c"
      "${CMAKE_SOURCE_DIR}/${SDK_CMAKE_RELATIVE_DIR}/main.c"
    )
  elseif(${TOOL_CHAINS} STREQUAL "IAR")
    set(SOURCE_LIBIEC60730
      "${CMAKE_SOURCE_DIR}/${SDK_CMAKE_RELATIVE_DIR}/app.c"
      "${CMAKE_SOURCE_DIR}/lib/asm/sl_iec60730_vmc_marchc_iar.S"
      "${CMAKE_SOURCE_DIR}/lib/asm/sl_iec60730_vmc_marchxc_iar.S"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_bist.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_cpu_registers.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_cpu_registers_ns.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_cpu_registers_s.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_invariable_memory.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_irq.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_post.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_program_counter.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_system_clock.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_variable_memory.c"
      "${CMAKE_SOURCE_DIR}/lib/src/sl_iec60730_watchdog.c"
      "${CMAKE_SOURCE_DIR}/${SDK_CMAKE_RELATIVE_DIR}/main.c"
    )
  endif()

  set(HEADER_LIBIEC60730 PUBLIC
    "${CMAKE_SOURCE_DIR}/lib/config/sl_iec60730_config.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_toolchain.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_board.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_comm.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_cpu_registers.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_internal.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_invariable_memory.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_irq.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_program_counter.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_system_clock.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_variable_memory.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730_watchdog.h"
    "${CMAKE_SOURCE_DIR}/lib/inc/sl_iec60730.h"
    "${CMAKE_SOURCE_DIR}/lib/lib/asm"
    "${CMAKE_SOURCE_DIR}/${SDK_CMAKE_RELATIVE_DIR}"
  )


  set(CODING_SOURCES
    ${SOURCE_LIBIEC60730}
  )
	set(CODING_INCLUDE
    	${HEADER_LIBIEC60730})
  add_executable(${CODING_CONVENTION} "${CODING_SOURCES}")
  target_include_directories(${CODING_CONVENTION} PUBLIC
  "${CMAKE_SOURCE_DIR}/lib/config/"
  "${CMAKE_SOURCE_DIR}/lib/inc/"
  "${CODING_INCLUDE}"
  )


	target_link_libraries(${CODING_CONVENTION} PUBLIC slc_lib_iec60730)

	# Create .bin, .hex and .s37 artifacts after building the project
  if(${TOOL_CHAINS} STREQUAL "GCC")
	  add_custom_command(TARGET ${CODING_CONVENTION}
      	POST_BUILD
        COMMAND rm -rf ${CODING_CONVENTION_BUILD_DIR}
    	  COMMAND ${CMAKE_OBJCOPY} -O srec "$<TARGET_FILE:${CODING_CONVENTION}>" "$<TARGET_FILE_DIR:${CODING_CONVENTION}>/$<TARGET_FILE_BASE_NAME:${CODING_CONVENTION}>.s37"
    	  COMMAND ${CMAKE_OBJCOPY} -O ihex "$<TARGET_FILE:${CODING_CONVENTION}>" "$<TARGET_FILE_DIR:${CODING_CONVENTION}>/$<TARGET_FILE_BASE_NAME:${CODING_CONVENTION}>.hex"
    	  COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:${CODING_CONVENTION}>" "$<TARGET_FILE_DIR:${CODING_CONVENTION}>/$<TARGET_FILE_BASE_NAME:${CODING_CONVENTION}>.bin"
    	  COMMAND mkdir -p ${CODING_CONVENTION_BUILD_DIR}
    	  COMMAND mv ${CODING_CONVENTION}.out ${CODING_CONVENTION_BUILD_DIR}
    	  COMMAND mv ${CODING_CONVENTION}.bin ${CODING_CONVENTION_BUILD_DIR}
    	  COMMAND mv ${CODING_CONVENTION}.hex ${CODING_CONVENTION_BUILD_DIR}
    	  COMMAND mv ${CODING_CONVENTION}.s37 ${CODING_CONVENTION_BUILD_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${LINKER_PATH} ${CODING_CONVENTION_BUILD_DIR}
        COMMAND mv ${CODING_CONVENTION_BUILD_DIR}/${LIB_IEC60730_MAP} ${CODING_CONVENTION_BUILD_DIR}/${CODING_CONVENTION}.map
	  )
  elseif (${TOOL_CHAINS} STREQUAL "IAR")
    add_custom_command(TARGET ${CODING_CONVENTION}
      POST_BUILD
      COMMAND rm -rf ${CODING_CONVENTION_BUILD_DIR}
      COMMAND ${CMAKE_OBJCOPY} --srec "$<TARGET_FILE:${CODING_CONVENTION}>" "$<TARGET_FILE_DIR:${CODING_CONVENTION}>/$<TARGET_FILE_BASE_NAME:${CODING_CONVENTION}>.s37"
      COMMAND ${CMAKE_OBJCOPY} --ihex "$<TARGET_FILE:${CODING_CONVENTION}>" "$<TARGET_FILE_DIR:${CODING_CONVENTION}>/$<TARGET_FILE_BASE_NAME:${CODING_CONVENTION}>.hex"
      COMMAND ${CMAKE_OBJCOPY} --bin "$<TARGET_FILE:${CODING_CONVENTION}>" "$<TARGET_FILE_DIR:${CODING_CONVENTION}>/$<TARGET_FILE_BASE_NAME:${CODING_CONVENTION}>.bin"
      COMMAND mkdir -p ${CODING_CONVENTION_BUILD_DIR}
      COMMAND mv ${CODING_CONVENTION}.out ${CODING_CONVENTION_BUILD_DIR}
      COMMAND mv ${CODING_CONVENTION}.bin ${CODING_CONVENTION_BUILD_DIR}
      COMMAND mv ${CODING_CONVENTION}.hex ${CODING_CONVENTION_BUILD_DIR}
      COMMAND mv ${CODING_CONVENTION}.s37 ${CODING_CONVENTION_BUILD_DIR}
      COMMAND ${CMAKE_COMMAND} -E copy ${LINKER_PATH} ${CODING_CONVENTION_BUILD_DIR}
      COMMAND mv ${CODING_CONVENTION_BUILD_DIR}/${LIB_IEC60730_MAP} ${CODING_CONVENTION_BUILD_DIR}/${CODING_CONVENTION}.map
    )
  endif()

	# Run post-build pipeline to perform additional post-processing
	if(post_build_command)
	  add_custom_command(TARGET ${CODING_CONVENTION}
    	POST_BUILD
    	COMMAND ${post_build_command}
	  )
	endif()
endfunction(generate_coding_convention)