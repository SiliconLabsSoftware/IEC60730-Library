/***************************************************************************/ /**
 * @file oem_iec60730.c
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

#ifndef OEM_IEC60730_H
#define OEM_IEC60730_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// OEM specific header file
#include "em_core.h"
#include "em_wdog.h"
#include "em_gpcrc.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "em_letimer.h"
#include "oem_iec60730_init_device.h"
#include "oem_iec60730_comm.h"
#include "oem_iec60730_timer.h"
#include "sl_iec60730_internal.h"
#include SL_IEC60730_BOARD_HEADER

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup IEC60730_OEM
 * @{
 * @section communications_hardware_architecture Hardware Architecture
 *
 * \link
 *
 * \image
 *****************************************************************************/
/** @} (end addtogroup IEC60730_OEM) */
/** @} (end addtogroup efr32_iec60730) */

// Tick 10 ms
#define SYSTICK_10MS   ((uint32_t) 10UL)
#define SYSTICK_100MS  ((uint32_t) 100UL)
#define SYSTICK_1000MS ((uint32_t) 1000UL)

// Exit ATOMIC section of IMC Post
#define SL_IEC60730_IMC_BIST_EXIT_ATOMIC() CORE_EXIT_ATOMIC()

// Declare Bist
#ifndef DECLARE_BENMARK_BIST
#define DECLARE_BENMARK_BIST()
#endif

// Start Benmark Bist
#ifndef STARTEXEC_BENMARK_BIST
#define STARTEXEC_BENMARK_BIST()
#endif

// Terminate Benmark Bist
#ifndef TERMINATE_BENMARK_BIST
#define TERMINATE_BENMARK_BIST()
#endif

// Bist execution status
extern volatile bool oem_bist_check_flag;
// USART run flag
extern volatile bool oem_usart_flag;

/**************************************************************************/ /**
 * @brief Configuration
 *
 * return void
 *
 * Config parameters before running #sl_iec60730_post
 *****************************************************************************/
extern void oem_iec60730_config(void);

/**************************************************************************/ /**
 * @brief Initialize
 *
 * return void
 *
 * Initialize parameters before running #sl_iec60730_bist
 *****************************************************************************/
extern void oem_iec60730_init(void);

/**************************************************************************/ /**
 * @brief Increase IRQ counter
 *
 * return void
 *
 * Increase #oem_irq_exec_count
 *****************************************************************************/
extern void oem_irq_exec_count_tick(void);

/**************************************************************************/ /**
 * @brief iec60730 App Initialize
 *
 * return void
 *
 * App init oem iec60730
 *****************************************************************************/
void app_oem_init(void);

/**************************************************************************/ /**
 * @brief iec60730 App Process Action
 *
 * return void
 *
 * App process action oem iec60730
 *****************************************************************************/
void app_oem_process_action(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
