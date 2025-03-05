/***************************************************************************/ /**
 * @file    sl_iec60730_program_counter.c
 * @brief Program Counter check
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
 * @addtogroup IEC60730_PROGRAM_COUNTER
 * @{
 *****************************************************************************/

uint16_t sl_iec60730_program_counter_check = 0;

void sl_iec60730_program_counter_test(void)
{
  LABEL_DEF(IEC60730_PC_BKPT);
  // All bits for enabled tests should be set, if not enter safe state
  if (sl_iec60730_program_counter_check != IEC60730_ALL_COMPLETE_BITS) {
    // Disable interrupts since we need to enter SafeState as quickly as possible
    __disable_irq();

    sl_iec60730_safety_check_error_occur(SL_IEC60730_PROGRAM_COUNTER_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_PROGRAM_COUNTER_FAIL);
  }
  // Reset flags to make sure that bist is continuously executing
  sl_iec60730_program_counter_check = 0;
}

/** @} (end addtogroup IEC60730_PROGRAM_COUNTER) */
/** @} (end addtogroup efr32_iec60730) */
