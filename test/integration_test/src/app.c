/***************************************************************************//**
 * @file
 * @brief Top level application functions
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

#include "app.h"
#include "integration_test_common.h"
#include "sl_iec60730_internal.h"

/* Do not printf over VCOM here: with HW flow control / VCOM disabled /
 * configuration_over_swo stub, USART_Tx can block forever on TXBL and
 * J-Link integration tests (e.g. cpu_registers) never reach POST breakpoints.
 */

void app_init(void)
{
  integration_test_run_init();
}

void app_process_action(void)
{
  integration_test_run_process_action();
}
