/***************************************************************************/ /**
 * @file integration_test_iec60730_irq.c
 * @brief setup integration test irq
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

/* Setup variable  */
#define IEC60730_IRQ_SIZE               8
#define INTEGRATION_TEST_NUM_IRQ_CHECK  1

volatile SL_IEC60730_IRQ_TYPE_VARIABLE integration_test_irq_exec_count[IEC60730_IRQ_SIZE] = { 0 };
const sl_iec60730_irq_execution_bounds_t integration_test_irq_freq_bounds[IEC60730_IRQ_SIZE] = { { .min = 9, .max = 11 },
                                                                                                 { .min = 9, .max = 11 },
                                                                                                 { .min = 9, .max = 11 },
                                                                                                 { .min = 9, .max = 11 },
                                                                                                 { .min = 9, .max = 11 },
                                                                                                 { .min = 9, .max = 11 },
                                                                                                 { .min = 9, .max = 11 },
                                                                                                 { .min = 9, .max = 11 } };

sl_iec60730_irq_cfg_t integration_test_irq_config = { &integration_test_irq_exec_count[0],
                                                      &integration_test_irq_freq_bounds[0],
                                                      INTEGRATION_TEST_NUM_IRQ_CHECK };

/* count which irq is executed  */
void integration_test_irq_tick(void)
{
  for (uint8_t i = 0; i < INTEGRATION_TEST_NUM_IRQ_CHECK; i++) {
    integration_test_irq_exec_count[i]++;
  }
}

/* 10ms timer interrupt handler */
void TIMER0_IRQHandler(void)
{
  TIMER_IntClear(TIMER_10MS, TIMER_IF_OF);
  // Increase IRQ counter
  integration_test_irq_tick();
}

/* 100ms timer interrupt handler */
void LETIMER0_IRQHandler(void)
{
  LETIMER_IntClear(TIMER_100MS, LETIMER_IF_UF);
  sl_iec60730_irq_check();
}

void integration_test_run_init(void)
{
  integration_test_init();
  integration_test_timers_init();
  integration_test_timers_enable();
  sl_iec60730_irq_init(&integration_test_irq_config);
  sl_iec60730_post();
}

void integration_test_run_process_action(void)
{
  sl_iec60730_bist();
  sl_iec60730_program_counter_check = IEC60730_ALL_COMPLETE_BITS;
  sl_iec60730_program_counter_test();
}
