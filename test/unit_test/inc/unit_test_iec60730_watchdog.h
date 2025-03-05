/***************************************************************************/ /**
 * @file  unit_test_iec60730_watchdog.h
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

/**************************************************************************/ /**
 * @addtogroup IEC60730_UNIT_TEST
 * @{
 * @defgroup IEC60730_WATCHDOG_VERIFICATION_UNIT_TEST Watchdog Verification Unit Tests
 * @{
 * Header file list of test cases for the verification operate functions in watchdog module.
 *
 * IEC60730 Watchdog Unit Tests
 *
 * @details These test cases run unit tests to check the operation of the functions in watchdog module.
 *
 * Hardware setup:
 *
 * - Connect DUT to PC. No additional hardware setup required.
 ******************************************************************************/

#ifndef UNIT_TEST_IEC60730_WATCHDOG_H
#define UNIT_TEST_IEC60730_WATCHDOG_H

/*======= External Functions =====*/

/**************************************************************************/ /**
 * Public   Function setup condition before running test case.
 *          Only used for unit testing.
 *
 * @returns None.
 *
 *****************************************************************************/
void unit_test_iec60730_watchdog_set_up(void);

/**************************************************************************/ /**
 * Public   Function replaces the value for the device being reset by Power On Reset.
 *          Only used for unit testing.
 *
 * @returns boolean.
 *
 *****************************************************************************/
bool unit_test_iec60730_watchdog_mock_rst_por(void);

/**************************************************************************/ /**
 * Public   Function replaces the value for the device being reset by EM4 Wakeup.
 *          Only used for unit testing.
 *
 * @returns boolean.
 *
 *****************************************************************************/
bool unit_test_iec60730_watchdog_mock_rst_em4(void);

/**************************************************************************/ /**
 * Public   Function replaces the value for the device being reset by Watchdog 0.
 *          Only used for unit testing.
 *
 * @returns boolean.
 *
 *****************************************************************************/
bool unit_test_iec60730_watchdog_mock_rst_wdog0(void);

/**************************************************************************/ /**
 * Public   Function replaces the value for the device being reset by Watchdog 1.
 *          Only used for unit testing.
 *
 * @returns boolean.
 *
 *****************************************************************************/
bool unit_test_iec60730_watchdog_mock_rst_wdog1(void);

/**************************************************************************/ /**
 * Public   Function replaces watchdog timeout min set value successfully or not.
 *          Only used for unit testing.
 *
 * @returns boolean.
 *
 *****************************************************************************/
bool unit_test_iec60730_watchdog_mock_set_watchdog_timout_min(void);

/**************************************************************************/ /**
 * Public   Function replaces watchdog clear register rstcause.
 *          Only used for unit testing.
 *
 * @returns None.
 *
 *****************************************************************************/
void unit_test_iec60730_watchdog_mock_rstcause_clear(void);

/**************************************************************************/ /**
 * Public   The function replaces the value of the RSTCAUSE register indicating the cause of the reset.
 *          Only used for unit testing
 *
 * @returns Value RSTCAUSE register.
 *
 *****************************************************************************/
uint32_t unit_test_iec60730_watchdog_mock_rstcause(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_watchdog_post() operate
 *
 * @returns None.
 *
 * If reset causes are not POR and not watchdog reset then no action. Value return by
 * function sl_iec60730_watchdog_post() is #SL_IEC60730_TEST_PASSED.
 *
 *****************************************************************************/
void test_sl_iec60730_watchdog_post_reset_causes_not_por_and_watchdog(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_watchdog_post() operate
 *
 * @returns None.
 *
 * When the device is reset by power on reset after watchdog reset, the variable #iec60730_watchdog_state does not
 * set to the value #SL_IEC60730_WATCHDOG_TESTING, then check return value is #SL_IEC60730_TEST_FAILED
 *
 *****************************************************************************/
void test_sl_iec60730_watchdog_post_wachdog_reset_before_por(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_watchdog_post() operate
 *
 * @returns None.
 *
 * When the device is reset by power on reset first set the variable #iec60730_watchdog_state
 * to the value #SL_IEC60730_WATCHDOG_TESTING, then check the watchdog reset.
 * If sli_iec60730_set_watchdog_timout_min() is set successfully,
 * and the device does not wake up from EM4, all Watchdogs are tested then finished.
 * The cause of the reset is by watchdog, the function sl_iec60730_watchdog_post()
 * return result is #SL_IEC60730_TEST_PASSED
 *
 *****************************************************************************/
void test_sl_iec60730_watchdog_post_set_watchdog_timout_min_success(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_watchdog_post() operate
 *
 * @returns None.
 *
 * When the device is reset by power on reset first set the variable #iec60730_watchdog_state
 * to the value #SL_IEC60730_WATCHDOG_TESTING, then check the watchdog reset.
 * If sli_iec60730_set_watchdog_timout_min() is failed, then after call sl_iec60730_watchdog_post()
 * again, return result is #SL_IEC60730_TEST_FAILED
 *
 *****************************************************************************/
void test_sl_iec60730_watchdog_post_set_watchdog_timout_min_fail(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_watchdog_post() operate
 *
 * @returns None.
 *
 * When the device is reset by power on reset first set the variable #iec60730_watchdog_state
 * to the value #SL_IEC60730_WATCHDOG_TESTING, then check the watchdog reset.
 * If sli_iec60730_set_watchdog_timout_min() is set successfully,
 * and the device does not wake up from EM4, all Watchdogs are tested then finished.
 * The cause of the reset is not by watchdog, the function sl_iec60730_watchdog_post()
 * return result is #SL_IEC60730_TEST_PASSED
 *
 *****************************************************************************/
void test_sl_iec60730_watchdog_post_reset_resason_no_valid(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_watchdog_post() operate
 *
 * @returns None.
 *
 * Check the function sl_iec60730_watchdog_post() operation when checking condition to
 * prevent unexpected changed value when reset or return from power saving mode.
 * If **iec60730_watchdog_count** value >= #SL_IEC60730_WDOGINST_NUMB after device
 * power on reset, the function sl_iec60730_watchdog_post() return result is
 * #SL_IEC60730_TEST_FAILED
 *
 *****************************************************************************/
void test_sl_iec60730_watchdog_post_prevent_unexpected_changed_value(void);

#endif // UNIT_TEST_IEC60730_WATCHDOG_H

/** @} (end defgroup IEC60730_WATCHDOG_VERIFICATION_UNIT_TEST) */
/** @} (end addtogroup IEC60730_UNIT_TEST) */
