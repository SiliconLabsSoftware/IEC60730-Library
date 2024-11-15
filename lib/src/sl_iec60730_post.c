/***************************************************************************/ /**
 * @file
 * @brief Power On Self Test
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
 * @addtogroup sl_iec60730_post
 * @{
 *****************************************************************************/

const sl_iec60730_library_version_t SL_IEC60730_LIBRARY_VERSION =
  IE60730_LIBRARY_VERSION;

void sl_iec60730_post(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;

  // Watchdog check
  result = sl_iec60730_watchdog_post();
  if (result == SL_IEC60730_TEST_FAILED) {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_WATCHDOG_POST_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_WATCHDOG_POST_FAIL);
  }

  // Invariable memory check
  result = sl_iec60730_imc_post();
  if (result == SL_IEC60730_TEST_FAILED) {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_IMC_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_IMC_FAIL);
  }
  LABEL_DEF(IEC60730_IMC_POST_END_BKPT);

  // Variable memory check
  result = sl_iec60730_vmc_post();
  if (result == SL_IEC60730_TEST_FAILED) {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_VMC_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_VMC_FAIL);
  }

  // CPU registers check
  result = sl_iec60730_cpu_registers_post();
  if (result == SL_IEC60730_TEST_FAILED) {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_CPU_REGISTERS_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_CPU_REGISTERS_FAIL);
  }

  LABEL_DEF(IEC60730_SYSTEM_CLOCK_FREQ_ADJUSTMENT_BKPT);
  sl_iec60730_sys_clock_test_enable();

  sl_iec60730_restart_watchdogs();

  return;
}

/** @} (end addtogroup sl_iec60730_post) */
/** @} (end addtogroup efr32_iec60730) */
