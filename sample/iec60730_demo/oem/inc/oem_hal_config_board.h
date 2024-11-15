/**************************************************************************/ /**
 * Copyright (c) 2021 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef OEM_HAL_CONFIG_BOARD_H
#define OEM_HAL_CONFIG_BOARD_H

#include "sl_component_catalog.h"
#include "sl_board_control_config.h"

#ifdef SL_CATALOG_IOSTREAM_UART_COMMON_PRESENT
#include "em_usart.h"
#include "sl_iostream_usart_vcom_config.h"

#define BSP_USART0_TX_PIN       SL_IOSTREAM_USART_VCOM_TX_PIN
#define BSP_USART0_TX_PORT      SL_IOSTREAM_USART_VCOM_TX_PORT
#define BSP_USART0_TX_LOC       (0U)

#define BSP_USART0_RX_PIN       SL_IOSTREAM_USART_VCOM_RX_PIN
#define BSP_USART0_RX_PORT      SL_IOSTREAM_USART_VCOM_RX_PORT
#define BSP_USART0_RX_LOC       (0U)

#if (SL_IOSTREAM_USART_VCOM_FLOW_CONTROL_TYPE == usartHwFlowControlCtsAndRts)

#ifdef SL_IOSTREAM_USART_VCOM_CTS_PIN
#define BSP_USART0_CTS_PIN      SL_IOSTREAM_USART_VCOM_CTS_PIN
#define BSP_USART0_CTS_PORT     SL_IOSTREAM_USART_VCOM_CTS_PORT
#define BSP_USART0_CTS_LOC      (0U)
#else
#define BSP_USART0_CTS_PIN      (0U)
#define BSP_USART0_CTS_PORT     (0U)
#define BSP_USART0_CTS_LOC      (0U)
#endif //SL_IOSTREAM_USART_VCOM_CTS_PIN

#ifdef SL_IOSTREAM_USART_VCOM_RTS_PIN
#define BSP_USART0_RTS_PIN      SL_IOSTREAM_USART_VCOM_RTS_PIN
#define BSP_USART0_RTS_PORT     SL_IOSTREAM_USART_VCOM_RTS_PORT
#define BSP_USART0_RTS_LOC      (0U)
#else
#define BSP_USART0_RTS_PIN      (0U)
#define BSP_USART0_RTS_PORT     (0U)
#define BSP_USART0_RTS_LOC      (0U)
#endif //SL_IOSTREAM_USART_VCOM_RTS_PIN

#define HAL_USART0_FLOW_CONTROL SL_IOSTREAM_USART_VCOM_FLOW_CONTROL_TYPE

#endif //(SL_IOSTREAM_USART_VCOM_FLOW_CONTROL_TYPE==usartHwFlowControlCtsAndRts)

#define HAL_USART0_BAUD_RATE    SL_IOSTREAM_USART_VCOM_BAUDRATE

#define HAL_VCOM_ENABLE         SL_BOARD_ENABLE_VCOM

#ifdef SL_BOARD_ENABLE_VCOM_PIN
#define BSP_VCOM_ENABLE_PIN     SL_BOARD_ENABLE_VCOM_PIN
#else
#define BSP_VCOM_ENABLE_PIN     (0U)
#endif //SL_BOARD_ENABLE_VCOM_PIN

#ifdef SL_BOARD_ENABLE_VCOM_PORT
#define BSP_VCOM_ENABLE_PORT    SL_BOARD_ENABLE_VCOM_PORT
#else
#define BSP_VCOM_ENABLE_PORT    (0U)
#endif //SL_BOARD_ENABLE_VCOM_PORT

#endif // SL_CATALOG_IOSTREAM_UART_COMMON_PRESENT

#endif // OEM_HAL_CONFIG_BOARD_H
