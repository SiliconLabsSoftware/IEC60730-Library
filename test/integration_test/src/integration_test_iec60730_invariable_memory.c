/***************************************************************************/ /**
 * @file integration_test_iec60730_invariable_memory.c
 * @brief setup integration test invariable memory
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

#define INTEGRATION_TEST_NUM_FLASH_REGIONS_CHECK     3

#if defined(__GNUC__)
#define INTEGRATION_TEST_FLASH_OFFSET                 20
#elif defined(__ICCARM__)
#define INTEGRATION_TEST_FLASH_OFFSET                 80
#else
#endif

__no_init sl_iec60730_imc_params_t integration_test_imc;
__no_init sl_iec60730_imc_test_multiple_regions_t integration_test_imc_test __CLASSB_RAM;

uint32_t num_flash_regions_check;

#if defined(__GNUC__)

sl_iec60730_imc_test_region_t integration_test_imc_region_test[INTEGRATION_TEST_NUM_FLASH_REGIONS_CHECK] =
{ { .start = SL_IEC60730_ROM_START, .end = SL_IEC60730_ROM_END },
  { .start = SL_IEC60730_ROM_START + 2 * INTEGRATION_TEST_FLASH_OFFSET, .end = SL_IEC60730_ROM_START + 3 * INTEGRATION_TEST_FLASH_OFFSET },
  { .start = SL_IEC60730_ROM_START + 4 * INTEGRATION_TEST_FLASH_OFFSET, .end = SL_IEC60730_ROM_START + 5 * INTEGRATION_TEST_FLASH_OFFSET } };

#elif defined(__ICCARM__)

sl_iec60730_imc_test_region_t integration_test_imc_region_test[INTEGRATION_TEST_NUM_FLASH_REGIONS_CHECK] =
{ { .start = SL_IEC60730_ROM_START, .end = SL_IEC60730_ROM_END },
  { .start = (uint32_t *)((uint32_t)SL_IEC60730_ROM_START + 2 * INTEGRATION_TEST_FLASH_OFFSET), .end = (uint32_t *)((uint32_t)SL_IEC60730_ROM_START + 3 * INTEGRATION_TEST_FLASH_OFFSET) },
  { .start = (uint32_t *)((uint32_t)SL_IEC60730_ROM_START + 4 * INTEGRATION_TEST_FLASH_OFFSET), .end = (uint32_t *)((uint32_t)SL_IEC60730_ROM_START + 5 * INTEGRATION_TEST_FLASH_OFFSET) } };

#else
#endif

void integration_test_run_init(void)
{
  integration_test_init();

  integration_test_imc.gpcrc = SL_IEC60730_DEFAULT_GPRC;
  integration_test_imc_test.region = integration_test_imc_region_test;
  integration_test_imc_test.number_of_test_regions = 1;
  num_flash_regions_check = 1;

  // Init regions vmc test
  sl_iec60730_imc_init(&integration_test_imc, &integration_test_imc_test);
  sl_iec60730_post();
}

void integration_test_run_process_action(void)
{
  sl_iec60730_bist();
}
