/***************************************************************************//**
 * @file  unit_test_iec60730_cpu_registers.c
 * @brief Unit tests for functions in cpu registers module.
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
#include "unit_test_iec60730_cpu_registers.h"

/*=======Mock Code=====*/
uint16_t sl_iec60730_program_counter_check = 0;

sl_iec60730_test_result_t unit_test_iec60730_cpu_registers_bist_mock(void)
{
  return cmock_return_value();
}

sl_iec60730_test_result_t unit_test_iec60730_cpu_registers_bist_s_mock(void)
{
  return cmock_return_value();
}

sl_iec60730_test_result_t unit_test_iec60730_cpu_registers_bist_ns_mock(void)
{
  return cmock_return_value();
}

/*=======Test Case=====*/
void test_sl_iec60730_cpu_registers_bist_pass_all_check_condition(void)
{
  /*Setup*/
  sl_iec60730_program_counter_check = 0;
  cmock_set_value(SL_IEC60730_TEST_PASSED);
  /*Execute test*/
  sl_iec60730_cpu_registers_bist();
  TEST_ASSERT_EQUAL(IEC60730_CPU_REGS_COMPLETE, sl_iec60730_program_counter_check);
}

void test_sl_iec60730_cpu_registers_bist_failed_check_condition(void)
{
  /*Setup*/
  sl_iec60730_program_counter_check = 0;
  cmock_set_value(SL_IEC60730_TEST_FAILED);
  /*Execute test*/
  sl_iec60730_cpu_registers_bist();
  TEST_ASSERT_EQUAL(0, sl_iec60730_program_counter_check);
}

void test_sl_iec60730_cpu_registers_bist_ns_pass_all_check_condition(void)
{
  #if (IEC60370_CPU == IEC60370_CM33)
  #if defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
  #if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  cmock_set_value(SL_IEC60730_TEST_PASSED);
  /*Execute test*/
  result = sl_iec60730_cpu_registers_bist_ns();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_PASSED, result);
  #endif
  #else
  #error the CM33 without TZ is not supported
  #endif // defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
  #endif // (IEC60370_CPU == IEC60370_CM33)

  #if (IEC60370_CPU == IEC60370_CM4)
  TEST_ASSERT(true);
  #endif // IEC60370_CPU == IEC60370_CM4)
}

void test_sl_iec60730_cpu_registers_bist_ns_failed_check_condition(void)
{
  #if (IEC60370_CPU == IEC60370_CM33)
  #if defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
  #if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  cmock_set_value(SL_IEC60730_TEST_FAILED);
  /*Execute test*/
  result = sl_iec60730_cpu_registers_bist_ns();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
  #endif
  #else
  #error the CM33 without TZ is not supported
  #endif // defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
  #endif // (IEC60370_CPU == IEC60370_CM33)

  #if (IEC60370_CPU == IEC60370_CM4)
  TEST_ASSERT(true);
  #endif // IEC60370_CPU == IEC60370_CM4)
}

void test_sl_iec60730_cpu_registers_bist_s_pass_all_check_condition(void)
{
  #if (IEC60370_CPU == IEC60370_CM33)
  #if defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
  #if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  cmock_set_value(SL_IEC60730_TEST_PASSED);
  /*Execute test*/
  result = sl_iec60730_cpu_registers_bist_s();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_PASSED, result);
  #endif
  #else
  #error the CM33 without TZ is not supported
  #endif // defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
  #endif // (IEC60370_CPU == IEC60370_CM33)

  #if (IEC60370_CPU == IEC60370_CM4)
  TEST_ASSERT(true);
  #endif // IEC60370_CPU == IEC60370_CM4)
}

void test_sl_iec60730_cpu_registers_bist_s_failed_check_condition(void)
{
  #if (IEC60370_CPU == IEC60370_CM33)
  #if defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
  #if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  cmock_set_value(SL_IEC60730_TEST_FAILED);
  /*Execute test*/
  result = sl_iec60730_cpu_registers_bist_s();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
  #endif
  #else
  #error the CM33 without TZ is not supported
  #endif // defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
  #endif // (IEC60370_CPU == IEC60370_CM33)

  #if (IEC60370_CPU == IEC60370_CM4)
  TEST_ASSERT(true);
  #endif // IEC60370_CPU == IEC60370_CM4)
}

/*=======Run Test Case=====*/

void unit_test_run_all_test_cases(void)
{
  // Initialize
  memset((void *)test_case_result, 0xff, sizeof(test_case_result));
  current_test_numth = 0;

  // Start run test
  printf("--- BEGIN UNIT TEST ---\n");
  UnityBegin("/unit_test/src/unit_test_iec60730_cpu_registers.c");
  run_test(test_sl_iec60730_cpu_registers_bist_pass_all_check_condition, "test_sl_iec60730_cpu_registers_bist_pass_all_check_condition", 38);
  run_test(test_sl_iec60730_cpu_registers_bist_failed_check_condition, "test_sl_iec60730_cpu_registers_bist_failed_check_condition", 48);
  run_test(test_sl_iec60730_cpu_registers_bist_ns_pass_all_check_condition, "test_sl_iec60730_cpu_registers_bist_ns_pass_all_check_condition", 58);
  run_test(test_sl_iec60730_cpu_registers_bist_ns_failed_check_condition, "test_sl_iec60730_cpu_registers_bist_ns_failed_check_condition", 80);
  run_test(test_sl_iec60730_cpu_registers_bist_s_pass_all_check_condition, "test_sl_iec60730_cpu_registers_bist_s_pass_all_check_condition", 102);
  run_test(test_sl_iec60730_cpu_registers_bist_s_failed_check_condition, "test_sl_iec60730_cpu_registers_bist_s_failed_check_condition", 124);

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
