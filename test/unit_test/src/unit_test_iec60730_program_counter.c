/***************************************************************************//**
 * @file  unit_test_iec60730_program_counter.c
 * @brief Unit test function sl_iec60730_program_counter_test().
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
#include "unit_test_iec60730_program_counter.h"

/*=======Mock Code=====*/

extern uint16_t sl_iec60730_program_counter_check;
static bool is_function_called = false;

// Dump functions

__WEAK void sl_iec60730_safety_check_error_occur(sl_iec60730_test_failure_t failure)
{
  (void)failure;
  is_function_called = true;
}

__WEAK void sl_iec60730_safe_state(sl_iec60730_test_failure_t failure)
{
  (void)failure;
}

/*=======Test Case=====*/
void test_sl_iec60730_program_counter_complete_all_bit_check(void)
{
  /*Setup*/
  is_function_called = false;
  sl_iec60730_program_counter_check = IEC60730_ALL_COMPLETE_BITS;
  /*Execute test*/
  sl_iec60730_program_counter_test();
  TEST_ASSERT_EQUAL(false, is_function_called);
}

void test_sl_iec60730_program_counter_fail_some_bit_check(void)
{
  /*Setup*/
  is_function_called = false;
  sl_iec60730_program_counter_check = (IEC60730_ALL_COMPLETE_BITS && (~(IEC60730_INTERRUPT_COMPLETE)));
  /*Execute test*/
  sl_iec60730_program_counter_test();
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
  UnityBegin("/unit_test/src/unit_test_iec60730_program_counter.c");
  run_test(test_sl_iec60730_program_counter_complete_all_bit_check, "test_sl_iec60730_program_counter_complete_all_bit_check", 40);
  run_test(test_sl_iec60730_program_counter_fail_some_bit_check, "test_sl_iec60730_program_counter_fail_some_bit_check", 50);
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
