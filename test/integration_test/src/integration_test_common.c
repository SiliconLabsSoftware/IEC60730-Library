/***************************************************************************/ /**
 * @file  integration_test_common.c
 * @brief Integration test init
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "integration_test_common.h"

// Watchdog config
const WDOG_Init_TypeDef integration_test_wdog0_init  = WDOG_INIT_DEFAULT;
const WDOG_Init_TypeDef integration_test_wdog1_init = WDOG_INIT_DEFAULT;
// Timer config
const TIMER_Init_TypeDef integration_test_timer_10ms_config    = TIMER_INIT_TEST_10MS;
const LETIMER_Init_TypeDef integration_test_timer_100ms_config = TIMER_INIT_TEST_100MS;

void nvic_init(void)
{
  // One Group Priority
  NVIC_SetPriorityGrouping(7);
  /* Timer 0 */
  NVIC_SetPriority(TIMER0_IRQn, 1);
  NVIC_EnableIRQ(TIMER0_IRQn);
  /* LETIMER 0 */
  NVIC_SetPriority(LETIMER0_IRQn, 1);
  NVIC_EnableIRQ(LETIMER0_IRQn);
}

/* Function Definition */
void integration_test_init(void)
{
  integration_test_clock_config();
  nvic_init();
#if defined(_RMU_CTRL_PINRMODE_MASK)
  RMU_ResetControl(rmuResetWdog, rmuResetModeExtended);
#endif // defined(_RMU_CTRL_PINRMODE_MASK)
#if (defined(WDOG0) && (SL_IEC60730_WDOG0_ENABLE == 1))
  WDOGn_Init(SL_IEC60730_WDOG_INST(0), &integration_test_wdog0_init);
#endif
#if (defined(WDOG1) && (SL_IEC60730_WDOG1_ENABLE == 1))
  WDOGn_Init(SL_IEC60730_WDOG_INST(1), &integration_test_wdog1_init);
#endif
  CORE_ATOMIC_IRQ_ENABLE();
}

void integration_test_deinit(void)
{
  /* Disable watchdog timer */
#if (defined(WDOG0) && (SL_IEC60730_WDOG0_ENABLE == 1))
  WDOGn_Enable(SL_IEC60730_WDOG_INST(0), false);
#endif
#if (defined(WDOG1) && (SL_IEC60730_WDOG1_ENABLE == 1))
  WDOGn_Enable(SL_IEC60730_WDOG_INST(1), false);
#endif
}

/* Clock configuration */
void integration_test_clock_config(void)
{
#if (_SILICON_LABS_32B_SERIES_2_CONFIG >= 1)
  /* System clock is default FSRCO - 20Mhz */
  /* Watchdog and LETIMER clock is default LFRCO - 32Khz */
  /* FSRCO clocks for timers at fixed 20Mhz */
  CMU_ClockSelectSet(cmuClock_TIMER0, cmuSelect_FSRCO);

  CMU_ClockSelectSet(cmuClock_LETIMER0, cmuSelect_LFRCO);

#if (_SILICON_LABS_32B_SERIES_2_CONFIG > 1)
  CMU_ClockEnable(cmuClock_TIMER0, true);
  CMU_ClockEnable(cmuClock_LETIMER0, true);
#if (defined(WDOG0) && (SL_IEC60730_WDOG0_ENABLE == 1))
  CMU_ClockEnable(cmuClock_WDOG0, true);
#endif
#if (defined(WDOG1) && (SL_IEC60730_WDOG1_ENABLE == 1))
  CMU_ClockEnable(cmuClock_WDOG1, true);
#endif
  CMU_ClockEnable(cmuClock_GPCRC, true);
  CMU_ClockEnable(cmuClock_BURAM, true);
  CMU_ClockEnable(cmuClock_FSRCO, true);
  CMU_ClockEnable(cmuClock_LFRCO, true);
#endif
#else
  /* System clock is default HFRCO - 19Mhz */
  /* Watchdog clock is default LFRCO - 32Khz */
  /* FSRCO clocks for timers at 19Mhz */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);
/* Enable Low energy Peripherals clock */
#if defined(CMU_HFCORECLKEN0_LE)
  CMU_ClockEnable(cmuClock_HFLE, true);
#else
  CMU->HFBUSCLKEN0 |= (CMU_HFBUSCLKEN0_LE & _CMU_HFBUSCLKEN0_MASK);
#endif

  /* LFACLK clock is selected as LFRCO */
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
  /* Enable clock for LETIMER0 as LFRCO - 32.768 kHz*/
  CMU_ClockEnable(cmuClock_LETIMER0, true);
  /* Enable clock of CRC */
  CMU_ClockEnable(cmuClock_GPCRC, true);
#endif
}

/* Init timer test */
void integration_test_timers_init(void)
{
  /* Initialize 10ms timer */
  TIMER_Init(TIMER_10MS, &integration_test_timer_10ms_config);
  /* Initialize 10ms timer */
  LETIMER_Init(TIMER_100MS, &integration_test_timer_100ms_config);

  /* Reset counter */
  TIMER_CounterSet(TIMER_10MS, TEST_TIMER_RESET_VAL);
  LETIMER_CounterSet(TIMER_100MS, TEST_TIMER_PRESET_VAL);

  /* Set reload value */
  TIMER_TopSet(TIMER_10MS, TEST_TIMER_10MS_VAL);
  LETIMER_TopSet(TIMER_100MS, TEST_TIMER_100MS_VAL);

  /* Clear pending interrupt */
  TIMER_IntClear(TIMER_10MS, TIMER_IF_OF);
  LETIMER_IntClear(TIMER_100MS, LETIMER_IF_UF);

  /* Enable overflow interrupt */
  TIMER_IntEnable(TIMER_10MS, TIMER_IF_OF);
  LETIMER_IntEnable(TIMER_100MS, LETIMER_IEN_UF);
}

/* Enable timer to test interrupt */
void integration_test_timers_enable(void)
{
  // Start timer
  TIMER_Enable(TIMER_10MS, true);
  LETIMER_Enable(TIMER_100MS, true);
}

/* Disable timer to test interrupt */
void integration_test_timers_disable(void)
{
  // Start timer
  TIMER_Enable(TIMER_10MS, false);
  LETIMER_Enable(TIMER_100MS, false);
}

// Dump functions
__WEAK sl_iec60730_test_result_t sl_iec60730_vmc_bist()
{
  return SL_IEC60730_TEST_PASSED;
}

__WEAK sl_iec60730_test_result_t sl_iec60730_imc_bist()
{
  return SL_IEC60730_TEST_PASSED;
}

__WEAK sl_iec60730_test_result_t sl_iec60730_cpu_registers_bist(void)
{
  return SL_IEC60730_TEST_PASSED;
}

__WEAK sl_iec60730_test_result_t sl_iec60730_vmc_post()
{
  return SL_IEC60730_TEST_PASSED;
}

__WEAK sl_iec60730_test_result_t sl_iec60730_imc_post()
{
  return SL_IEC60730_TEST_PASSED;
}

__WEAK sl_iec60730_test_result_t sl_iec60730_watchdog_post(void)
{
  return SL_IEC60730_TEST_PASSED;
}

__WEAK sl_iec60730_test_result_t sl_iec60730_cpu_registers_post(void)
{
  return SL_IEC60730_TEST_PASSED;
}

/* IEC60730 safe state */
void sl_iec60730_safe_state(sl_iec60730_test_failure_t failure)
{
  printf("Fail-Test. Error status: %d\n", failure);
  LABEL_DEF(IEC60730_SAFE_STATE_BKPT);
  SL_IEC60730_RSTCAUSES_CLEAR();
  while (1) {
    sl_iec60730_restart_watchdogs();
  }
}
