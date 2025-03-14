/***************************************************************************/ /**
 * @file integration_test_iec60730_program_counter.c
 * @brief setup integration test program counter
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

void integration_test_run_init(void)
{
  integration_test_init();
  sl_iec60730_post();
}

void integration_test_run_process_action(void)
{
  sl_iec60730_bist();
  sl_iec60730_program_counter_check = IEC60730_ALL_COMPLETE_BITS;
  sl_iec60730_program_counter_test();
}
