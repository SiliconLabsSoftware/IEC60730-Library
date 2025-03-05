/***************************************************************************/ /**
 * @file  unit_test_iec60730_cpu_registers.h
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

/**************************************************************************/ /**
 * @addtogroup IEC60730_UNIT_TEST
 * @{
 * @defgroup IEC60730_CPU_REGISTERS_VERIFICATION_UNIT_TEST CPU Registers Verification Unit Tests
 * @{
 * Header file list of test cases for the verification operate functions in cpu registers module.
 *
 * IEC60730 CPU REGISTERS Unit Tests
 *
 * @details These test cases run unit tests to check the operation of the function in cpu registers module.
 *
 * Hardware setup:
 *
 * - Connect DUT to PC. No additional hardware setup required.
 ******************************************************************************/

#ifndef UNIT_TEST_IEC60730_CPU_REGISTERS_H
#define UNIT_TEST_IEC60730_CPU_REGISTERS_H

/*======= External Functions =====*/

/**************************************************************************/ /**
 * Public   Check sl_iec60730_cpu_registers_bist() operate
 *
 * @returns None.
 *
 * If the register checks in the function sl_iec60730_cpu_registers_bist() pass,
 * then #sl_iec60730_program_counter_check will be set #IEC60730_CPU_REGS_COMPLETE.
 *
 *****************************************************************************/
void test_sl_iec60730_cpu_registers_bist_pass_all_check_condition(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_cpu_registers_bist() operate
 *
 * @returns None.
 *
 * If one of the register checks in the function sl_iec60730_cpu_registers_bist() failed,
 * then #sl_iec60730_program_counter_check will be 0.
 *
 *****************************************************************************/
void test_sl_iec60730_cpu_registers_bist_failed_check_condition(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_cpu_registers_bist_ns() operate
 *
 * @returns None.
 *
 * This test case only checks when the chip supports is cortex M33.
 * If the register checks in the function sl_iec60730_cpu_registers_bist_ns() pass,
 * then function will be returned #SL_IEC60730_TEST_PASSED.
 *
 *****************************************************************************/
void test_sl_iec60730_cpu_registers_bist_ns_pass_all_check_condition(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_cpu_registers_bist_ns() operate
 *
 * @returns None.
 *
 * This test case only checks when the chip supports is cortex M33.
 * If one of the register checks in the function sl_iec60730_cpu_registers_bist_ns() failed,
 * then function will be returned #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_cpu_registers_bist_ns_failed_check_condition(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_cpu_registers_bist_s() operate
 *
 * @returns None.
 *
 * This test case only checks when the chip supports is cortex M33.
 * If the register checks in the function sl_iec60730_cpu_registers_bist_s() pass,
 * then function will be returned #SL_IEC60730_TEST_PASSED.
 *
 *****************************************************************************/
void test_sl_iec60730_cpu_registers_bist_s_pass_all_check_condition(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_cpu_registers_bist_s() operate
 *
 * @returns None.
 *
 * This test case only checks when the chip supports is cortex M33.
 * If one of the register checks in the function sl_iec60730_cpu_registers_bist_s() failed,
 * then function will be returned #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_cpu_registers_bist_s_failed_check_condition(void);

#endif // UNIT_TEST_IEC60730_CPU_REGISTERS_H

/** @} (end defgroup IEC60730_CPU_REGISTERS_VERIFICATION_UNIT_TEST) */
/** @} (end addtogroup IEC60730_UNIT_TEST) */
