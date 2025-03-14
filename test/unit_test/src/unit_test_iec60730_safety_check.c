/***************************************************************************//**
 * @file  unit_test_iec60730_safety_check.c
 * @brief Unit test function sl_iec60730_safety_check_reset_error() and sl_iec60730_safety_check_error_occur().
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: LicenseRef-MSLA
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of the Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement
 * By installing, copying or otherwise using this software, you agree to the
 * terms of the MSLA.
 *
 ******************************************************************************/

#include "unit_test_common.h"
#include "unit_test_iec60730_safety_check.h"

/*=======Set up=====*/
sl_iec60730_safety_check_t unit_test_safety_check = { SL_IEC60730_NO_FAILURE, 0 };
sl_iec60730_safety_check_t* get_result_safety_check = NULL;

/*=======Mock Code=====*/

/*=======Test Case=====*/
void test_sl_iec60730_safety_check_error_occur(void)
{
  /*Setup*/

  /*Execute test*/
  sl_iec60730_safety_check_error_occur(SL_IEC60730_INTERRUPT_FAIL);
  unit_test_safety_check.number_error++;
  unit_test_safety_check.error |= (1 << SL_IEC60730_INTERRUPT_FAIL);
  get_result_safety_check = sl_iec60730_safety_check_get_error();
  TEST_ASSERT_EQUAL(unit_test_safety_check.number_error, get_result_safety_check->number_error);
  TEST_ASSERT_EQUAL(unit_test_safety_check.error, get_result_safety_check->error);
}

void test_sl_iec60730_safety_check_reset_error(void)
{
  /*Setup*/

  /*Execute test*/
  sl_iec60730_safety_check_reset_error();
  get_result_safety_check = sl_iec60730_safety_check_get_error();
  TEST_ASSERT_EQUAL(0, get_result_safety_check->number_error);
  TEST_ASSERT_EQUAL(0, get_result_safety_check->error);
}

/*=======Run Test Case=====*/

void unit_test_run_all_test_cases(void)
{
  // Initialize
  memset((void *)test_case_result, 0xff, sizeof(test_case_result));
  current_test_numth = 0;

  // Start run test
  printf("--- BEGIN UNIT TEST ---\n");
  UnityBegin("/unit_test/src/unit_test_iec60730_safety_check.c");
  run_test(test_sl_iec60730_safety_check_error_occur, "test_sl_iec60730_safety_check_error_occur", 29);
  run_test(test_sl_iec60730_safety_check_reset_error, "test_sl_iec60730_safety_check_reset_error", 42);
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
