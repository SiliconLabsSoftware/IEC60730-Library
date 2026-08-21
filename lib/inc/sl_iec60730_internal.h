/***************************************************************************/ /**
 * @file
 * @brief Internal definitions
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: LicenseRef-MSLA
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of the Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement
 * By installing, copying or otherwise using this software, you agree to the
 * terms of the MSLA.
 *
 ******************************************************************************/

#ifndef SL_IEC60730_INTERNAL_H
#define SL_IEC60730_INTERNAL_H

// Standard libraries
#include <stdbool.h>
#include <stdint.h>

// Header files needed for publics
#include "sl_iec60730.h"

// Header files needed for privates
#include "sl_iec60730_watchdog.h"
#include "sl_iec60730_invariable_memory.h"
#include "sl_iec60730_variable_memory.h"
#include "sl_iec60730_cpu_registers.h"
#include "sl_iec60730_system_clock.h"
#include "sl_iec60730_program_counter.h"
#include "sl_iec60730_irq.h"

// Disable warnings on unused labels.  Labels are used to set breakpoints for
// automated testing.
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-label"
#endif

#endif /* SL_IEC60730_INTERNAL_H */
