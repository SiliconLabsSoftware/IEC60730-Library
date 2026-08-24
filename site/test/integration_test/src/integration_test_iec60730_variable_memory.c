/***************************************************************************/ /**
 * @file integration_test_iec60730_variable_memory.c
 * @brief setup integration test variable memory
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

#include "integration_test_common.h"
#include "sl_iec60730_internal.h"

#define INTEGRATION_TEST_NUM_RAM_REGIONS_CHECK     3

#if defined(__GNUC__)
#define INTEGRATION_TEST_RAM_OFFSET                20
#elif defined(__ICCARM__)
#define INTEGRATION_TEST_RAM_OFFSET                80
#else
#endif

__no_init sl_iec60730_vmc_test_multiple_regions_t integration_test_vmc_test;
//uint32_t ram_test_end;
uint32_t num_ram_regions_check;

#if defined(__GNUC__)

sl_iec60730_vmc_test_region_t integration_test_vmc_region_test[INTEGRATION_TEST_NUM_RAM_REGIONS_CHECK] =
{ { .start = RAMTEST_START, .end = RAMTEST_END },
  { .start = RAM_START, .end = RAM_START + INTEGRATION_TEST_RAM_OFFSET },
  { .start = RAM_START + 2 * INTEGRATION_TEST_RAM_OFFSET, .end = RAM_START + 3 * INTEGRATION_TEST_RAM_OFFSET } };

#elif defined(__ICCARM__)

sl_iec60730_vmc_test_region_t integration_test_vmc_region_test[INTEGRATION_TEST_NUM_RAM_REGIONS_CHECK] =
{ { .start = RAMTEST_START, .end = RAMTEST_END },
  { .start = RAM_START, .end = (uint32_t *)((uint32_t)RAM_START + INTEGRATION_TEST_RAM_OFFSET) },
  { .start = (uint32_t *)((uint32_t)RAM_START + 2 * INTEGRATION_TEST_RAM_OFFSET), .end = (uint32_t *)((uint32_t)RAM_START + 3 * INTEGRATION_TEST_RAM_OFFSET) } };

#else
#endif

void integration_test_run_init(void)
{
  integration_test_init();

  integration_test_vmc_test.region = integration_test_vmc_region_test;
  integration_test_vmc_test.number_of_test_regions = INTEGRATION_TEST_NUM_RAM_REGIONS_CHECK;
  //ram_test_end = (uint32_t)RAMTEST_END;
  num_ram_regions_check = INTEGRATION_TEST_NUM_RAM_REGIONS_CHECK;

  // Init regions vmc test
  sl_iec60730_vmc_init(&integration_test_vmc_test);
  sl_iec60730_post();
}

void integration_test_run_process_action(void)
{
  sl_iec60730_bist();
}
