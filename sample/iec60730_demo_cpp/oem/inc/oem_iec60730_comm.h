/***************************************************************************/ /**
 * @file ie60730_oem_comm.h
 * @brief
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

#ifndef OEM_IEC60730_COMM_H
#define OEM_IEC60730_COMM_H

#include "sl_iec60730_internal.h"

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup IEC60730_OEM_COMM_Test
 * @{
 * @details These files provide an example communications protocol that meet
 * IEC60730 requirements, which include address redundancy, a CRC check on data payloads,
 * and a periodic transfer of data.  This example uses UART as its protocol.
 *
 * This protocol simulates both the transmitter and receiver in an IEC60730 compliant
 * application.  The UART RX and TX pins must be shorted together in order for the
 * example to successfully receive transmitted data.
 *****************************************************************************/

/**************************************************************************/ /**
 * public Initialize UART
 *
 * @return void
 *
 * This function initialize UART
 *****************************************************************************/
void oem_comm_init(void);

/**************************************************************************/ /**
 * public Deinitialize UART
 *
 * @return void
 *
 * This function deinitialize UART
 *****************************************************************************/
void oem_comm_deinit(void);

/**************************************************************************/ /**
 * public Ready peripheral to read a status message
 *
 * @return void
 *
 * This function calls into the UART peripheral library to receive
 * the status packet.
 *****************************************************************************/
void oem_comm_init_receive_status_message(void);

/**************************************************************************/ /**
 * public Transmit status message
 *
 * @return void
 *
 * This function passes a status message buffer into the UART peripheral library
 * for transmit using @ref oem_comm_send_message.
 *****************************************************************************/
void oem_comm_transmit_status_message(void);

/**************************************************************************/ /**
 * public Check received message
 *
 * @return void
 *
 * This function examines the received buffer to check that the CRC is valid.
 * @returns Returns #SL_IEC60730_TEST_FAILED if message does not provide
 * correct CRC and address, #SL_IEC60730_TEST_PASSED otherwise.
 *****************************************************************************/
sl_iec60730_test_result_t oem_comm_check_received_message(void);

/**************************************************************************/ /**
 * public Transmit message
 *
 * @param oem_message_byte 1-byte data payload to be transmitted
 *
 * @return void
 *
 * This function sends a 1-byte payload with IEC-compliant header
 * and footer overhead.
 *****************************************************************************/
void oem_comm_send_message(uint8_t oem_message_byte);

/**************************************************************************/ /**
 * public Transmit Log
 *
 * @param str Log string
 *
 * @return void
 *
 * This function sends a string of Log.
 *****************************************************************************/
void oem_comm_log(const uint8_t *str);

/// Sets the address used by the transmitter and checked by receiver in the comm protocol.
#define IEC60730_COMM_DEVICE_ADDRESS 0x01

/// Buffer used in comm that includes IEC60730-compliant address and CRC.
typedef struct messageBuffer {
  uint8_t device_address; ///< 8-bit address of transmitting device
  uint8_t message;        ///< Payload of buffer
  sl_iec60730_crc_t crc;              ///< CRC covering all other bytes in packet
} oem_message_buffer_t;

/** @} (end addtogroup IEC60730_OEM_COMM_Test) */
/** @} (end addtogroup efr32_iec60730) */

#endif /* OEM_IEC60730_COMM_H */
