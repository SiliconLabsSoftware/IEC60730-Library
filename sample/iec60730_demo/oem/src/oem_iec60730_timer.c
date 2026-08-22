/***************************************************************************/ /**
 * @file oem_iec60730_timer.c
 * @brief Oem timer used for test
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

#include "oem_iec60730.h"

//oem_test_clock_source_t TestClock_Status = CLK_SYS;

const TIMER_Init_TypeDef oem_timer_10ms_config    = TIMER_INIT_TEST_10MS;
const LETIMER_Init_TypeDef oem_timer_100ms_config = TIMER_INIT_TEST_100MS;

// Change the test clock source
//void TestClock_Switch(oem_test_clock_source_t test_clk);

void oem_timers_init(void)
{
  /* Initialize 10ms timer */
  TIMER_Init(TIMER_10MS, &oem_timer_10ms_config);
  /* Initialize 10ms timer */
  LETIMER_Init(TIMER_100MS, &oem_timer_100ms_config);

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

/* 10ms timer interrupt handler */
void TIMER0_IRQHandler(void)
{
  TIMER_IntClear(TIMER_10MS, TIMER_IF_OF);

  // Signal the Bist execution
#ifndef IEC60730_DISABLE_COMM
  oem_bist_check_flag = true;
#endif

  // Increase clock tick counter
  sl_iec60730_sys_clock_count_tick();

  // Increase IRQ counter
  oem_irq_exec_count_tick();
}

/* 100ms timer interrupt handler */
void LETIMER0_IRQHandler(void)
{
  LETIMER_IntClear(TIMER_100MS, LETIMER_IF_UF);

#ifndef IEC60730_DISABLE_COMM
  oem_usart_flag = true;
#endif

  // Execute clock checking
  sl_iec60730_test_clock_tick();
}

void oem_timers_enable(void)
{
  // Start timer
  TIMER_Enable(TIMER_10MS, true);
  LETIMER_Enable(TIMER_100MS, true);
}

void oem_timers_disable(void)
{
  // Start timer
  TIMER_Enable(TIMER_10MS, false);
  LETIMER_Enable(TIMER_100MS, false);
}

void oem_adj_top_val_timer(void)
{
#if defined(TIMER_STATUS_SYNCBUSY)
  TIMER_SyncWait(TIMER_10MS);
#endif

  // Get timer frequency
  uint32_t freq = CMU_ClockFreqGet(cmuClock_TIMER0);

  // Calculate top_value of the timer to generate 10ms
#if (_SILICON_LABS_32B_SERIES < 2)
  uint32_t top_val = (freq * SYSTICK_10MS) / (SYSTICK_1000MS * (1 << oem_timer_10ms_config.prescale));
#else
  uint32_t top_val = (freq * SYSTICK_10MS) / (SYSTICK_1000MS * (oem_timer_10ms_config.prescale + 1));
#endif //(_SILICON_LABS_32B_SERIES < 2)
  TIMER_TopSet(TIMER_10MS, top_val);

  freq = CMU_ClockFreqGet(cmuClock_LETIMER0);

  // Calculate top_value of the timer to generate 100ms
  top_val = (freq * SYSTICK_100MS) / SYSTICK_1000MS;

  LETIMER_TopSet(TIMER_100MS, top_val);
}
