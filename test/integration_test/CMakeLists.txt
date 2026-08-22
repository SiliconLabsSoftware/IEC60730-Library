# unit test info
set(INTEGRATION_TEST_INFO "integration_test_info")
#unit test clean
set(INTEGRATION_TEST_CLEAN "integration_test_clean")

add_custom_target(${INTEGRATION_TEST_INFO}
                  COMMAND echo "Components list: integration_test_iec60730_program_counter"
                  " integration_test_iec60730_irq "
                  " integration_test_iec60730_system_clock "
                  " integration_test_iec60730_watchdog "
                  " integration_test_iec60730_cpu_registers "
                  " integration_test_iec60730_variable_memory "
                  " integration_test_iec60730_invariable_memory\ "
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_custom_target(${INTEGRATION_TEST_CLEAN}
                  COMMAND rm -rf ${CMAKE_BINARY_DIR}/test/integration_test/build/${BOARD_NAME}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
)

include(integration_test)

generate_integration_test(test/integration_test integration_test_iec60730_program_counter)
generate_integration_test(test/integration_test integration_test_iec60730_irq)
generate_integration_test(test/integration_test integration_test_iec60730_system_clock)
generate_integration_test(test/integration_test integration_test_iec60730_watchdog)
generate_integration_test(test/integration_test integration_test_iec60730_cpu_registers)
generate_integration_test(test/integration_test integration_test_iec60730_variable_memory)
generate_integration_test(test/integration_test integration_test_iec60730_invariable_memory)
