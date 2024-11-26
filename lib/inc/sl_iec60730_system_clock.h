/***************************************************************************/ /**
 * @file
 * @brief Clock check
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

#ifndef SL_IEC60730_SYSTEM_CLOCK_H
#define SL_IEC60730_SYSTEM_CLOCK_H

#include <stdint.h>
#include "sl_iec60730_toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup IEC60730_SYSTEM_CLOCK_TEST
 * @{
 * @details
 * For IEC60730, a plausibility check must be performed to check that the
 * system clock is running at a frequency with an expected bounds.  This
 * check requires that the system clock frequency be compared against a second,
 * independent clock's frequency (test clock).
 *
 * @section system_clock_hardware_architecture Hardware Architecture
 * EFR32 family devices provide a high frequency internal oscillator as well
 * as a low frequency internal oscillator.  The devices also provide timers
 * modules that can be configured to use either oscillator as a clock
 * source.  These peripherals can be configured to vector code to interrupt
 * service routines when the counter incremented by the peripheral and clocked
 * by one of the clock sources meets certain criteria.
 *
 * For timers, the criteria is a counter overflow event. The frequency of
 * this overflow is defined by a reload value configured in firmware that hardware
 * automatically loads into the timer's counter upon overflow.
 *
 * Some timers offer a special mode where the timer's clock source is gated
 * by a falling edge of the low frequency oscillator.  In these cases, the
 * system clock can be used as the clock source, and the number of timer counter
 * ticks captured during the low frequency oscillator's logic low phase expresses
 * the relationship between the two clocks.
 *
 * The define #SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ holds the ratio value between system clock timer interrupt period and
 * test clock timer interrupt period, this value is used to compare with the system clock counter. The ratio value is divided by slow
 * timer period (test clock timer interrupt period) and fast timer period (system clock timer interrupt period).
 *
 * The tolerance of test (OEM set value of the define, config: #SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE) is typical (+/-) 10%-49% of the #SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ.
 *
 * @section system_clock_suggested_OEM_configuration Suggested OEM Configuration
 *  Not every EFR32 device offers the same types of timers and clock sources.  For this
 * reason, the optimal resource usage on each device family varies.  The following
 * chart offers a suggested configuration for each EFR32 family.
 * Firmware examples configure system and test timers as in \link system_clock_test_suggested_oem_configuration_chart Table 1 \endlink
 *
 * <center><b>Table 1 Suggested system and test timer configuration on EFR32 device families</b></center> \anchor system_clock_test_suggested_oem_configuration_chart
 * <table>
 * <tr><th>EFR32 device <th>System timer                                                              <th>Test timer
 * <tr><td>Series 1<td><ul><li>TIMERn clocked by system clock HFCLK which is the master clock of the
 *                       core and other peripherals.</li>
 *                     <li>Special case: If the ADC module is clocked by AUXHFRCO then OEM could
 *                       use IRQCheck to measure clock  indirectly via sample rate interrupt.</li></ul>
 *                 <td><ul><li>LETIMER0 clocked by internal fixed clock
 *                       (LFRCO or ULFRCO) with configurable prescaler.</li></ul>
 * <tr><td>Series 2<td><ul><li>TIMERn clocked by one of the HFXO, HFRCODPLL, HFRCOEM23, FSRCO clock
 *                       sources that could be used to test for CPU, high speed peripherals
 *                       (SYSCLK), Radio module (HFXO) and IADC module (HFRCOEM23).</li>
 *                     <li>WDOGn clocked by HCLK when system clock (SYSCLK) is selected as CLKIN0</li></ul>
 *                 <td><ul><li>LETIMERn clocked by internal fixed clock
 *                      (LFRCO or ULFRCO).</li></ul>
 * </table>
 *
 * @section system_clock_failure_risks Failure Risks
 * The library's system clock plausibility test will force entry into
 * safe state if the frequency relationship between the system clock and
 * the test clock exceeds upper and lower bounds.
 *
 * The library examines the relationship between the two clocks using calls
 * that the OEM must place in OEM-defined timer interrupt service routines.
 * Code examples for each EFR32 device provide drop-in firmware solutions for
 * OEMs for timer initialization and ISR source code.  OEMs must ensure that
 * timer ISRs are able to be serviced promptly during operation.
 *
 * Additionally, the OEM is responsible for leaving the system clock at
 * a constant and defined operating frequency throughout all OEM firmware.
 *
 * Finally, the OEM is responsible for setting **sl_iec60730_timer_test_control** to #SL_IEC60730_TIMER_TEST_DISABLE
 * within safety-critical code by calling function **sl_iec60730_sys_clock_test_disable()**. This value is only used in cases
 * where the system is executing non-safety-critical code, where the BIST routine is also not being called.
 * When returning to normal operation, clear all interrupt counters in
 * iec60730 IRQ execution count, reset the timer clock tick and system clock tick timers,
 * and set **sl_iec60730_timer_test_control** to #SL_IEC60730_TIMER_TEST_ENABLE.
 *
 * @section system_clock_software_architecture Software Architecture
 *
 * \link iec60730_wdog_info Table 1\endlink
 *
 * For the EFR32 IEC60730 implementation, \link BIST_system_clock_frequency Figure 1\endlink
 * shows the logic flow chart of the system clock frequency check during BIST. The test
 * executes in the test timer interrupt, which runs as foreground calls to OEM functions and
 * the foreground BIST test continuously execute.
 *
 * \image html bist_system_clock_frequency.png "Figure 1 System clock frequency check flowchart" \anchor BIST_system_clock_frequency
 *
 *****************************************************************************/

/**************************************************************************/ /**
 * public IEC60730 Get Timer Tick
 *
 * @returns sl_iec60730_number_test_timer_tick.
 *
 * Function get value sl_iec60730_number_test_timer_tick, used for OEM testing
 *****************************************************************************/
uint16_t sl_iec60730_get_number_test_timer_tick(void);

/**************************************************************************/ /**
 * public IEC60730 Enable Timer Test
 *
 * @returns None.
 *
 * Function set sl_iec60730_timer_test_control to SL_IEC60730_TIMER_TEST_ENABLE.
 *****************************************************************************/
void sl_iec60730_sys_clock_test_enable(void);

/**************************************************************************/ /**
 * public IEC60730 Disable Timer Test
 *
 * @returns None.
 *
 * Function set sl_iec60730_timer_test_control to SL_IEC60730_TIMER_TEST_DISABLE.
 *****************************************************************************/
void sl_iec60730_sys_clock_test_disable(void);

/** @} (end addtogroup IEC60730_SYSTEM_CLOCK_TEST) */
/** @} (end addtogroup efr32_iec60730) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SL_IEC60730_SYSTEM_CLOCK_H */
