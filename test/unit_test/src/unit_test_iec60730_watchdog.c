/***************************************************************************//**
 * @file  unit_test_iec60730_watchdog.c
 * @brief Unit tests for functions in watchdog module.
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
#include "unit_test_iec60730_watchdog.h"

/*=======Mock Code=====*/
extern volatile sl_iec60730_test_watchdog_t iec60730_watchdog_state;
sl_iec60730_test_result_t unit_test_result = SL_IEC60730_TEST_FAILED;
bool iec60730_watchdog_mock_rst_por = false;
bool iec60730_watchdog_mock_rst_em4 = false;
bool iec60730_watchdog_mock_set_watchdog_timout_min = false;
bool iec60730_watchdog_mock_rstcause_flag_valid = false;
uint32_t iec60730_watchdog_mock_rstcause = 0;

#if (SL_IEC60730_WDOG0_ENABLE == 1)
bool iec60730_watchdog_mock_rst_wdog0 = false;
#endif // (SL_IEC60730_WDOG0_ENABLE == 1)

#if (SL_IEC60730_WDOG1_ENABLE == 1)
bool iec60730_watchdog_mock_rst_wdog1 = false;
#endif // (SL_IEC60730_WDOG1_ENABLE == 1)

void unit_test_iec60730_watchdog_set_up(void)
{
  unit_test_result = SL_IEC60730_TEST_FAILED;
  iec60730_watchdog_state = SL_IEC60730_WATCHDOG_INVALID;
  iec60730_watchdog_mock_rst_por = false;
  iec60730_watchdog_mock_set_watchdog_timout_min = false;
  iec60730_watchdog_mock_rstcause_flag_valid = false;
#if (SL_IEC60730_WDOG0_ENABLE == 1)
  iec60730_watchdog_mock_rst_wdog0 = false;
#endif // (SL_IEC60730_WDOG0_ENABLE == 1)
#if (SL_IEC60730_WDOG1_ENABLE == 1)
  iec60730_watchdog_mock_rst_wdog1 = false;
#endif // (SL_IEC60730_WDOG1_ENABLE == 1)
  sl_iec60730_watchdog_count_reset();
}

bool unit_test_iec60730_watchdog_mock_rst_por(void)
{
  return iec60730_watchdog_mock_rst_por;
}

bool unit_test_iec60730_watchdog_mock_rst_em4(void)
{
  return iec60730_watchdog_mock_rst_em4;
}

#if (SL_IEC60730_WDOG0_ENABLE == 1)
bool unit_test_iec60730_watchdog_mock_rst_wdog0(void)
{
  return iec60730_watchdog_mock_rst_wdog0;
}
#endif // (SL_IEC60730_WDOG0_ENABLE == 1)

#if (SL_IEC60730_WDOG1_ENABLE == 1)
bool unit_test_iec60730_watchdog_mock_rst_wdog1(void)
{
  return iec60730_watchdog_mock_rst_wdog1;
}
#endif // (SL_IEC60730_WDOG1_ENABLE == 1)

bool unit_test_iec60730_watchdog_mock_set_watchdog_timout_min(void)
{
  return iec60730_watchdog_mock_set_watchdog_timout_min;
}

#ifdef SL_IEC60730_RSTCAUSES_CLEAR_ENABLE
void unit_test_iec60730_watchdog_mock_rstcause_clear(void)
{
  iec60730_watchdog_mock_rst_por = false;
  iec60730_watchdog_mock_rst_em4 = false;
  iec60730_watchdog_mock_rstcause = 0;
#if (SL_IEC60730_WDOG0_ENABLE == 1)
  iec60730_watchdog_mock_rst_wdog0 = false;
#endif // (SL_IEC60730_WDOG0_ENABLE == 1)
#if (SL_IEC60730_WDOG1_ENABLE == 1)
  iec60730_watchdog_mock_rst_wdog1 = false;
#endif // (SL_IEC60730_WDOG1_ENABLE == 1)
}
#endif // SL_IEC60730_RSTCAUSES_CLEAR_ENABLE

uint32_t unit_test_iec60730_watchdog_mock_rstcause(void)
{
  if (iec60730_watchdog_mock_rstcause_flag_valid) {
#if (_SILICON_LABS_32B_SERIES < 2)
#if (SL_IEC60730_WDOG0_ENABLE == 1)
    iec60730_watchdog_mock_rstcause = RMU_RSTCAUSE_WDOGRST;
#endif //(SL_IEC60730_WDOG0_ENABLE == 1)
#if (SL_IEC60730_WDOG1_ENABLE == 1)
    iec60730_watchdog_mock_rstcause = RMU_RSTCAUSE_WDOGRST;
#endif //(SL_IEC60730_WDOG1_ENABLE == 1)
#else
#if (SL_IEC60730_WDOG0_ENABLE == 1)
    iec60730_watchdog_mock_rstcause = EMU_RSTCAUSE_WDOG0;
#endif //(SL_IEC60730_WDOG0_ENABLE == 1)
#if (SL_IEC60730_WDOG1_ENABLE == 1)
    iec60730_watchdog_mock_rstcause = EMU_RSTCAUSE_WDOG0;
#endif //(SL_IEC60730_WDOG1_ENABLE == 1)
#endif // (_SILICON_LABS_32B_SERIES < 2)
  } else {
#if (_SILICON_LABS_32B_SERIES < 2)
#if (SL_IEC60730_WDOG0_ENABLE == 1)
    iec60730_watchdog_mock_rstcause = RMU_RSTCAUSE_SYSREQRST;
#endif //(SL_IEC60730_WDOG0_ENABLE == 1)
#if (SL_IEC60730_WDOG1_ENABLE == 1)
    iec60730_watchdog_mock_rstcause = RMU_RSTCAUSE_SYSREQRST;
#endif //(SL_IEC60730_WDOG1_ENABLE == 1)
#else
#if (SL_IEC60730_WDOG0_ENABLE == 1)
    iec60730_watchdog_mock_rstcause = EMU_RSTCAUSE_SYSREQ;
#endif //(SL_IEC60730_WDOG0_ENABLE == 1)
#if (SL_IEC60730_WDOG1_ENABLE == 1)
    iec60730_watchdog_mock_rstcause = EMU_RSTCAUSE_SYSREQ;
#endif //(SL_IEC60730_WDOG1_ENABLE == 1)
#endif // (_SILICON_LABS_32B_SERIES < 2)
  }

  return iec60730_watchdog_mock_rstcause;
}

/*=======Test Case=====*/

void test_sl_iec60730_watchdog_post_reset_causes_not_por_and_watchdog(void)
{
  /*Setup*/
  unit_test_iec60730_watchdog_set_up();
  /*Execute test*/
  unit_test_result = sl_iec60730_watchdog_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_PASSED, unit_test_result);
}

void test_sl_iec60730_watchdog_post_wachdog_reset_before_por(void)
{
  /*Setup*/
  unit_test_iec60730_watchdog_set_up();
  #if (SL_IEC60730_WDOG0_ENABLE == 1)
  iec60730_watchdog_mock_rst_wdog0 = true;
  #endif // (SL_IEC60730_WDOG0_ENABLE == 1)
  #if (SL_IEC60730_WDOG1_ENABLE == 1)
  iec60730_watchdog_mock_rst_wdog1 = true;
  #endif // (SL_IEC60730_WDOG1_ENABLE == 1)
  /*Execute test*/
  unit_test_result = sl_iec60730_watchdog_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, unit_test_result);
}

void test_sl_iec60730_watchdog_post_set_watchdog_timout_min_success(void)
{
  /*Setup*/
  unit_test_iec60730_watchdog_set_up();
  iec60730_watchdog_mock_rst_por = true;
  iec60730_watchdog_mock_set_watchdog_timout_min = true;
  iec60730_watchdog_mock_rstcause_flag_valid = true;
  /*Execute test*/
  unit_test_result = sl_iec60730_watchdog_post();
  if (unit_test_iec60730_watchdog_mock_set_watchdog_timout_min()) {
    #if (SL_IEC60730_WDOG0_ENABLE == 1)
    iec60730_watchdog_mock_rst_wdog0 = true;
    unit_test_result = sl_iec60730_watchdog_post();
    #endif // (SL_IEC60730_WDOG0_ENABLE == 1)
    #if (SL_IEC60730_WDOG1_ENABLE == 1)
    iec60730_watchdog_mock_rst_wdog1 = true;
    unit_test_result = sl_iec60730_watchdog_post();
    #endif // (SL_IEC60730_WDOG1_ENABLE == 1)
  }
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_PASSED, unit_test_result);
}

void test_sl_iec60730_watchdog_post_set_watchdog_timout_min_fail(void)
{
  /*Setup*/
  unit_test_iec60730_watchdog_set_up();
  iec60730_watchdog_mock_rst_por = true;
  /*Execute test*/
  unit_test_result = sl_iec60730_watchdog_post();
  if (!unit_test_iec60730_watchdog_mock_set_watchdog_timout_min()) {
    #if (SL_IEC60730_WDOG0_ENABLE == 1)
    iec60730_watchdog_mock_rst_wdog0 = false;
    unit_test_result = sl_iec60730_watchdog_post();
    #endif // (SL_IEC60730_WDOG0_ENABLE == 1)
    #if (SL_IEC60730_WDOG1_ENABLE == 1)
    iec60730_watchdog_mock_rst_wdog1 = false;
    unit_test_result = sl_iec60730_watchdog_post();
    #endif // (SL_IEC60730_WDOG1_ENABLE == 1)
  }
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, unit_test_result);
}

void test_sl_iec60730_watchdog_post_reset_resason_no_valid(void)
{
  /*Setup*/
  unit_test_iec60730_watchdog_set_up();
  iec60730_watchdog_mock_rst_por = true;
  iec60730_watchdog_mock_set_watchdog_timout_min = true;
  sl_iec60730_watchdog_post();
  /*Execute test*/
  unit_test_result = sl_iec60730_watchdog_post();
  if (unit_test_iec60730_watchdog_mock_set_watchdog_timout_min()) {
    #if (SL_IEC60730_WDOG0_ENABLE == 1)
    iec60730_watchdog_mock_rst_wdog0 = true;
    unit_test_result = sl_iec60730_watchdog_post();
    #endif // (SL_IEC60730_WDOG0_ENABLE == 1)
    #if (SL_IEC60730_WDOG1_ENABLE == 1)
    iec60730_watchdog_mock_rst_wdog1 = true;
    unit_test_result = sl_iec60730_watchdog_post();
    #endif // (SL_IEC60730_WDOG1_ENABLE == 1)
  }
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, unit_test_result);
}

void test_sl_iec60730_watchdog_post_prevent_unexpected_changed_value(void)
{
  /*Setup*/
  unit_test_iec60730_watchdog_set_up();
  iec60730_watchdog_mock_rst_por = true;
  iec60730_watchdog_mock_set_watchdog_timout_min = true;
  iec60730_watchdog_mock_rstcause_flag_valid = true;
  sl_iec60730_watchdog_count_set(SL_IEC60730_WDOGINST_NUMB);
  /*Execute test*/
  unit_test_result = sl_iec60730_watchdog_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, unit_test_result);
}

/*=======Run Test Case=====*/

void unit_test_run_all_test_cases(void)
{
  // Initialize
  memset((void *)test_case_result, 0xff, sizeof(test_case_result));
  current_test_numth = 0;

  // Start run test
  printf("--- BEGIN UNIT TEST ---\n");
  UnityBegin("/unit_test/src/unit_test_iec60730_watchdog.c");
  run_test(test_sl_iec60730_watchdog_post_reset_causes_not_por_and_watchdog, "test_sl_iec60730_watchdog_post_reset_causes_not_por_and_watchdog", 132);
  run_test(test_sl_iec60730_watchdog_post_wachdog_reset_before_por, "test_sl_iec60730_watchdog_post_wachdog_reset_before_por", 140);
  run_test(test_sl_iec60730_watchdog_post_set_watchdog_timout_min_success, "test_sl_iec60730_watchdog_post_set_watchdog_timout_min_success", 154);
  run_test(test_sl_iec60730_watchdog_post_set_watchdog_timout_min_fail, "test_sl_iec60730_watchdog_post_set_watchdog_timout_min_fail", 175);
  run_test(test_sl_iec60730_watchdog_post_reset_resason_no_valid, "test_sl_iec60730_watchdog_post_reset_resason_no_valid", 194);
  run_test(test_sl_iec60730_watchdog_post_prevent_unexpected_changed_value, "test_sl_iec60730_watchdog_post_prevent_unexpected_changed_value", 215);
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
