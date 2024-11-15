/***************************************************************************/ /**
 * @file  unit_test_iec60730_post.h
 * @brief Unit test function sl_iec60730_post().
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
 * @defgroup IEC60730_UNIT_TEST_POST Post Verification Unit Tests
 * @{
 * Header file list of test cases for the verification operate function sl_iec60730_post().
 *
 * IEC60730 Post Unit Tests
 *
 * @details These test cases run unit tests to check the operation of the function sl_iec60730_post()
 *
 * Hardware setup:
 *
 *   - Connect DUT to PC. No additional hardware setup required.
 ******************************************************************************/

#ifndef UNIT_TEST_IEC60730_POST_H
#define UNIT_TEST_IEC60730_POST_H

/*======= External Functions =====*/

/**************************************************************************/ /**
 * Public   Check sl_iec60730_post() operate
 *
 * @returns None.
 *
 * If bool local **is_function_called**  variable equal false. It means test result routine of sl_iec60730_post()
 * is a pass, function sl_iec60730_safe_state() is not called. So timer support test is enabled.
 *
 *****************************************************************************/
void test_sl_iec60730_post_pass_all_check_condition(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_post() operate
 *
 * @returns None.
 *
 * If bool local **is_function_called**  variable true. It means test result routine of sl_iec60730_post()
 * is a failure, function sl_iec60730_safe_state() is called.
 *
 *****************************************************************************/
void test_sl_iec60730_post_failed_check_condition(void);

#endif // UNIT_TEST_IEC60730_POST_H

/** @} (end defgroup IEC60730_POST_VERIFICATION_UNIT_TEST) */
/** @} (end addtogroup IEC60730_UNIT_TEST) */
