/***************************************************************************/ /**
 * @file  unit_test_iec60730_system_clock.h
 * @brief Unit tests for functions in system_clock module.
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
 * @defgroup IEC60730_SYSTEM_CLOCK_VERIFICATION_UNIT_TEST System Clock Verification Unit Tests
 * @{
 * Header file list of test cases for the verification operate functions in system_clock module.
 *
 * IEC60730 System Clock Unit Tests
 *
 * @details These test cases run unit tests to check the operation of the functions in system_clock module.
 *
 * Hardware setup:
 *
 * - Connect DUT to PC. No additional hardware setup required.
 ******************************************************************************/

#ifndef UNIT_TEST_IEC60730_SAFETY_CHECK_H
#define UNIT_TEST_IEC60730_SAFETY_CHECK_H

/*======= External Functions =====*/

/**************************************************************************/ /**
 * Public   Check sl_iec60730_test_clock_tick() operate
 *
 * @returns None.
 *
 * If sl_iec60730_timer_test_control equals #SL_IEC60730_TIMER_TEST_DISABLE.
 * Function sl_iec60730_safe_state() will be called.
 *
 *****************************************************************************/
void test_sl_iec60730_test_clock_tick_timer_test_disable(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_test_clock_tick() operate
 *
 * @returns None.
 *
 * If sl_iec60730_timer_test_control equals #SL_IEC60730_TIMER_TEST_ENABLE.
 * Call function sl_iec60730_sys_clock_count_tick() to increase the value of
 * variable **sl_iec60730_sys_clock_count**. If the **sl_iec60730_sys_clock_count** value is
 * within the check boundary value. Function sl_iec60730_safe_state() will not be called.
 *
 *****************************************************************************/
void test_sl_iec60730_test_clock_tick_count_in_bound(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_test_clock_tick() operate
 *
 * @returns None.
 *
 * If **sl_iec60730_timer_test_control** equals #SL_IEC60730_TIMER_TEST_ENABLE.
 * Call function sl_iec60730_sys_clock_count_tick() to increase the value of
 * variable **sl_iec60730_sys_clock_count**. If the **sl_iec60730_sys_clock_count** value is
 * out of range the check boundary value. Function sl_iec60730_safe_state() will be called.
 *
 *****************************************************************************/
void test_sl_iec60730_test_clock_tick_count_out_bound(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_test_clock_tick() operate
 *
 * @returns None.
 *
 * If **sl_iec60730_timer_test_control** equals #SL_IEC60730_TIMER_TEST_ENABLE.
 * Call function sl_iec60730_sys_clock_count_tick() to increase the value of
 * variable **sl_iec60730_sys_clock_count**. If the **sl_iec60730_sys_clock_count** value is
 * within the check boundary value. Then call function sl_iec60730_sys_clock_count_reset()
 * to reset variable **sl_iec60730_sys_clock_count**. Function sl_iec60730_safe_state() will be called.
 *
 *****************************************************************************/
void test_sl_iec60730_sys_clock_count_reset(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_test_clock_tick() operate
 *
 * @returns None.
 *
 * The number of function sl_iec60730_test_clock_tick() calls is greater than the default value and
 * variable **sl_iec60730_sys_clock_count value** is within the check boundary value each time call.
 * Function sl_iec60730_program_counter_test() will be called.
 *
 *****************************************************************************/
void test_sl_iec60730_test_clock_tick_test_clock_multiplier(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_get_number_test_timer_tick() operate
 *
 * @returns None.
 *
 * Check if the number of times the function sl_iec60730_test_clock_tick() has been called is
 * the same as the value received from the function sl_iec60730_get_number_test_timer_tick().
 *
 *****************************************************************************/
void test_sl_iec60730_get_number_test_timer_tick(void);

#endif // UNIT_TEST_IEC60730_SAFETY_CHECK_H

/** @} (end defgroup IEC60730_SYSTEM_CLOCK_VERIFICATION_UNIT_TEST) */
/** @} (end addtogroup IEC60730_UNIT_TEST) */
