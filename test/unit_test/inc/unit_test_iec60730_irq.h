/***************************************************************************/ /**
 * @file  unit_test_iec60730_irq.h
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

/**************************************************************************/ /**
 * @addtogroup IEC60730_UNIT_TEST
 * @{
 * @defgroup IEC60730_IRQ_VERIFICATION_UNIT_TEST IRQ Verification Unit Tests
 * @{
 * Header file list of test cases for the verification operate functions in irq module.
 *
 * IEC60730 IRQ Unit Tests
 *
 * @details These test cases run unit tests to check the operation of the functions in irq module.
 *
 * Hardware setup:
 *
 * - Connect DUT to PC. No additional hardware setup required.
 ******************************************************************************/

#ifndef UNIT_TEST_IEC60730_IRQ_H
#define UNIT_TEST_IEC60730_IRQ_H

/*======= External Functions =====*/

/**************************************************************************/ /**
 * Public  Check sl_iec60730_irq_init() operate
 *
 * @returns None.
 *
 * If the passed param is NULL after call sl_iec60730_irq_check(),
 * function sl_iec60730_safe_state() will be called (variable local **is_function_called**  set true).
 *
 *****************************************************************************/
void test_sl_iec60730_irq_init_param_null(void);

/**************************************************************************/ /**
 * Public  Check sl_iec60730_irq_init() operate
 *
 * @returns None.
 *
 * If the param greater than #IEC60730_MAX_IRQ_CHECK, function sl_iec60730_safe_state() will be called
 * (variable local **is_function_called**  set true).
 *
 *****************************************************************************/
void test_sl_iec60730_irq_init_param_size_greater_than_max_irq(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_irq_check() operate
 *
 * @returns None.
 *
 * After successful initialization, after calling function.
 * If the test element has an irq_count between the **irq_bounds** max and min values,
 * function sl_iec60730_safe_state() is not called.
 *
 *****************************************************************************/
void test_sl_iec60730_irq_check_count_in_bound(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_irq_check() operate
 *
 * @returns None.
 *
 * After successful initialization, after calling function.
 * If the test element has an irq_count out of range the **irq_bounds** max and min values,
 * function sl_iec60730_safe_state() is called.
 *
 *****************************************************************************/
void test_sl_iec60730_irq_check_count_out_bound(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_irq_reset_counter() operate
 *
 * @returns None.
 *
 * After calling this function, check the value of the **irq_count** element of the config variable
 * passed to function sl_iec60730_irq_init(). If the value is 0, the test case passes.
 *
 *****************************************************************************/
void test_sl_iec60730_irq_reset_counter(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_irq_fail_occur() operate
 *
 * @returns None.
 *
 * Check if the value of wrong interrupt count and wrong interrupt index passed into the function
 * is equal to the value received from function sl_iec60730_get_irq_index_failed(), then test passes.
 *
 *****************************************************************************/
void test_sl_iec60730_irq_fail_occur(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_irq_reset_fail_result() operate
 *
 * @returns None.
 *
 * Check if the value of wrong interrupt count and wrong interrupt position received from
 * function sl_iec60730_get_irq_index_failed() is 0 after calling sl_iec60730_irq_reset_fail_result()
 * then test passes.
 *
 *****************************************************************************/
void test_sl_iec60730_irq_reset_fail_result(void);

#endif // UNIT_TEST_IEC60730_IRQ_H

/** @} (end defgroup IEC60730_IRQ_VERIFICATION_UNIT_TEST) */
/** @} (end addtogroup IEC60730_UNIT_TEST) */
