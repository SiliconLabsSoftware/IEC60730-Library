/***************************************************************************/ /**
 * @file  unit_test_iec60730_invariable_memory.h
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

/**************************************************************************/ /**
 * @addtogroup IEC60730_UNIT_TEST
 * @{
 * @defgroup IEC60730_INVARIABLE_MEMORY_VERIFICATION_UNIT_TEST Invariable Memory Verification Unit Tests
 * @{
 * Header file list of test cases for the verification operate functions in invariable memory module.
 *
 * IEC60730 Invariable Memory Unit Tests
 *
 * @details These test cases run unit tests to check the operation of the functions in invariable memory module.
 *
 * Hardware setup:
 *
 *   - Connect DUT to PC. No additional hardware setup required.
 ******************************************************************************/

#ifndef UNIT_TEST_IEC60730_INVARIABLE_MEMORY_H
#define UNIT_TEST_IEC60730_INVARIABLE_MEMORY_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_iec60730_internal.h"

/*======= External Functions =====*/

/**************************************************************************/ /**
 * Public   Function mock returns the starting address value of the ram test area
 *
 * @returns pointer uint32_t*.
 *
 *****************************************************************************/
extern uint32_t* unit_test_iec60730_imc_mock_init_run_crc(void);

/**************************************************************************/ /**
 * Public   Function simulates the result of variable value integrity check.
 *
 * @returns boolean.
 *
 *****************************************************************************/
extern sl_iec60730_test_result_t unit_test_mock_check_integrity(void);

/**************************************************************************/ /**
 * Public   Function mock enable gpcrc hardware operate.
 *
 * @returns boolean.
 *
 *****************************************************************************/
extern bool unit_test_iec60730_imc_mock_enable_gpcrc(void);

/**************************************************************************/ /**
 * Public   Function mock enable calculate crc by software.
 *
 * @returns boolean.
 *
 *****************************************************************************/
extern bool unit_test_iec60730_imc_mock_sw_enable_cal_crc(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_imc_init() operate
 *
 * @returns None.
 *
 * If the passed parameter value is NULL, the sl_iec60730_imc_init() function will fail.
 * When calling the function sl_iec60730_imc_post(), the value will be returned #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_imc_init_param_null(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_imc_post() operate
 *
 * @returns None.
 *
 * If the passed parameter value is valid, the sl_iec60730_imc_init() function will success.
 * When calling the function sl_iec60730_imc_post(), the value will be returned #SL_IEC60730_TEST_PASSED.
 *
 *****************************************************************************/
void test_sl_iec60730_imc_post_pass_check(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_imc_post() operate
 *
 * @returns None.
 *
 * In the case of #SL_IEC60730_CRC_DEBUG_ENABLE == 1, if the initial crc calculated
 * value is 0, or in the case of #SL_IEC60730_CRC_DEBUG_ENABLE == 0, the initial crc
 * calculated value is not equal to the value stored in #check_sum. The function
 * returns #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_imc_post_fail_check(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_imc_bist() operate
 *
 * @returns None.
 *
 * In the case of #SL_IEC60730_CRC_DEBUG_ENABLE == 1, if the value crc calculated
 * value is equal value calculated from sl_iec60730_imc_post(), or in the case of
 * #SL_IEC60730_CRC_DEBUG_ENABLE == 0, the initial crc calculated value is equal to
 * the value stored in #check_sum. The function returns# SL_IEC60730_TEST_PASSED.
 *
 *****************************************************************************/
void test_sl_iec60730_imc_bist_pass_all_check(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_imc_bist() operate
 *
 * @returns None.
 *
 * In the case of #SL_IEC60730_CRC_DEBUG_ENABLE == 1, if the value crc calculated
 * value is equal value calculated from sl_iec60730_imc_post(), or in the case of
 * #SL_IEC60730_CRC_DEBUG_ENABLE == 0, the initial crc calculated value is equal to
 * the value stored in #check_sum. The function sl_iec60730_imc_bist() returns #SL_IEC60730_TEST_PASSED.
 *
 *****************************************************************************/
void test_sl_iec60730_imc_bist_pass_all_check(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_imc_bist() operate
 *
 * @returns None.
 *
 * If the check integrity of the pointer fails, the function sl_iec60730_imc_bist() returns #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_imc_bist_fail_check_integrity(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_imc_bist() operate
 *
 * @returns None.
 *
 * If **iec60730_run_crc** init greater than rom end address before **iec60730_cur_crc** is calculated.
 * Then the function sl_iec60730_imc_bist() returns #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_imc_bist_iec60730_run_crc_greater_than_rom_end_address(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_imc_bist() operate
 *
 * @returns None.
 *
 * In the case of #SL_IEC60730_CRC_DEBUG_ENABLE == 1, if the value crc calculated
 * value is not equal value calculated from sl_iec60730_imc_post(), or in the case of
 * #SL_IEC60730_CRC_DEBUG_ENABLE == 0, the initial crc calculated value is not equal to
 * the value stored in #check_sum. The function sl_iec60730_imc_bist() returns #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_imc_bist_fail_compare_crc(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_update_crc_with_data_buffer() operate
 *
 * @returns None.
 *
 * If the passed parameter value is NULL, the sl_iec60730_update_crc_with_data_buffer() function will fail.
 * returns #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_update_crc_with_data_buffer_params_null(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_update_crc_with_data_buffer() operate
 *
 * @returns None.
 *
 * Calculate the CRC value of the input string, check whether the returned CRC value is correct or not.
 *
 *****************************************************************************/
void test_sl_iec60730_update_crc_with_data_buffer_calculation_crc(void);

#endif // UNIT_TEST_IEC60730_INVARIABLE_MEMORY_H

/** @} (end defgroup IEC60730_INVARIABLE_MEMORY_VERIFICATION_UNIT_TEST) */
/** @} (end addtogroup IEC60730_UNIT_TEST) */
