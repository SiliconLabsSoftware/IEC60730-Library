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

void app_init(void)
{
#if (_SILICON_LABS_32B_SERIES == 2)
#if ((defined SL_IEC60730_NON_SECURE_ENABLE) && (!defined(SL_TRUSTZONE_SECURE)))
  printf("Test non-secure peripherals\n");
#else
  printf("Test secure peripherals\n");
#endif
#else // Series 1 devices
  printf("Test secure peripherals\n");
#endif // (_SILICON_LABS_32B_SERIES == 2)

  integration_test_run_init();
}

void app_process_action(void)
{
  integration_test_run_process_action();
  printf("Pass-Test\n");
}
