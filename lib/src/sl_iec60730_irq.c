/***************************************************************************/ /**
 * @file
 * @brief Interrupt check
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
 * @addtogroup IEC60730_IRQ_Test
 * @{
 *****************************************************************************/

// static variable save config
static sl_iec60730_irq_cfg_t* iec60730_irq_cfg = NULL;

#if (SL_IEC60730_IRQ_STATUS_ENABLE == 1)
static sl_iec60730_irq_fail_t iec60730_irq_fail_result = { 0, 0 };
#endif //(SL_IEC60730_IRQ_STATUS_ENABLE == 1)

#if (SL_IEC60730_IRQ_STATUS_ENABLE == 1)

sl_iec60730_irq_fail_t* sl_iec60730_get_irq_index_failed(void)
{
  sl_iec60730_irq_fail_t* result = NULL;

  result = &iec60730_irq_fail_result;

  return result;
}

void sl_iec60730_irq_reset_fail_result(void)
{
  iec60730_irq_fail_result.num_irq_fail = 0;
  iec60730_irq_fail_result.irq_fail = 0;
}

/**************************************************************************/ /**
 * local IEC60730 remember the location of the failed IRQ
 *
 * @param position input the index of the failed IRQ
 *
 * @returns None.
 *
 * This function remember the location of the failed IRQ
 *****************************************************************************/
static void sl_iec60730_irq_fail_occur(uint8_t position)
{
  iec60730_irq_fail_result.irq_fail |= (1 << position);
  iec60730_irq_fail_result.num_irq_fail++;
}

#endif // (SL_IEC60730_IRQ_STATUS_ENABLE == 1)

void sl_iec60730_irq_init(sl_iec60730_irq_cfg_t * irq_cfg_ptr)
{
  if (irq_cfg_ptr != NULL) {
    if (irq_cfg_ptr->size < IEC60730_MAX_IRQ_CHECK) {
      iec60730_irq_cfg = irq_cfg_ptr;
    } else {
      sl_iec60730_safety_check_error_occur(SL_IEC60730_INTERRUPT_FAIL);
      sl_iec60730_safe_state(SL_IEC60730_INTERRUPT_FAIL);
    }
  }
}

void sl_iec60730_irq_reset_counter(void)
{
  if (iec60730_irq_cfg != NULL) {
    for (uint8_t index = 0; index < iec60730_irq_cfg->size; index++) {
      iec60730_irq_cfg->irq_count[index] = 0;
    }
  }
}

void sl_iec60730_irq_check(void)
{
  if (iec60730_irq_cfg != NULL) {
    LABEL_DEF(IEC60730_IRQ_BKPT);
    for (uint8_t index = 0; index < iec60730_irq_cfg->size; index++) {
      if ((iec60730_irq_cfg->irq_count[index] > iec60730_irq_cfg->irq_bounds[index].max)
          || (iec60730_irq_cfg->irq_count[index] < iec60730_irq_cfg->irq_bounds[index].min)) {
        #if (SL_IEC60730_IRQ_STATUS_ENABLE == 1)
        sl_iec60730_irq_fail_occur(index);
        #endif // (SL_IEC60730_IRQ_STATUS_ENABLE == 1)
        sl_iec60730_safety_check_error_occur(SL_IEC60730_INTERRUPT_FAIL);
        sl_iec60730_safe_state(SL_IEC60730_INTERRUPT_FAIL);
      }
    }
  } else {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_INTERRUPT_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_INTERRUPT_FAIL);
  }
  sl_iec60730_irq_reset_counter();
  sl_iec60730_program_counter_check |= IEC60730_INTERRUPT_COMPLETE;
}

/** @} (end addtogroup IEC60730_IRQ_Test) */
/** @} (end addtogroup efr32_iec60730) */
