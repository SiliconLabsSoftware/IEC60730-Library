/***************************************************************************//**
 * @file
 * @brief Stub Configuration Over SWO (no CoreSight / debug_swo).
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: LicenseRef-MSLA
 *
 ******************************************************************************/
#ifndef SL_COS_H
#define SL_COS_H

#include "sl_enum.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Match Silabs configuration_over_swo — used by iostream USART VCOM init. */
#define COS_CONFIG_FLOWCONTROL_NONE      0
#define COS_CONFIG_FLOWCONTROL_CTS       1
#define COS_CONFIG_FLOWCONTROL_RTS       2
#define COS_CONFIG_FLOWCONTROL_CTS_RTS   3

SL_ENUM(COS_PtiMode_t) {
  COS_CONFIG_PTI_MODE_ONEWIRE,
  COS_CONFIG_PTI_MODE_TWOWIRE,
  COS_CONFIG_PTI_MODE_EFRUART,
  COS_CONFIG_PTI_MODE_EFRSPI,
  COS_CONFIG_PTI_MODE_COUNT
};

SL_ENUM(COS_PtiInterface_t) {
  COS_CONFIG_PTI_INTERFACE_0,
  COS_CONFIG_PTI_INTERFACE_1,
  COS_CONFIG_PTI_INTERFACE_COUNT
};

void sl_cos_send_config(void);
void sl_cos_config_vcom(uint32_t baudrate, uint8_t flow_control);
void sl_cos_config_pti(uint32_t baudrate,
                       COS_PtiMode_t mode,
                       COS_PtiInterface_t interface);

#ifdef __cplusplus
}
#endif

#endif /* SL_COS_H */
