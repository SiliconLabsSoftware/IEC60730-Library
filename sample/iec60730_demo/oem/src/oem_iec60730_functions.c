/***************************************************************************/ /**
 * @file oem_iec60730_functions.c
 * @brief Oem function sl_iec60730_safe_state
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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
