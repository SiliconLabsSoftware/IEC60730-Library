/***************************************************************************/ /**
 * @file  unit_test_iec60730_safety_check.h
 * @brief Unit test function sl_iec60730_safety_check_reset_error() and sl_iec60730_safety_check_error_occur().
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
 * @defgroup IEC60730_SAFETY_CHECK_VERIFICATION_UNIT_TEST Safety Check Verification Unit Tests
 * @{
 * Header file list of test cases for the verification operate function sl_iec60730_safety_check_reset_error() and sl_iec60730_safety_check_error_occur().
 *
 * IEC60730 Safety Check Unit Tests
 *
 * @details These test cases run unit tests to check the operation of the function sl_iec60730_safety_check_reset_error() and sl_iec60730_safety_check_error_occur()
 *
 * Hardware setup:
 *
 * - Connect DUT to PC. No additional hardware setup required.
 ******************************************************************************/

#ifndef UNIT_TEST_IEC60730_SAFETY_CHECK_H
#define UNIT_TEST_IEC60730_SAFETY_CHECK_H

/*======= External Functions =====*/

/**************************************************************************/ /**
 * Public   Check sl_iec60730_safety_check_error_occur() operate
 *
 * @returns None.
 *
 * If **iec60730_safety_check.error** value get from sl_iec60730_safety_check_get_error()
 * equal error passed in via function sl_iec60730_safety_check_error_occur(). Then test case
 * is pass.
 *
 *****************************************************************************/
void test_sl_iec60730_safety_check_error_occur(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_safety_check_reset_error() operate
 *
 * @returns None.
 *
 * If **iec60730_safety_check.error** and **iec60730_safety_check.number_error** value
 * get from sl_iec60730_safety_check_get_error() is 0. Then test case is pass.
 *
 *****************************************************************************/
void test_sl_iec60730_safety_check_reset_error(void);

#endif // UNIT_TEST_IEC60730_SAFETY_CHECK_H

/** @} (end defgroup IEC60730_SAFETY_CHECK_VERIFICATION_UNIT_TEST) */
/** @} (end addtogroup IEC60730_UNIT_TEST) */
