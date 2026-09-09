/***************************************************************************/ /**
 * @file
 * @brief main() function.
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
#include "sl_component_catalog.h"

// Gecko SDK 4.x boots the application through sl_system_*(), while Simplicity
// SDK 2024.6 and later replaced that service with sl_main_*(). Detect which one
// the selected SDK provides so this file builds unmodified on both.
#if defined(SL_CATALOG_SL_MAIN_PRESENT)
#define IEC60730_DEMO_USE_SL_MAIN
#elif defined(__has_include)
#if __has_include("sl_main_init.h")
#define IEC60730_DEMO_USE_SL_MAIN
#endif
#endif

#if defined(IEC60730_DEMO_USE_SL_MAIN)
#include "sl_main_init.h"
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_main_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_main_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT
#else  // IEC60730_DEMO_USE_SL_MAIN
#include "sl_system_init.h"
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT
#endif // IEC60730_DEMO_USE_SL_MAIN

#include "app.h"

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif

int main(void)
{
  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  // Note that if the kernel is present, processing task(s) will be created by
  // this call.
#if defined(IEC60730_DEMO_USE_SL_MAIN)
  sl_main_init();
#else
  sl_system_init();
#endif

#if defined(SL_CATALOG_KERNEL_PRESENT)
#if defined(IEC60730_DEMO_USE_SL_MAIN)
  // Start the kernel. The start task completes the component initialization and
  // calls the app_init() hook before the application tasks run.
  sl_main_kernel_start();
#else
  // Initialize the application, then start the kernel. Task(s) created in
  // app_init() will start running.
  app_init();
  sl_system_kernel_start();
#endif
#else // SL_CATALOG_KERNEL_PRESENT
  // Initialize the application. For example, create periodic timer(s).
  app_init();

  while (1) {
    // Do not remove this call: Silicon Labs components process action routine
    // must be called from the super loop.
#if defined(IEC60730_DEMO_USE_SL_MAIN)
    sl_main_process_action();
#else
    sl_system_process_action();
#endif

    // Application process.
    app_process_action();

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
    // Let the CPU go to sleep if the system allows it.
    sl_power_manager_sleep();
#endif
  }
#endif // SL_CATALOG_KERNEL_PRESENT
}
