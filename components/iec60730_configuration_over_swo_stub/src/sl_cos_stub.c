/***************************************************************************//**
 * @file
 * @brief Stub Configuration Over SWO — no-op (avoids debug_swo / TPIU init).
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: LicenseRef-MSLA
 *
 ******************************************************************************/
#include "sl_cos.h"

void sl_cos_send_config(void)
{
}

void sl_cos_config_vcom(uint32_t baudrate, uint8_t flow_control)
{
  (void)baudrate;
  (void)flow_control;
}

void sl_cos_config_pti(uint32_t baudrate,
                       COS_PtiMode_t mode,
                       COS_PtiInterface_t interface)
{
  (void)baudrate;
  (void)mode;
  (void)interface;
}
