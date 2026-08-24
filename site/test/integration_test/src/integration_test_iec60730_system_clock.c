/***************************************************************************/ /**
 * @file integration_test_iec60730_system_clock.c
 * @brief setup integration test system clock
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: LicenseRef-MSLA
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of the Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement
 * By installing, copying or otherwise using this software, you agree to the
 * terms of the MSLA.
 *
 ******************************************************************************/

#include "integration_test_common.h"

__WEAK void sl_iec60730_irq_check(void)
{
}

/* 10ms timer interrupt handler */
void TIMER0_IRQHandler(void)
{
  TIMER_IntClear(TIMER_10MS, TIMER_IF_OF);
  // Increase clock tick counter
  sl_iec60730_sys_clock_count_tick();
}

/* 100ms timer interrupt handler */
void LETIMER0_IRQHandler(void)
{
  LETIMER_IntClear(TIMER_100MS, LETIMER_IF_UF);
  sl_iec60730_test_clock_tick();
}

void integration_test_run_init(void)
{
  integration_test_init();
  integration_test_timers_init();
  integration_test_timers_enable();
  // Reset clock tick counter
  sl_iec60730_sys_clock_count_reset();
  sl_iec60730_post();
}

void integration_test_run_process_action(void)
{
  sl_iec60730_program_counter_check = IEC60730_ALL_COMPLETE_BITS;
  sl_iec60730_bist();
}
