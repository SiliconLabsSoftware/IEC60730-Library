/***************************************************************************//**
 * @file  unit_test_iec60730_bist.c
 * @brief Unit test function sl_iec60730_bist().
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
#include "unit_test_iec60730_bist.h"

/*=======Mock Code=====*/
static bool is_function_called = false;

__WEAK void sl_iec60730_safe_state(sl_iec60730_test_failure_t failure)
{
  (void)failure;
  is_function_called = true;
}

// Dump functions

__WEAK sl_iec60730_test_result_t sl_iec60730_vmc_bist(void)
{
  return cmock_return_value();
}

__WEAK sl_iec60730_test_result_t sl_iec60730_imc_bist(void)
{
  return cmock_return_value();
}

__WEAK sl_iec60730_test_result_t sl_iec60730_cpu_registers_bist(void)
{
  return cmock_return_value();
}

__WEAK void sl_iec60730_restart_watchdogs(void)
{
}

/*=======Test Case=====*/
void test_sl_iec60730_bist_pass_all_check_condition(void)
{
  /*Setup*/
  is_function_called = false;
  cmock_set_value(SL_IEC60730_TEST_PASSED);
  /*Execute test*/
  sl_iec60730_bist();
  TEST_ASSERT_EQUAL(false, is_function_called);
}

void test_sl_iec60730_bist_failed_check_condition(void)
{
  /*Setup*/
  is_function_called = false;
  cmock_set_value(SL_IEC60730_TEST_FAILED);
  /*Execute test*/
  sl_iec60730_bist();
  TEST_ASSERT_EQUAL(true, is_function_called);
}

void test_iec60730_safety_check_error_occur(void)
{
  /*Setup*/
  is_function_called = false;
  sl_iec60730_safety_check_error_occur(SL_IEC60730_CLOCK_FAIL);
  cmock_set_value(SL_IEC60730_TEST_PASSED);
  /*Execute test*/
  sl_iec60730_bist();
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
  UnityBegin("/unit_test/src/unit_test_iec60730_bist.c");
  run_test(test_sl_iec60730_bist_pass_all_check_condition, "test_sl_iec60730_bist_pass_all_check_condition", 54);
  run_test(test_sl_iec60730_bist_failed_check_condition, "test_sl_iec60730_bist_failed_check_condition", 64);
  run_test(test_iec60730_safety_check_error_occur, "test_iec60730_safety_check_error_occur", 74);
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
