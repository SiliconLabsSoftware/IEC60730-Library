/***************************************************************************//**
 * @file  unit_test_iec60730_system_clock.c
 * @brief Unit tests for functions in system_clock module.
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

#include "unit_test_common.h"
#include "unit_test_iec60730_system_clock.h"

/*=======Set up=====*/
uint16_t sl_iec60730_program_counter_check = 0;
static bool is_function_called = false;
uint16_t count_sl_iec60730_test_clock_tick_called = 0;

/*=======Mock Code=====*/

__WEAK void sl_iec60730_safe_state(sl_iec60730_test_failure_t failure)
{
  (void)failure;
  is_function_called = true;
}

__WEAK void sl_iec60730_safety_check_error_occur(sl_iec60730_test_failure_t failure)
{
  (void) failure;
}

__WEAK void sl_iec60730_irq_check(void)
{
}

__WEAK void sl_iec60730_program_counter_test(void)
{
  is_function_called = true;
}

/*=======Test Case=====*/
void test_sl_iec60730_test_clock_tick_timer_test_disable(void)
{
  /*Setup*/
  is_function_called = false;
  sl_iec60730_sys_clock_test_disable();
  /*Execute test*/
  sl_iec60730_test_clock_tick();
  count_sl_iec60730_test_clock_tick_called++;
  TEST_ASSERT_EQUAL(true, is_function_called);
}

void test_sl_iec60730_test_clock_tick_count_in_bound(void)
{
  /*Setup*/
  uint8_t index;
  is_function_called = false;
  sl_iec60730_sys_clock_test_enable();
  for (index = 0; index < (SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ + SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE); index++) {
    sl_iec60730_sys_clock_count_tick();
  }
  /*Execute test*/
  sl_iec60730_test_clock_tick();
  count_sl_iec60730_test_clock_tick_called++;
  TEST_ASSERT_EQUAL(false, is_function_called);
}

void test_sl_iec60730_test_clock_tick_count_out_bound(void)
{
  /*Setup*/
  uint8_t index;
  is_function_called = false;
  sl_iec60730_sys_clock_test_enable();
  for (index = 0; index <= (SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ + SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE + 1); index++) {
    sl_iec60730_sys_clock_count_tick();
  }
  /*Execute test*/
  sl_iec60730_test_clock_tick();
  count_sl_iec60730_test_clock_tick_called++;
  TEST_ASSERT_EQUAL(true, is_function_called);
}

void test_sl_iec60730_sys_clock_count_reset(void)
{
  /*Setup*/
  uint8_t index;
  is_function_called = false;
  sl_iec60730_sys_clock_test_enable();
  for (index = 0; index < (SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ + SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE); index++) {
    sl_iec60730_sys_clock_count_tick();
  }
  /*Execute test*/
  sl_iec60730_sys_clock_count_reset();
  sl_iec60730_test_clock_tick();
  count_sl_iec60730_test_clock_tick_called++;
  TEST_ASSERT_EQUAL(true, is_function_called);
}

void test_sl_iec60730_test_clock_tick_test_clock_multiplier(void)
{
  /*Setup*/
  uint8_t index;
  is_function_called = false;
  sl_iec60730_sys_clock_test_enable();
  /*Execute test*/
  while (count_sl_iec60730_test_clock_tick_called <= SL_IEC60730_TEST_CLOCK_MULTIPLIER + 1) {
    for (index = 0; index < (SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ + SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE); index++) {
      sl_iec60730_sys_clock_count_tick();
    }
    sl_iec60730_test_clock_tick();
    count_sl_iec60730_test_clock_tick_called++;
  }
  TEST_ASSERT_EQUAL(true, is_function_called);
}

void test_sl_iec60730_get_number_test_timer_tick(void)
{
  /*Setup*/
  uint16_t unit_test_get_number_test_timer_tick;
  /*Execute test*/
  unit_test_get_number_test_timer_tick = sl_iec60730_get_number_test_timer_tick();
  TEST_ASSERT_EQUAL(count_sl_iec60730_test_clock_tick_called, unit_test_get_number_test_timer_tick);
}

/*=======Run Test Case=====*/

void unit_test_run_all_test_cases(void)
{
  // Initialize
  memset((void *)test_case_result, 0xff, sizeof(test_case_result));
  current_test_numth = 0;

  // Start run test
  printf("--- BEGIN UNIT TEST ---\n");
  UnityBegin("/unit_test/src/unit_test_iec60730_system_clock.c");
  run_test(test_sl_iec60730_test_clock_tick_timer_test_disable, "test_sl_iec60730_test_clock_tick_timer_test_disable", 46);
  run_test(test_sl_iec60730_test_clock_tick_count_in_bound, "test_sl_iec60730_test_clock_tick_count_in_bound", 57);
  run_test(test_sl_iec60730_test_clock_tick_count_out_bound, "test_sl_iec60730_test_clock_tick_count_out_bound", 72);
  run_test(test_sl_iec60730_sys_clock_count_reset, "test_sl_iec60730_sys_clock_count_reset", 87);
  run_test(test_sl_iec60730_test_clock_tick_test_clock_multiplier, "test_sl_iec60730_test_clock_tick_test_clock_multiplier", 103);
  run_test(test_sl_iec60730_get_number_test_timer_tick, "test_sl_iec60730_get_number_test_timer_tick", 120);
  UnityEnd();
  #ifndef IAR_TESTING  /* GCC */
  __asm volatile("IEC60730_UNIT_TEST_END:");
#else
  __asm volatile("IEC60730_UNIT_TEST_END::");
#endif

  while (1) {
    // Do nothing
  }
}
