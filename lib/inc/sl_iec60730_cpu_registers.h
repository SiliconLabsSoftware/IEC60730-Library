/***************************************************************************/ /**
 * @file
 * @brief CPU Registers check
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

#ifndef SL_IEC60730_CPU_REGISTERS_H
#define SL_IEC60730_CPU_REGISTERS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "sl_iec60730_internal.h"

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup IEC60730_CPU_REG_TEST
 * @{
 * @section cpureg_hardware_architecture Hardware Architecture
 * The CM33 CPU registers include the following registers:
 *
 * General-purpose registers:
 *      • R0-R12
 *
 * Stack pointer registers:
 *      • MSP + MSPLIM (secure/non-secure)
 *      • PSP + PSPLIM (secure/non-secure)
 *
 * Special registers:
 *      • APSR • CONTROL (secure/non-secure)
 *      • PRIMASK (secure/non-secure)
 *      • FAULTMASK (secure/non-secure)
 *      • BASEPRI (secure/non-secure)
 *
 * Link register:
 *      • LR
 *
 * FPU registers:
 *      • FPSCR
 *      • S0 – S31
 *
 * There is a set of tests performed once after the MCU reset and also during
 * runtime.
 *
 * @section cpureg_failure_risks Failure Risks
 * The primary risks with the CPU registers are stuck bits and shorted bits.
 * The CPU register tests will verify that bits can be set high and low, and
 * adjacent bits are not shorted.
 *
 * @section cpureg_software_architecture Software Architecture
 * Because internal CPU registers are being tested, the code must be written
 * using assembly language.
 *
 * The identification of safety errors is ensured by the specific FAIL return
 * if some registers have the stuck-at fault. Assess the return value of every
 * function. If the value equals the FAIL return, then a jump into the safety
 * error handling function should occur. The safety error handling function may
 * be specific to the application and it is not a part of the library. The main
 * purpose of this function is to put the application into a safe state.
 *
 * In some special cases, the error is not reported by the FAIL return, because
 * it would require the action of a corrupt register. In that case, the function
 * waits for reset in an endless loop. The principle of the stuck-at error test
 * of the CPU registers is to write and compare two test patterns in every
 * register. The content of the register is compared with the constant or with
 * the value written into another register that was tested before. Most of the
 *  time, R0, R1, and R2 are used as auxiliary registers. Patterns are defined
 * to check the logical one and logical zero values in all register bits.
 *
 * Due to the Arm TrustZone support, some core registers are banked between the
 * security states. The Secure (S) or Non-Secure (NS) sets of the corresponding
 * registers are accessible during execution (depending on the current security
 * state). Both register versions are accessible during the S state, but not
 * during the NS state. This is the reason why the NXP Safety Library must be
 * executed in a secure mode. All of the banked registers are listed above.
 *
 * For the EFR32 IEC60730 implementation, \link cpu_registers_check_flowchart Figure 1\endlink
 * shows the logic flow chart of the CPU registers check during BIST and POST.
 *
 * \image html cpu_registers_check_flowchart.png "Figure 1 CPU registers check flowchart" \anchor cpu_registers_check_flowchart
 *
 *****************************************************************************/

/**************************************************************************/ /**
 * public IEC60730 CPU Registers Built In Self Test
 *
 * @returns #sl_iec60730_test_result_t
 * @returns #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function will do a test of all the CPU core registers in non-secure state
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_bist_ns(void);

/**************************************************************************/ /**
 * public IEC60730 CPU Registers Built In Self Test
 *
 * @returns #sl_iec60730_test_result_t
 * @returns #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function will do a test of all the CPU core registers in secure state
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_bist_s(void);

/**************************************************************************/ /**
 * public IEC60730 CPU Registers Built In Self Test
 *
 * @returns #sl_iec60730_test_result_t
 * @returns #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function will do a test of all the CPU core registers in non-secure and secure state.
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_bist(void);

/**************************************************************************/ /**
 * public IEC60730 CPU Registers Power On Self Test
 *
 * @returns #sl_iec60730_test_result_t
 *
 * This function simply calls the sl_iec60730_cpu_registers_bist() function.
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_post(void);

/** @} (end addtogroup IEC60730_CPU_REG_TEST) */
/** @} (end addtogroup efr32_iec60730) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SL_IEC60730_CPU_REGISTERS_H */
