/***************************************************************************//**
 * @file  unit_test_iec60730_irq.c
 * @brief Unit tests for functions in irq module
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
#include "unit_test_iec60730_irq.h"

/*=======Set up=====*/
#define IEC60730_IRQ_SIZE           8
#define UNIT_TEST_NUM_IRQ_CHECK     1

static bool is_function_called = false;
uint16_t sl_iec60730_program_counter_check = 0;
sl_iec60730_irq_fail_t unit_test_irq = { 0, 0 };
sl_iec60730_irq_fail_t* get_result_irq_fail = NULL;

volatile SL_IEC60730_IRQ_TYPE_VARIABLE unit_test_irq_exec_count[IEC60730_IRQ_SIZE] = { 0 };
const sl_iec60730_irq_execution_bounds_t unit_test_irq_freq_bounds[IEC60730_IRQ_SIZE] = { { .min = 9, .max = 11 },
                                                                                          { .min = 9, .max = 11 },
                                                                                          { .min = 9, .max = 11 },
                                                                                          { .min = 9, .max = 11 },
                                                                                          { .min = 9, .max = 11 },
                                                                                          { .min = 9, .max = 11 },
                                                                                          { .min = 9, .max = 11 },
                                                                                          { .min = 9, .max = 11 } };

sl_iec60730_irq_cfg_t unit_test_irq_config = { &unit_test_irq_exec_count[0],
                                               &unit_test_irq_freq_bounds[0],
                                               UNIT_TEST_NUM_IRQ_CHECK };

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

/*=======Test Case=====*/

void test_sl_iec60730_irq_init_param_null(void)
{
  /*Setup*/
  is_function_called = false;
  /*Execute test*/
  sl_iec60730_irq_init(NULL);
  // check config is not init when param NULL
  sl_iec60730_irq_check();
  TEST_ASSERT_EQUAL(true, is_function_called);
}
void test_sl_iec60730_irq_init_param_size_greater_than_max_irq(void)
{
  /*Setup*/
  is_function_called = false;
  unit_test_irq_config.size = IEC60730_MAX_IRQ_CHECK;
  /*Execute test*/
  sl_iec60730_irq_init(&unit_test_irq_config);
  TEST_ASSERT_EQUAL(true, is_function_called);
}

void test_sl_iec60730_irq_check_count_in_bound(void)
{
  /*Setup*/
  is_function_called = false;
  unit_test_irq_config.size = UNIT_TEST_NUM_IRQ_CHECK;
  unit_test_irq_config.irq_count[0] = 10;
  sl_iec60730_irq_init(&unit_test_irq_config);
  /*Execute test*/
  sl_iec60730_irq_check();
  TEST_ASSERT_EQUAL(false, is_function_called);
}

void test_sl_iec60730_irq_check_count_out_bound(void)
{
  /*Setup*/
  is_function_called = false;
  unit_test_irq_config.size = UNIT_TEST_NUM_IRQ_CHECK;
  unit_test_irq_config.irq_count[0] = 12;
  sl_iec60730_irq_init(&unit_test_irq_config);
  /*Execute test*/
  sl_iec60730_irq_check();
  TEST_ASSERT_EQUAL(true, is_function_called);
}

void test_sl_iec60730_irq_reset_counter(void)
{
  /*Setup*/
  unit_test_irq_config.irq_count[0] = 12;
  sl_iec60730_irq_init(&unit_test_irq_config);
  /*Execute test*/
  sl_iec60730_irq_reset_counter();
  TEST_ASSERT_EQUAL(0, unit_test_irq_config.irq_count[0]);
}

void test_sl_iec60730_irq_fail_occur(void)
{
  /*Setup*/
  unit_test_irq.irq_fail |= (1 << 0);
  unit_test_irq.num_irq_fail = 1;
  unit_test_irq_config.size = UNIT_TEST_NUM_IRQ_CHECK;
  unit_test_irq_config.irq_count[0] = 12;
  sl_iec60730_irq_init(&unit_test_irq_config);
  sl_iec60730_irq_reset_fail_result();
  /*Execute test*/
  sl_iec60730_irq_check();
  get_result_irq_fail = sl_iec60730_get_irq_index_failed();
  TEST_ASSERT_EQUAL(unit_test_irq.irq_fail, get_result_irq_fail->irq_fail);
  TEST_ASSERT_EQUAL(unit_test_irq.num_irq_fail, get_result_irq_fail->num_irq_fail);
}

void test_sl_iec60730_irq_reset_fail_result(void)
{
  /*Setup*/
  unit_test_irq_config.size = UNIT_TEST_NUM_IRQ_CHECK;
  unit_test_irq_config.irq_count[0] = 12;
  sl_iec60730_irq_init(&unit_test_irq_config);
  sl_iec60730_irq_check();
  /*Execute test*/
  sl_iec60730_irq_reset_fail_result();
  get_result_irq_fail = sl_iec60730_get_irq_index_failed();
  TEST_ASSERT_EQUAL(0, get_result_irq_fail->irq_fail);
  TEST_ASSERT_EQUAL(0, get_result_irq_fail->num_irq_fail);
}

/*=======Run Test Case=====*/

void unit_test_run_all_test_cases(void)
{
  // Initialize
  memset((void *)test_case_result, 0xff, sizeof(test_case_result));
  current_test_numth = 0;

  // Start run test
  printf("--- BEGIN UNIT TEST ---\n");
  UnityBegin("/unit_test/src/unit_test_iec60730_irq.c");
  run_test(test_sl_iec60730_irq_init_param_null, "test_sl_iec60730_irq_init_param_null", 53);
  run_test(test_sl_iec60730_irq_init_param_size_greater_than_max_irq, "test_sl_iec60730_irq_init_param_size_greater_than_max_irq", 62);
  run_test(test_sl_iec60730_irq_check_count_in_bound, "test_sl_iec60730_irq_check_count_in_bound", 72);
  run_test(test_sl_iec60730_irq_check_count_out_bound, "test_sl_iec60730_irq_check_count_out_bound", 85);
  run_test(test_sl_iec60730_irq_reset_counter, "test_sl_iec60730_irq_reset_counter", 98);
  run_test(test_sl_iec60730_irq_fail_occur, "test_sl_iec60730_irq_fail_occur", 108);
  run_test(test_sl_iec60730_irq_reset_fail_result, "test_sl_iec60730_irq_reset_fail_result", 120);
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
