/***************************************************************************/ /**
 * @file  unit_test_iec60730_variable_memory.h
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

/**************************************************************************/ /**
 * @addtogroup IEC60730_UNIT_TEST
 * @{
 * @defgroup IEC60730_VARIABLE_MEMORY_VERIFICATION_UNIT_TEST Variable Memory Verification Unit Tests
 * @{
 * Header file list of test cases for the verification operate functions in variable memory module.
 *
 *  IEC60730 Variable Memory Unit Tests
 *
 * @details These test cases run unit tests to check the operation of the functions in variable memory module.
 *
 * Hardware setup:
 *
 *  - Connect DUT to PC. No additional hardware setup required.
 ******************************************************************************/

#ifndef UNIT_TEST_IEC60730_VARIABLE_MEMORY_H
#define UNIT_TEST_IEC60730_VARIABLE_MEMORY_H

/*======= External Functions =====*/

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_init() operate
 *
 * @returns boolean.
 *
 * Function simulates the result of **iec60730_rt_check** variable value integrity check.
 *
 *****************************************************************************/
extern sl_iec60730_test_result_t unit_test_mock_check_integrity(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_init() operate
 *
 * @returns None.
 *
 * If the input parameter has the value NULL, the components in the initialization value are NULL,
 * the vmc test initialization flag fails. The function sl_iec60730_vmc_post() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_init_param_null(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_init() operate
 *
 * @returns None.
 *
 * If the input parameter has the value start member greater than value end member,
 * the vmc test initialization flag fails. The function sl_iec60730_vmc_post() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_init_start_address_greater_than_end_address(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_post() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchc algorithms to check the ram and
 * backup section pass, checking the integrity of the variable **iec60730_rt_check** pass. The function sl_iec60730_vmc_post()
 * returns the value #SL_IEC60730_TEST_PASSED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_post_pass_all_check_sections(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_post() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchc algorithms to check
 * the ram and backup section fail. The function sl_iec60730_vmc_post() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_post_fail_marchc_check_ram_section(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_post() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchc algorithms to check the backup
 * section fail. The function sl_iec60730_vmc_post() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_post_fail_marchc_check_backup_section(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_post() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchc algorithms to check the ram
 * section pass, but checking the integrity of the variable **iec60730_rt_check** fail.
 * The function sl_iec60730_vmc_post() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_post_fail_check_integrity_ram_section(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_post() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchc algorithms to check the backup
 * section pass, but checking the integrity of the variable **iec60730_rt_check** fail.
 * The function sl_iec60730_vmc_post() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_post_fail_check_integrity_backup_section(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_bist() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchxc algorithms to check the ram and
 * backup section pass, checking the integrity of the variable **iec60730_rt_check** pass, check stack over flow pass
 * The function sl_iec60730_vmc_bist() returns the value #SL_IEC60730_TEST_PASSED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_bist_pass_all_check_sections(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_bist() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchxc algorithms to check
 * the ram and backup section fail. The function sl_iec60730_vmc_bist() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_bist_fail_marchxc_check_ram_section(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_bist() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchxc algorithms to check the backup
 * section fail. The function sl_iec60730_vmc_bist() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_bist_fail_marchxc_check_backup_section(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_bist() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchxc algorithms to check the ram
 * section pass, but checking the integrity of the variable **iec60730_rt_check** fail.
 * The function sl_iec60730_vmc_bist() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_bist_fail_check_integrity_ram_section(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_bist() operate
 *
 * @returns None.
 *
 * If the initialization value is successful, the functions run the marchxc algorithms to check the backup
 * section pass, but checking the integrity of the variable **iec60730_rt_check** fail.
 * The function sl_iec60730_vmc_bist() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_bist_fail_check_integrity_backup_section(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_bist() operate
 *
 * @returns None.
 *
 * If sl_iec60730_vmc_pre_run_marchxc_step() returns false, meaning it is not allowed to run test
 * sl_iec60730_vmc_marchxc_step() to check the ram section. Function sl_iec60730_vmc_marchxc_step()
 * will not be called when checking ram section.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_bist_checking_allow_run_test(void);

/**************************************************************************/ /**
 * Public   Check sl_iec60730_vmc_bist() operate
 *
 * @returns None.
 *
 * If check that the value of the **iec60730_stack_test_over_flow** variable located in the ending stack area is
 * different from the initial initial value. The function sl_iec60730_vmc_bist() returns the value #SL_IEC60730_TEST_FAILED.
 *
 *****************************************************************************/
void test_sl_iec60730_vmc_bist_fail_stack_test_over_flow(void);

#endif // UNIT_TEST_IEC60730_VARIABLE_MEMORY_H

/** @} (end defgroup IEC60730_VARIABLE_MEMORY_VERIFICATION_UNIT_TEST) */
/** @} (end addtogroup IEC60730_UNIT_TEST) */
