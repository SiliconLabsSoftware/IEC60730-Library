/***************************************************************************//**
 * @file  unit_test_iec60730_irq.c
 * @brief Unit tests for functions in invariable memory module
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

#include <string.h>
#include "em_core.h"
#include "em_gpcrc.h"
#include "em_cmu.h"
#include "unit_test_common.h"
#include "unit_test_iec60730_invariable_memory.h"

/*=======Mock Code=====*/
uint16_t sl_iec60730_program_counter_check;
__no_init sl_iec60730_imc_params_t unit_test_imc;
__no_init sl_iec60730_imc_test_multiple_regions_t unit_test_imc_test __CLASSB_RAM;

#define UNIT_TEST_NUM_FLASH_REGIONS_CHECK   3

#if defined(__GNUC__)
#define UNIT_TEST_FLASH_OFFSET              20
#elif defined(__ICCARM__)
#define UNIT_TEST_FLASH_OFFSET              80
#else
#endif

#if defined(__GNUC__)
const sl_iec60730_imc_test_region_t unit_test_imc_region_test[UNIT_TEST_NUM_FLASH_REGIONS_CHECK] =
{ { .start = SL_IEC60730_ROM_START, .end = SL_IEC60730_ROM_END },
  { .start = SL_IEC60730_ROM_START + 2 * UNIT_TEST_FLASH_OFFSET, .end = SL_IEC60730_ROM_START + 3 * UNIT_TEST_FLASH_OFFSET },
  { .start = SL_IEC60730_ROM_START + 4 * UNIT_TEST_FLASH_OFFSET, .end = SL_IEC60730_ROM_START + 5 * UNIT_TEST_FLASH_OFFSET } };

#elif defined(__ICCARM__)
const sl_iec60730_imc_test_region_t unit_test_imc_region_test[UNIT_TEST_NUM_FLASH_REGIONS_CHECK] =
{ { .start = SL_IEC60730_ROM_START, .end = SL_IEC60730_ROM_END },
  { .start = (uint32_t *)((uint32_t)SL_IEC60730_ROM_START + 2 * UNIT_TEST_FLASH_OFFSET), .end = (uint32_t *)((uint32_t)SL_IEC60730_ROM_START + 3 * UNIT_TEST_FLASH_OFFSET) },
  { .start = (uint32_t *)((uint32_t)SL_IEC60730_ROM_START + 4 * UNIT_TEST_FLASH_OFFSET), .end = (uint32_t *)((uint32_t)SL_IEC60730_ROM_START + 5 * UNIT_TEST_FLASH_OFFSET) } };
#else
#endif

#if (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
bool unit_test_gpcrc_enable;
#else
bool unit_test_sw_cal_crc_enable;
#endif // (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
sl_iec60730_test_result_t check_integrity_result;
uint32_t* unit_test_init_run_crc;

uint32_t* unit_test_iec60730_imc_mock_init_run_crc(void)
{
  return unit_test_init_run_crc;
}

sl_iec60730_test_result_t unit_test_mock_check_integrity(void)
{
  return check_integrity_result;
}

void set_up_test_vmc_post_and_bist(void)
{
  CMU_ClockEnable(cmuClock_GPCRC, true);
  unit_test_imc.gpcrc = SL_IEC60730_DEFAULT_GPRC;
  unit_test_imc_test.region = unit_test_imc_region_test;
  unit_test_imc_test.number_of_test_regions = 1;
  check_integrity_result = SL_IEC60730_TEST_PASSED;
  unit_test_init_run_crc = SL_IEC60730_ROM_START;
  #if (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
  unit_test_gpcrc_enable = true;
  #else
  unit_test_sw_cal_crc_enable = true;
  #endif // (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
  sl_iec60730_imc_init(&unit_test_imc, &unit_test_imc_test);
}

#if (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
bool unit_test_iec60730_imc_mock_enable_gpcrc(void)
{
  return unit_test_gpcrc_enable;
}
#else
bool unit_test_iec60730_imc_mock_sw_enable_cal_crc(void)
{
  return unit_test_sw_cal_crc_enable;
}
#endif // (SL_IEC60730_CRC_USE_SW_ENABLE == 0)

/*=======Test Case=====*/
void test_sl_iec60730_imc_init_param_null(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  unit_test_imc.gpcrc = NULL;
  /*Execute test*/
  sl_iec60730_imc_init(NULL, NULL);
  result = sl_iec60730_imc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_imc_post_pass_check(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  /*Execute test*/
  result = sl_iec60730_imc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_PASSED, result);
}

void test_sl_iec60730_imc_post_fail_check(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  #if (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
  unit_test_gpcrc_enable = false;
  #else
  unit_test_sw_cal_crc_enable = false;
  #endif // (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
  /*Execute test*/
  sl_iec60730_imc_init(&unit_test_imc, &unit_test_imc_test);
  result = sl_iec60730_imc_post();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_imc_bist_pass_all_check(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  sl_iec60730_imc_post();
  /*Execute test*/
  result = sl_iec60730_imc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_PASSED, result);
}

void test_sl_iec60730_imc_bist_fail_check_integrity(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  check_integrity_result = SL_IEC60730_TEST_FAILED;
  /*Execute test*/
  result = sl_iec60730_imc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_imc_bist_iec60730_run_crc_greater_than_rom_end_address(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  unit_test_init_run_crc = SL_IEC60730_ROM_END;
  sl_iec60730_imc_init(&unit_test_imc, &unit_test_imc_test);
  /*Execute test*/
  result = sl_iec60730_imc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_imc_bist_fail_compare_crc(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  sl_iec60730_imc_post();
  set_up_test_vmc_post_and_bist();
  #if (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
  unit_test_gpcrc_enable = false;
  sl_iec60730_imc_init(&unit_test_imc, &unit_test_imc_test);
  #else
  sl_iec60730_imc_init(&unit_test_imc, &unit_test_imc_test);
  unit_test_sw_cal_crc_enable = false;
  #endif // (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
  /*Execute test*/
  result = sl_iec60730_imc_bist();
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_update_crc_with_data_buffer_params_null(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  sl_iec60730_crc_t crc_value = (sl_iec60730_crc_t) SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE;
  /*Execute test*/
  result = sl_iec60730_update_crc_with_data_buffer(NULL,
                                                   &crc_value,
                                                   (uint8_t *)SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_TEST,
                                                   strlen(SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_TEST));
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_FAILED, result);
}

void test_sl_iec60730_update_crc_with_data_buffer_calculation_crc(void)
{
  /*Setup*/
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  set_up_test_vmc_post_and_bist();
  sl_iec60730_crc_t crc_value = (sl_iec60730_crc_t) SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE;
  sl_iec60730_update_crc_params_t update = SL_IEC60730_IMC_CRC_BUFFER_UPDATE_DEFAULT;
  /*Execute test*/
  result = sl_iec60730_update_crc_with_data_buffer(&update,
                                                   &crc_value,
                                                   (uint8_t *)SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_TEST,
                                                   (uint32_t)strlen(SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_TEST));
  TEST_ASSERT_EQUAL(SL_IEC60730_TEST_PASSED, result);

  #if (SL_IEC60730_CRC_USE_SW_ENABLE == 1)
  #if (SL_IEC60730_USE_CRC_32_ENABLE == 1)
  TEST_ASSERT_EQUAL(SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_RESULT, crc_value);
  #else // (SL_IEC60730_USE_CRC_32_ENABLE != 1)
  TEST_ASSERT_EQUAL(SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_RESULT, crc_value);
  #endif // (SL_IEC60730_USE_CRC_32_ENABLE == 1)
  #else // (SL_IEC60730_CRC_USE_SW_ENABLE != 1)
  #if (SL_IEC60730_USE_CRC_32_ENABLE == 1)
  if (SL_IEC60730_IMC_DATA_READ == update.readType) {
    TEST_ASSERT_EQUAL(SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_RESULT, crc_value);
  } else if (SL_IEC60730_IMC_DATA_READ_BIT_REVERSED == update.readType) {
    TEST_ASSERT_EQUAL(0x649C2FD3, crc_value);
  } else if (SL_IEC60730_IMC_DATA_READ_BYTE_REVERSED == update.readType) {
    TEST_ASSERT_EQUAL(0x2639F4CB, crc_value);
  } else {
    TEST_ASSERT_EQUAL(SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE, crc_value);
  }
  #else // (SL_IEC60730_USE_CRC_32_ENABLE != 1)
  if (SL_IEC60730_IMC_DATA_READ == update.readType) {
    TEST_ASSERT_EQUAL(0x8D94, crc_value);
  } else if (SL_IEC60730_IMC_DATA_READ_BIT_REVERSED == update.readType) {
    TEST_ASSERT_EQUAL(SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_RESULT, crc_value);
  } else if (SL_IEC60730_IMC_DATA_READ_BYTE_REVERSED == update.readType) {
    TEST_ASSERT_EQUAL(0x948D, crc_value);
  } else {
    TEST_ASSERT_EQUAL(SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE, crc_value);
  }
  #endif // (SL_IEC60730_USE_CRC_32_ENABLE == 1)
  #endif // (SL_IEC60730_CRC_USE_SW_ENABLE == 1)
}

/*=======Run Test Case=====*/

void unit_test_run_all_test_cases(void)
{
  // Initialize
  memset((void *)test_case_result, 0xff, sizeof(test_case_result));
  current_test_numth = 0;

  // Start run test
  printf("--- BEGIN UNIT TEST ---\n");
  UnityBegin("/unit_test/src/unit_test_iec60730_invariable_memory.c");
  run_test(test_sl_iec60730_imc_init_param_null, "test_sl_iec60730_imc_init_param_null", 95);
  run_test(test_sl_iec60730_imc_post_pass_check, "test_sl_iec60730_imc_post_pass_check", 105);
  run_test(test_sl_iec60730_imc_post_fail_check, "test_sl_iec60730_imc_post_fail_check", 114);
  run_test(test_sl_iec60730_imc_bist_pass_all_check, "test_sl_iec60730_imc_bist_pass_all_check", 129);
  run_test(test_sl_iec60730_imc_bist_fail_check_integrity, "test_sl_iec60730_imc_bist_fail_check_integrity", 139);
  run_test(test_sl_iec60730_imc_bist_iec60730_run_crc_greater_than_rom_end_address, "test_sl_iec60730_imc_bist_iec60730_run_crc_greater_than_rom_end_address", 149);
  run_test(test_sl_iec60730_imc_bist_fail_compare_crc, "test_sl_iec60730_imc_bist_fail_compare_crc", 160);
  run_test(test_sl_iec60730_update_crc_with_data_buffer_params_null, "test_sl_iec60730_update_crc_with_data_buffer_params_null", 178);
  run_test(test_sl_iec60730_update_crc_with_data_buffer_calculation_crc, "test_sl_iec60730_update_crc_with_data_buffer_calculation_crc", 191);
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
