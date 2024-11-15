/***************************************************************************//**
 * @file  unit_test_iec60730_post.c
 * @brief Unit test function sl_iec60730_post().
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
#include "unit_test_iec60730_post.h"

/*=======Mock Code=====*/
uint8_t iec60730_timer_test_control = SL_IEC60730_TIMER_TEST_ENABLE;
static bool is_function_called = false;

__WEAK void sl_iec60730_safe_state(sl_iec60730_test_failure_t failure)
{
  (void)failure;
  is_function_called = true;
}

// Dump functions

__WEAK sl_iec60730_test_result_t sl_iec60730_vmc_post(void)
{
  return cmock_return_value();
}

__WEAK sl_iec60730_test_result_t sl_iec60730_imc_post(void)
{
  return cmock_return_value();
}

__WEAK sl_iec60730_test_result_t sl_iec60730_cpu_registers_post(void)
{
  return cmock_return_value();
}

__WEAK sl_iec60730_test_result_t sl_iec60730_watchdog_post(void)
{
  return cmock_return_value();
}

__WEAK void sl_iec60730_safety_check_error_occur(sl_iec60730_test_failure_t failure)
{
  (void) failure;
}

__WEAK void sl_iec60730_restart_watchdogs(void)
{
}

__WEAK void sl_iec60730_sys_clock_test_enable(void)
{
}

/*=======Test Case=====*/
void test_sl_iec60730_post_pass_all_check_condition(void)
{
  /*Setup*/
  is_function_called = false;
  cmock_set_value(SL_IEC60730_TEST_PASSED);
  /*Execute test*/
  sl_iec60730_post();
  TEST_ASSERT_EQUAL(false, is_function_called);
}

void test_sl_iec60730_post_failed_check_condition(void)
{
  /*Setup*/
  is_function_called = false;
  cmock_set_value(SL_IEC60730_TEST_FAILED);
  /*Execute test*/
  sl_iec60730_post();
  TEST_ASSERT_EQUAL(true, is_function_called);
}

/*=======Run Test Case=====*/

void unit_test_run_all_test_cases(void)
{
  // Initialize
  memset((void *)test_case_result, 0xff, sizeof(test_case_result));
  current_test_numth = 0;

  // Start run test
  printf("--- BEGIN UNIT TEST ---\n");
  UnityBegin("/unit_test/src/unit_test_iec60730_post.c");
  run_test(test_sl_iec60730_post_pass_all_check_condition, "test_sl_iec60730_post_pass_all_check_condition", 68);
  run_test(test_sl_iec60730_post_failed_check_condition, "test_sl_iec60730_post_failed_check_condition", 78);
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
