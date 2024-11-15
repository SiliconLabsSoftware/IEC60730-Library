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

#include "sl_iec60730_internal.h"

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup IEC60730_SYSTEM_CLOCK_Test
 * @{
 *****************************************************************************/

static uint8_t sl_iec60730_sys_clock_count = 0;
static uint8_t sl_iec60730_timer_test_control = SL_IEC60730_TIMER_TEST_DISABLE;
static uint16_t sl_iec60730_number_test_timer_tick = 0;

uint16_t sl_iec60730_get_number_test_timer_tick(void)
{
  return sl_iec60730_number_test_timer_tick;
}

void sl_iec60730_sys_clock_test_enable(void)
{
  sl_iec60730_timer_test_control = SL_IEC60730_TIMER_TEST_ENABLE;
}

void sl_iec60730_sys_clock_test_disable(void)
{
  sl_iec60730_timer_test_control = SL_IEC60730_TIMER_TEST_DISABLE;
}

void sl_iec60730_sys_clock_count_tick(void)
{
  LABEL_DEF(IEC60730_SYSTEM_CLOCK_TICK_BKPT);
  // Increment counter, to be compared with SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ
  sl_iec60730_sys_clock_count++;
}

void sl_iec60730_sys_clock_count_reset(void)
{
  sl_iec60730_sys_clock_count = 0;
}

void sl_iec60730_test_clock_tick(void)
{
  static uint8_t test_clock_tick_counter = 0;
  LABEL_DEF(IEC60730_TEST_CLOCK_TICK_BKPT);
  // Only run test if OEM has enabled ISR-based IEC tests
  if (sl_iec60730_timer_test_control == SL_IEC60730_TIMER_TEST_ENABLE) {
    // System clock ticks must equal OEM-configured ratio plus or minus tolerance
    if ((sl_iec60730_sys_clock_count > (SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ
                                        + SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE))
        || (sl_iec60730_sys_clock_count < (SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ
                                           - SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE))) {
      sl_iec60730_safety_check_error_occur(SL_IEC60730_CLOCK_FAIL);
      sl_iec60730_safe_state(SL_IEC60730_CLOCK_FAIL);
    }
    sl_iec60730_irq_check();
    // Increment multiplier, which runs bist frequency test at terminal value
    test_clock_tick_counter++;
    if (test_clock_tick_counter > SL_IEC60730_TEST_CLOCK_MULTIPLIER) {
      sl_iec60730_program_counter_test();
      test_clock_tick_counter = 0;
    }
  } else {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_CLOCK_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_CLOCK_FAIL);
  }
  sl_iec60730_sys_clock_count_reset();
  sl_iec60730_program_counter_check |= IEC60730_CPU_CLOCKS_COMPLETE;
  // count the number of times the function sl_iec60730_test_clock_tick is called
  sl_iec60730_number_test_timer_tick++;
}

/** @} (end addtogroup IEC60730_SYSTEM_CLOCK_Test) */
/** @} (end addtogroup efr32_iec60730) */
