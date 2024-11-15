/***************************************************************************/ /**
 * @file
 * @brief Build In Self Test
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
 * @addtogroup sl_iec60730_bist
 * @{
 *****************************************************************************/

static sl_iec60730_safety_check_t iec60730_safety_check = { SL_IEC60730_NO_FAILURE, 0 };

__WEAK void sl_iec60730_safe_state(sl_iec60730_test_failure_t failure)
{
  UNUSED_VAR(failure);

  CORE_ATOMIC_IRQ_DISABLE();
  SL_IEC60730_RSTCAUSES_CLEAR();
  while (1) {
    sl_iec60730_restart_watchdogs();
  }
}

void sl_iec60730_safety_check_reset_error(void)
{
  iec60730_safety_check.error = SL_IEC60730_NO_FAILURE;
  iec60730_safety_check.number_error = 0;
}

void sl_iec60730_safety_check_error_occur(sl_iec60730_test_failure_t failure)
{
  iec60730_safety_check.error |= (1 << failure);
  iec60730_safety_check.number_error++;
}

sl_iec60730_safety_check_t* sl_iec60730_safety_check_get_error(void)
{
  sl_iec60730_safety_check_t* result = NULL;

  result = &iec60730_safety_check;

  return result;
}

void sl_iec60730_bist(void)
{
  sl_iec60730_test_result_t
    result;   ///< Temporary storage of the result of each test

  result = sl_iec60730_imc_bist();
  if (result == SL_IEC60730_TEST_FAILED) {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_IMC_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_IMC_FAIL);
  }
  LABEL_DEF(IEC60730_IMC_BIST_END_BKPT);

  result = sl_iec60730_vmc_bist();
  if (SL_IEC60730_TEST_FAILED == result) {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_VMC_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_VMC_FAIL);
  }

  // CPU Register Check
  result = sl_iec60730_cpu_registers_bist();
  if (result == SL_IEC60730_TEST_FAILED) {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_CPU_REGISTERS_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_CPU_REGISTERS_FAIL);
  }

  LABEL_DEF(IEC60730_RESTART_WATCHDOG_BKPT);

  if (iec60730_safety_check.number_error == SL_IEC60730_NO_FAILURE) {
    // Pet the watchdog
    sl_iec60730_restart_watchdogs();
  } else {
    // Enter Safe State and secure the system
    sl_iec60730_safe_state(SL_IEC60730_SAFETY_CHECK_FAIL);
  }
  return;
}

/** @} (end addtogroup sl_iec60730_bist) */
/** @} (end addtogroup efr32_iec60730) */
