/***************************************************************************/ /**
 * @file oem_iec60730_comm.c
 * @brief Oem function test uart through com
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

#include <string.h>
#include "uartdrv.h"
#include "oem_iec60730.h"
#include HAL_CONFIG_HEADER

#if defined(_USART_ROUTELOC0_MASK)
#define USART0_INIT                                                    \
  {                                                                    \
    USART0,                 /* USART port                   */         \
    HAL_USART0_BAUD_RATE,   /* Baud rate                    */         \
    BSP_USART0_TX_LOC,      /* USART Tx pin location number */         \
    BSP_USART0_RX_LOC,      /* USART Rx pin location number */         \
    (USART_Stopbits_TypeDef)                                           \
    USART_FRAME_STOPBITS_ONE,       /* Stop bits                    */ \
    (USART_Parity_TypeDef)                                             \
    USART_FRAME_PARITY_NONE,       /* Parity                       */  \
    (USART_OVS_TypeDef)                                                \
    USART_CTRL_OVS_X16,       /* Oversampling mode            */       \
    false,                    /* Majority vote disable        */       \
    (UARTDRV_FlowControlType_t)                                        \
    HAL_USART0_FLOW_CONTROL,       /* Flow control                 */  \
    BSP_USART0_CTS_PORT,           /* CTS port number              */  \
    BSP_USART0_CTS_PIN,            /* CTS pin number               */  \
    BSP_USART0_RTS_PORT,           /* RTS port number              */  \
    BSP_USART0_RTS_PIN,            /* RTS pin number               */  \
    NULL,                          /* RX operation queue           */  \
    NULL,                          /* TX operation queue           */  \
    BSP_USART0_CTS_LOC,            /* CTS pin location             */  \
    BSP_USART0_RTS_LOC             /* RTS pin location             */  \
  }
#elif defined(_GPIO_USART_ROUTEEN_MASK) // #define USART0_INIT
#define USART0_INIT                                                    \
  {                                                                    \
    USART0,                 /* USART port                   */         \
    HAL_USART0_BAUD_RATE,   /* Baud rate                    */         \
    BSP_USART0_TX_PORT,     /* TX port number               */         \
    BSP_USART0_RX_PORT,     /* RX port number               */         \
    BSP_USART0_TX_PIN,      /* TX pin number                */         \
    BSP_USART0_RX_PIN,      /* RX pin number                */         \
    0,                      /* UART instance number         */         \
    (USART_Stopbits_TypeDef)                                           \
    USART_FRAME_STOPBITS_ONE,       /* Stop bits                    */ \
    (USART_Parity_TypeDef)                                             \
    USART_FRAME_PARITY_NONE,       /* Parity                       */  \
    (USART_OVS_TypeDef)                                                \
    USART_CTRL_OVS_X16,       /* Oversampling mode            */       \
    false,                    /* Majority vote disable        */       \
    (UARTDRV_FlowControlType_t)                                        \
    HAL_USART0_FLOW_CONTROL,       /* Flow control                 */  \
    BSP_USART0_CTS_PORT,           /* CTS port number              */  \
    BSP_USART0_CTS_PIN,            /* CTS pin number               */  \
    BSP_USART0_RTS_PORT,           /* RTS port number              */  \
    BSP_USART0_RTS_PIN,            /* RTS pin number               */  \
    NULL,                          /* RX operation queue           */  \
    NULL,                          /* TX operation queue           */  \
  }
#else
#define USART0_INIT                                                    \
  {                                                                    \
    USART0,                 /* USART port                   */         \
    HAL_USART0_BAUD_RATE,   /* Baud rate                    */         \
    BSP_USART0_ROUTE_LOC,   /* USART pins location number   */         \
    (USART_Stopbits_TypeDef)                                           \
    USART_FRAME_STOPBITS_ONE,       /* Stop bits                    */ \
    (USART_Parity_TypeDef)                                             \
    USART_FRAME_PARITY_NONE,       /* Parity                       */  \
    (USART_OVS_TypeDef)                                                \
    USART_CTRL_OVS_X16,       /* Oversampling mode            */       \
    (UARTDRV_FlowControlType_t)                                        \
    HAL_USART0_FLOW_CONTROL,       /* Flow control                 */  \
    BSP_USART0_CTS_PORT,           /* CTS port number              */  \
    BSP_USART0_CTS_PIN,            /* CTS pin number               */  \
    BSP_USART0_RTS_PORT,           /* RTS port number              */  \
    BSP_USART0_RTS_PIN,            /* RTS pin number               */  \
    NULL,                          /* RX operation queue           */  \
    NULL                           /* TX operation queue           */  \
  }
#endif //defined( _USART_ROUTELOC0_MASK )

//#define TOOL_LOOPBACK

static oem_message_buffer_t oem_tx_buffer; /*!< Transmit Buffer */
static oem_message_buffer_t oem_rx_buffer; /*!< Receive Buffer */
static uint8_t oem_message_byte = 0;          /*!< Byte to send */
static UARTDRV_HandleData_t oem_uart_handle;
static UARTDRV_Handle_t oem_comm_handle = &oem_uart_handle;
// Use 2 variables to sync TX-RX
static bool oem_ready_to_send_tx_message  = true;
static bool oem_ready_to_check_rx_message = false;
typedef uint8_t oem_usart_error_code_t;
enum {
  E_UART_NOERROR            = 0,
  E_UART_ERROR_ADDRESS      = 1,
  E_UART_ERROR_FUNC_CRC     = 2,
  E_UART_ERROR_RECEIVED_CRC = 3,
  E_UART_ERROR_PAYLOAD      = 4,
  E_UART_ERROR_HARDWARE     = 5,
};

oem_usart_error_code_t oem_error_code = E_UART_NOERROR;
sl_iec60730_crc_t oem_calculated_crc  = (sl_iec60730_crc_t) SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE;

DEFINE_BUF_QUEUE(1, oem_comm_buffer_queue_rx_usart0);
DEFINE_BUF_QUEUE(1, oem_comm_buffer_queue_tx_usart0);

static sl_iec60730_test_failure_t oem_iec60730_comm_check_status_message(void);

static void oem_rx_callback(UARTDRV_Handle_t handle,
                            Ecode_t transfer_status,
                            uint8_t *data,
                            UARTDRV_Count_t transfer_count)
{
  (void) data;
  (void) handle;
  (void) transfer_count;

  if (transfer_status == ECODE_EMDRV_UARTDRV_ABORTED) {
    oem_error_code            = E_UART_ERROR_HARDWARE;
    sl_iec60730_safety_check_error_occur(SL_IEC60730_USART0_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_USART0_FAIL);
    return;
  }

  if (transfer_status == ECODE_EMDRV_UARTDRV_OK) {
    LABEL_DEF(IEC60730_COMM_CHECK_STATUS_BKPT);
    oem_ready_to_check_rx_message = true;
    LABEL_DEF(IEC60730_COMM_BIST_FLAG_BKPT);
    sl_iec60730_program_counter_check |= IEC60730_COMMS_COMPLETE;
  }
}

static void oem_tx_callback(UARTDRV_Handle_t handle,
                            Ecode_t transfer_status,
                            uint8_t *data,
                            UARTDRV_Count_t transfer_count)
{
  (void) data;
  (void) handle;
  (void) transfer_count;

  if (transfer_status == ECODE_EMDRV_UARTDRV_ABORTED) {
    oem_error_code            = E_UART_ERROR_HARDWARE;
    sl_iec60730_safety_check_error_occur(SL_IEC60730_USART0_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_USART0_FAIL);
  }
}

void oem_comm_init(void)
{
  UARTDRV_InitUart_t init_com = USART0_INIT;

  init_com.rxQueue = (UARTDRV_Buffer_FifoQueue_t *) &oem_comm_buffer_queue_rx_usart0;
  init_com.txQueue = (UARTDRV_Buffer_FifoQueue_t *) &oem_comm_buffer_queue_tx_usart0;

  UARTDRV_InitUart(oem_comm_handle, &init_com);
  GPIO_PinModeSet(oem_comm_handle->rxPort, oem_comm_handle->rxPin, gpioModeInputPull, 1);
#ifdef HAL_VCOM_ENABLE
  GPIO_PinModeSet(BSP_VCOM_ENABLE_PORT,
                  BSP_VCOM_ENABLE_PIN,
                  gpioModePushPull,
                  1);
#endif /* HAL_VCOM_ENABLE */
  UARTDRV_Receive(oem_comm_handle,
                  (uint8_t *) (&oem_rx_buffer),
                  sizeof(oem_message_buffer_t),
                  oem_rx_callback);
}

void oem_comm_deinit(void)
{
  UARTDRV_DeInit(oem_comm_handle);
}

void oem_comm_transmit_status_message(void)
{
  if (oem_ready_to_send_tx_message) {
#ifdef TOOL_LOOPBACK
    oem_ready_to_send_tx_message = false;
#endif
    oem_comm_send_message(oem_message_byte);
    sl_iec60730_program_counter_check |= IEC60730_COMMS_COMPLETE;
  }
}

void oem_comm_send_message(uint8_t oem_message_byte)
{
  LABEL_DEF(IEC60730_TRANSMIT_DISABLE_BKPT);
  // Packet begins with transmitting device address
  oem_tx_buffer.device_address = IEC60730_COMM_DEVICE_ADDRESS;

  // Payload in this example is one byte
  oem_tx_buffer.message = oem_message_byte;

  sl_iec60730_crc_t crc_value           = (sl_iec60730_crc_t) SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE;
  sl_iec60730_update_crc_params_t update = SL_IEC60730_IMC_CRC_BUFFER_UPDATE_DEFAULT;

  // Calculate CRC using CRC generating function included in library
  if (SL_IEC60730_TEST_FAILED
      == sl_iec60730_update_crc_with_data_buffer(&update,
                                                 &crc_value,
                                                 (uint8_t *) (&oem_tx_buffer),
                                                 sizeof(oem_message_buffer_t)
                                                 - sizeof(sl_iec60730_crc_t))) {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_OEM_FAIL_1);
    sl_iec60730_safe_state(SL_IEC60730_OEM_FAIL_1);
  }

  oem_tx_buffer.crc = crc_value;

  UARTDRV_Transmit(oem_comm_handle,
                   (uint8_t *) (&oem_tx_buffer),
                   sizeof(oem_message_buffer_t),
                   oem_tx_callback);
}

void oem_comm_init_receive_status_message(void)
{
  if (oem_ready_to_check_rx_message) {
    oem_ready_to_check_rx_message = false;
    //DMADRV_StopTransfer(oem_comm_handle->rxDmaCh);
    if (oem_iec60730_comm_check_status_message() == SL_IEC60730_USART0_FAIL) {
      sl_iec60730_safety_check_error_occur(SL_IEC60730_USART0_FAIL);
      sl_iec60730_safe_state(SL_IEC60730_USART0_FAIL);
    }
    UARTDRV_Receive(oem_comm_handle,
                    (uint8_t *) (&oem_rx_buffer),
                    sizeof(oem_message_buffer_t),
                    oem_rx_callback);
    oem_ready_to_send_tx_message = true;
  }
}

sl_iec60730_test_failure_t oem_iec60730_comm_check_status_message(void)
{
  // If received and expected address do not match, enter safe state
  if (oem_rx_buffer.device_address != IEC60730_COMM_DEVICE_ADDRESS) {
    oem_error_code = E_UART_ERROR_ADDRESS;
    return SL_IEC60730_USART0_FAIL;
  }

  oem_calculated_crc            = (sl_iec60730_crc_t) SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE;
  sl_iec60730_update_crc_params_t update = SL_IEC60730_IMC_CRC_BUFFER_UPDATE_DEFAULT;

  // Calculate CRC using CRC generating function included in library
  if (SL_IEC60730_TEST_FAILED
      == sl_iec60730_update_crc_with_data_buffer(&update,
                                                 &oem_calculated_crc,
                                                 (uint8_t *) (&oem_rx_buffer),
                                                 sizeof(oem_message_buffer_t)
                                                 - sizeof(sl_iec60730_crc_t))) {
    oem_error_code = E_UART_ERROR_FUNC_CRC;
    return SL_IEC60730_USART0_FAIL;
  }

#ifdef UART_CRC_CHECK
  sl_iec60730_crc_t received_crc = oem_rx_buffer.crc;

  // If CRCs don't match, enter safe state
  if (oem_calculated_crc != received_crc) {
    oem_error_code = E_UART_ERROR_RECEIVED_CRC;
    return SL_IEC60730_USART0_FAIL;
  }
#endif // UART_CRC_CHECK

#ifdef UART_PAYLOAD_CHECK
  // Also check payload to make sure that 'status' message is what is expected
  if (oem_rx_buffer.message != oem_message_byte) {
    oem_error_code = E_UART_ERROR_PAYLOAD;
    return SL_IEC60730_USART0_FAIL;
  }
#endif // UART_PAYLOAD_CHECK

  // For the purposes of this example, the status message is an incrementing value
  oem_message_byte = oem_message_byte + 1;

  return SL_IEC60730_NO_FAILURE;
}

void oem_comm_log(const uint8_t *str)
{
  if (str != NULL) {
    UARTDRV_Transmit(oem_comm_handle,
                     (uint8_t *) str,
                     strlen((char *) str),
                     oem_tx_callback);
  }
}
