/***************************************************************************//**
 * @file  unit_test_iec60730_variable_memory.c
 * @brief Unit tests for functions in variable memory module
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
#include "unit_test_iec60730_variable_memory.h"

/*=======Mock Code=====*/

#define UNIT_TEST_NUM_RAM_REGIONS_CHECK     3

uint16_t sl_iec60730_program_counter_check;
__no_init sl_iec60730_vmc_test_multiple_regions_t unit_test_vmc_test;

const sl_iec60730_vmc_test_region_t unit_test_vmc_region_test_failed[1] =
{ { .start = RAMTEST_END, .end = RAMTEST_START } };

#if defined(__GNUC__)
#define UNIT_TEST_RAM_OFFSET                20
#elif defined(__ICCARM__)
#define UNIT_TEST_RAM_OFFSET                80
#else
#endif

#if defined(__GNUC__)

const sl_iec60730_vmc_test_region_t unit_test_vmc_region_test[UNIT_TEST_NUM_RAM_REGIONS_CHECK] =
{ { .start = RAMTEST_START, .end = RAMTEST_END },
  { .start = RAM_START, .end = RAM_START + UNIT_TEST_RAM_OFFSET },
  { .start = RAM_START + 2 * UNIT_TEST_RAM_OFFSET, .end = RAM_START + 3 * UNIT_TEST_RAM_OFFSET } };

#elif defined(__ICCARM__)

const sl_iec60730_vmc_test_region_t unit_test_vmc_region_test[UNIT_TEST_NUM_RAM_REGIONS_CHECK] =
{ { .start = RAMTEST_START, .end = RAMTEST_END },
  { .start = RAM_START, .end = (uint32_t *)((uint32_t)RAM_START + UNIT_TEST_RAM_OFFSET) },
  { .start = (uint32_t *)((uint32_t)RAM_START + 2 * UNIT_TEST_RAM_OFFSET), .end = (uint32_t *)((uint32_t)RAM_START + 3 * UNIT_TEST_RAM_OFFSET) } };

#else
#endif
sl_iec60730_test_result_t vmc_marchc_step_check_ram_section;
sl_iec60730_test_result_t vmc_marchc_step_check_backup_section;
sl_iec60730_test_result_t check_integrity_result;
bool iec60730_vmc_pre_run_marchxc_step_flag;
bool run_test_sl_iec60730_vmc_bist_allow;

sl_iec60730_test_result_t unit_test_mock_check_integrity(void)
{
  return check_integrity_result;
}

__WEAK bool sl_iec60730_vmc_marchc_step(uint32_t *start,
                                        uint32_t *end,
                                        uint32_t background,
                                        uint32_t *backup)
{
  (void)background;
  (void)end;
  if (start == backup) {
    cmock_set_value(vmc_marchc_step_check_backup_section);
    if (!check_integrity_result) {
      cmock_set_value(unit_test_mock_check_integrity());
    }
  } else {
    cmock_set_value(vmc_marchc_step_check_ram_section);
    if (!check_integrity_result) {
      cmock_set_value(unit_test_mock_check_integrity());
    }
  }
  return cmock_return_value();
}

__WEAK bool sl_iec60730_vmc_marchxc_step(uint32_t *start,
                                         uint32_t *end,
                                         uint32_t background,
                                         uint32_t *backup)
{
  (void)background;
  (void)end;
  if (start == backup) {
    cmock_set_value(vmc_marchc_step_check_backup_section);
    if (!check_integrity_result) {
      cmock_set_value(unit_test_mock_check_integrity());
    }
  } else {
    run_test_sl_iec60730_vmc_bist_allow = true;
    cmock_set_value(vmc_marchc_step_check_ram_section);
    if (!check_integrity_result) {
      cmock_set_value(unit_test_mock_check_integrity());
    }
  }
  return cmock_return_value();
}

bool sl_iec60730_vmc_pre_run_marchxc_step(uint32_t *addr, uint32_t size)
{
  UNUSED_VAR(addr);
  UNUSED_VAR(size);
  return iec60730_vmc_pre_run_marchxc_step_flag;
}

void set_up_test_vmc_post_and_bist(void)
{
  // VMC test config
  unit_test_vmc_test.region = unit_test_vmc_region_test;
  unit_test_vmc_test.number_of_test_regions = UNIT_TEST_NUM_RAM_REGIONS_CHECK;
  vmc_marchc_step_check_ram_section = SL_IEC60730_TEST_PASSED;
  vmc_marchc_step_check_backup_section = SL_IEC60730_TEST_PASSED;
  check_integrity_result = SL_IEC60730_TEST_PASSED;
  iec60730_vmc_pre_run_marchxc_step_flag = true;
  run_test_sl_iec60730_vmc_bist_allow = false;
  sl_iec60730_vmc_init(&unit_test_vmc_test);
}

/*=======Test Case=====*/
void test_sl_iec60730_vmc_init_param_null(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  unit_test_vmc_test.region = NULL;
  /*Execute test*/
  sl_iec60730_vmc_init(&unit_test_vmc_test);
  result = sl_iec60730_vmc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_init_start_address_greater_than_end_address(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  unit_test_vmc_test.region = unit_test_vmc_region_test_failed;
  unit_test_vmc_test.number_of_test_regions = 1;
  /*Execute test*/
  sl_iec60730_vmc_init(&unit_test_vmc_test);
  result = sl_iec60730_vmc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_post_pass_all_check_sections(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  /*Execute test*/
  result = sl_iec60730_vmc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_PASSED, result);
}

void test_sl_iec60730_vmc_post_fail_marchc_check_ram_section(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  vmc_marchc_step_check_ram_section = SL_IEC60730_TEST_FAILED;
  /*Execute test*/
  result = sl_iec60730_vmc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_post_fail_marchc_check_backup_section(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  vmc_marchc_step_check_backup_section = SL_IEC60730_TEST_FAILED;
  /*Execute test*/
  result = sl_iec60730_vmc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_post_fail_check_integrity_ram_section(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  check_integrity_result = SL_IEC60730_TEST_FAILED;
  /*Execute test*/
  result = sl_iec60730_vmc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_post_fail_check_integrity_backup_section(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  check_integrity_result = SL_IEC60730_TEST_FAILED;
  /*Execute test*/
  result = sl_iec60730_vmc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_bist_pass_all_check_sections(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  /*Execute test*/
  result = sl_iec60730_vmc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_PASSED, result);
}

void test_sl_iec60730_vmc_bist_fail_marchxc_check_ram_section(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  vmc_marchc_step_check_ram_section = SL_IEC60730_TEST_FAILED;
  /*Execute test*/
  result = sl_iec60730_vmc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_bist_fail_marchxc_check_backup_section(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  vmc_marchc_step_check_backup_section = SL_IEC60730_TEST_FAILED;
  /*Execute test*/
  result = sl_iec60730_vmc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_bist_fail_check_integrity_ram_section(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  check_integrity_result = SL_IEC60730_TEST_FAILED;
  /*Execute test*/
  result = sl_iec60730_vmc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_bist_fail_check_integrity_backup_section(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  check_integrity_result = SL_IEC60730_TEST_FAILED;
  /*Execute test*/
  result = sl_iec60730_vmc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_vmc_bist_checking_allow_run_test(void)
{
  /*Setup*/
  set_up_test_vmc_post_and_bist();
  iec60730_vmc_pre_run_marchxc_step_flag = false;
  /*Execute test*/
  sl_iec60730_vmc_bist();
  TEST_ASSERT_EQUAL(false, run_test_sl_iec60730_vmc_bist_allow);
}

void test_sl_iec60730_vmc_bist_fail_stack_test_over_flow(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  uint32_t *stack_start = STACK_CHECK;
  /*Execute test*/
  *stack_start = SL_STACK_OVERFLOW_CONST_GUARD_VALUE_3;
  result = sl_iec60730_vmc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

/*=======Run Test Case=====*/

void unit_test_run_all_test_cases(void)
{
  // Initialize
  memset((void *)test_case_result, 0xff, sizeof(test_case_result));
  current_test_numth = 0;

  // Start run test
  printf("--- BEGIN UNIT TEST ---\n");
  UnityBegin("/unit_test/src/unit_test_iec60730_variable_memory.c");
  run_test(test_sl_iec60730_vmc_init_param_null, "test_sl_iec60730_vmc_init_param_null", 126);
  run_test(test_sl_iec60730_vmc_init_start_address_greater_than_end_address, "test_sl_iec60730_vmc_init_start_address_greater_than_end_address", 136);
  run_test(test_sl_iec60730_vmc_post_pass_all_check_sections, "test_sl_iec60730_vmc_post_pass_all_check_sections", 147);
  run_test(test_sl_iec60730_vmc_post_fail_marchc_check_ram_section, "test_sl_iec60730_vmc_post_fail_marchc_check_ram_section", 156);
  run_test(test_sl_iec60730_vmc_post_fail_marchc_check_backup_section, "test_sl_iec60730_vmc_post_fail_marchc_check_backup_section", 166);
  run_test(test_sl_iec60730_vmc_post_fail_check_integrity_ram_section, "test_sl_iec60730_vmc_post_fail_check_integrity_ram_section", 176);
  run_test(test_sl_iec60730_vmc_post_fail_check_integrity_backup_section, "test_sl_iec60730_vmc_post_fail_check_integrity_backup_section", 186);
  run_test(test_sl_iec60730_vmc_bist_pass_all_check_sections, "test_sl_iec60730_vmc_bist_pass_all_check_sections", 196);
  run_test(test_sl_iec60730_vmc_bist_fail_marchxc_check_ram_section, "test_sl_iec60730_vmc_bist_fail_marchxc_check_ram_section", 205);
  run_test(test_sl_iec60730_vmc_bist_fail_marchxc_check_backup_section, "test_sl_iec60730_vmc_bist_fail_marchxc_check_backup_section", 215);
  run_test(test_sl_iec60730_vmc_bist_fail_check_integrity_ram_section, "test_sl_iec60730_vmc_bist_fail_check_integrity_ram_section", 225);
  run_test(test_sl_iec60730_vmc_bist_fail_check_integrity_backup_section, "test_sl_iec60730_vmc_bist_fail_check_integrity_backup_section", 235);
  run_test(test_sl_iec60730_vmc_bist_checking_allow_run_test, "test_sl_iec60730_vmc_bist_checking_allow_run_test", 245);
  run_test(test_sl_iec60730_vmc_bist_fail_stack_test_over_flow, "test_sl_iec60730_vmc_bist_fail_stack_test_over_flow", 254);
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
