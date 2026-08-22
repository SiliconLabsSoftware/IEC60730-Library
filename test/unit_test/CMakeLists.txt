# unit test info
set(UNIT_TEST_INFO "unit_test_info")
#unit test clean
set(UNIT_TEST_CLEAN "unit_test_clean")

add_custom_target(${UNIT_TEST_INFO}
                  COMMAND echo "Components list: unit_test_iec60730_post "
                  " unit_test_iec60730_bist "
                  " unit_test_iec60730_program_counter "
                  " unit_test_iec60730_safety_check "
                  " unit_test_iec60730_irq "
                  " unit_test_iec60730_system_clock "
                  " unit_test_iec60730_watchdog "
                  " unit_test_iec60730_cpu_registers "
                  " unit_test_iec60730_variable_memory "
                  " unit_test_iec60730_invariable_memory\ "
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_custom_target(${UNIT_TEST_CLEAN}
                  COMMAND rm -rf ${CMAKE_BINARY_DIR}/test/unit_test/build/${BOARD_NAME}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
)

include(unit_test)

generate_unit_test(test/unit_test unit_test_iec60730_post sl_iec60730_post.c)
generate_unit_test(test/unit_test unit_test_iec60730_bist sl_iec60730_bist.c)
generate_unit_test(test/unit_test unit_test_iec60730_program_counter sl_iec60730_program_counter.c)
generate_unit_test(test/unit_test unit_test_iec60730_safety_check sl_iec60730_bist.c)
generate_unit_test(test/unit_test unit_test_iec60730_irq sl_iec60730_irq.c)
generate_unit_test(test/unit_test unit_test_iec60730_system_clock sl_iec60730_system_clock.c)
generate_unit_test(test/unit_test unit_test_iec60730_watchdog sl_iec60730_watchdog.c)
generate_unit_test(test/unit_test unit_test_iec60730_cpu_registers sl_iec60730_cpu_registers)
generate_unit_test(test/unit_test unit_test_iec60730_variable_memory sl_iec60730_variable_memory.c)
generate_unit_test(test/unit_test unit_test_iec60730_invariable_memory sl_iec60730_invariable_memory.c)
