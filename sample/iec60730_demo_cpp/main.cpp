/***************************************************************************//**
 * @file main.c
 * @brief main() function.
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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

// app_init() and app_process_action() are defined in the C++ translation unit
// app.cpp; app.h declares them with C linkage so this call site and the
// definitions agree on the symbol names.
#include "app.h"

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif

int main(void)
{
  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  // Note that if the kernel is present, the start task will be started and software
  // component initialization will take place there.
#if defined(IEC60730_DEMO_USE_SL_MAIN)
  sl_main_init();
#else
  sl_system_init();
#endif

#if defined(SL_CATALOG_KERNEL_PRESENT)
#if defined(IEC60730_DEMO_USE_SL_MAIN)
  // Start the kernel. The start task will be executed (Highest priority) to complete
  // the Simplicity SDK components initialization and the user app_init() hook function will be called.
  sl_main_kernel_start();
#else
  // User provided code, then start the kernel. Task(s) created in app_init()
  // will start running.
  app_init();
  sl_system_kernel_start();
#endif
#else // SL_CATALOG_KERNEL_PRESENT

  // User provided code.
  app_init();

  while (1) {
    // Silicon Labs components process action routine
    // must be called from the super loop.
#if defined(IEC60730_DEMO_USE_SL_MAIN)
    sl_main_process_action();
#else
    sl_system_process_action();
#endif

    // User provided code. Application process.
    app_process_action();

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
    // Let the CPU go to sleep if the system allows it.
    sl_power_manager_sleep();
#endif
  }
#endif // SL_CATALOG_KERNEL_PRESENT
}
