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

#ifndef APP_H
#define APP_H

#include "unit_test_common.h"
#include "unit_test_iec60730_post.h"
#include "unit_test_iec60730_bist.h"
#include "unit_test_iec60730_program_counter.h"
#include "unit_test_iec60730_safety_check.h"
#include "unit_test_iec60730_irq.h"
#include "unit_test_iec60730_system_clock.h"
#include "unit_test_iec60730_watchdog.h"
#include "unit_test_iec60730_cpu_registers.h"
#include "unit_test_iec60730_variable_memory.h"
#include "unit_test_iec60730_invariable_memory.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void);

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void);

#endif // APP_H
