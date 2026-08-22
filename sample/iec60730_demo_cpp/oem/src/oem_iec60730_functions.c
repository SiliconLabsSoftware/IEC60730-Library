/***************************************************************************/ /**
 * @file oem_iec60730_functions.c
 * @brief Oem function sl_iec60730_safe_state
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

#include <stdio.h>
#include "sl_udelay.h"
#include "oem_iec60730.h"

uint32_t oem_current_failure = SL_IEC60730_NO_FAILURE;
uint8_t oem_err_log[30];

void oem_print_error(void)
{
  uint8_t number_of_failure;
  sl_iec60730_safety_check_t* iec60730_safety_check_result;

  iec60730_safety_check_result = sl_iec60730_safety_check_get_error();
  oem_current_failure = iec60730_safety_check_result->error;
  number_of_failure = iec60730_safety_check_result->number_error;
  sprintf((char *) oem_err_log, "Fail-Test. Number error: %d\n", number_of_failure);
  oem_comm_log(oem_err_log);
  sl_udelay_wait(5000);
  sprintf((char *) oem_err_log, "Fail-Test. Error status: %ld\n", oem_current_failure);
  oem_comm_log(oem_err_log);
  sl_udelay_wait(5000);
}

void sl_iec60730_safe_state(sl_iec60730_test_failure_t failure)
{
  (void) failure;
  oem_timers_disable();
  oem_print_error();
  SL_IEC60730_RSTCAUSES_CLEAR();
  while (1) {
    sl_iec60730_restart_watchdogs();
  }
}
