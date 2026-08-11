
# Run coding_convention
include(coding_convention)

if(DEFINED ENV{TOOL_CHAINS})
    set(TOOL_CHAINS "$ENV{TOOL_CHAINS}")
else()
    set(TOOL_CHAINS "GCC")
endif()

if(CHECK_CODING_CONVETION_ENABLE)
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

endif()
