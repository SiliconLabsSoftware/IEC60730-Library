/***************************************************************************/ /**
 * @file integration_test_iec60730_variable_memory.c
 * @brief setup integration test variable memory
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
