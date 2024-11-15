/***************************************************************************/ /**
 * @file integration_test_iec60730_watchdog.c
 * @brief setup integration test watchdog
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

uint8_t wdog_restart_enable        = 1;
volatile uint32_t wdog_por_wait    = 0;
#if (_SILICON_LABS_32B_SERIES == 2)
uint32_t buram_addr;
#endif // (_SILICON_LABS_32B_SERIES == 2)
uint32_t rst_addr  = (uint32_t)SL_IEC60730_RST;
uint32_t rstcauses_register;
uint32_t rstcauses_clear;
uint32_t offset_rst_cmd;

// Dump functions
__WEAK sl_iec60730_test_result_t sl_iec60730_vmc_bist()
{
  return SL_IEC60730_TEST_PASSED;
}

__WEAK sl_iec60730_test_result_t sl_iec60730_imc_bist()
{
  return SL_IEC60730_TEST_PASSED;
}

__WEAK sl_iec60730_test_result_t sl_iec60730_cpu_registers_bist(void)
{
  return SL_IEC60730_TEST_PASSED;
}

void integration_test_run_init(void)
{
  // Get address buram, rst and rst_cmd register
#if (_SILICON_LABS_32B_SERIES < 2)
  offset_rst_cmd = offsetof(RMU_TypeDef, CMD);
  rstcauses_clear = _RMU_CMD_RCCLR_MASK;
#else // Series 2 devices
  offset_rst_cmd = offsetof(EMU_TypeDef, CMD);
  rstcauses_clear = _EMU_CMD_RSTCAUSECLR_MASK;
  buram_addr = (uint32_t)BURAM;
#endif
  rstcauses_register = rst_addr + offset_rst_cmd;
  // Program is waiting for debugger take control after power on reset
  // Debugger must set this flag to true
  LABEL_DEF(IEC60730_TEST_WDOG_POR_BKPT);
  while (wdog_por_wait == 0);

  integration_test_init();
  sl_iec60730_test_result_t result = sl_iec60730_watchdog_post();
  if (result == SL_IEC60730_TEST_FAILED) {
    sl_iec60730_safety_check_error_occur(SL_IEC60730_WATCHDOG_POST_FAIL);
    sl_iec60730_safe_state(SL_IEC60730_WATCHDOG_POST_FAIL);
  }

  LABEL_DEF(IEC60730_TEST_RESTART_WDOG_BKPT);
  __NOP();
  while (1) {
    if (wdog_restart_enable) {
      sl_iec60730_restart_watchdogs();
    }
  }
}

void integration_test_run_process_action(void)
{
  sl_iec60730_bist();
}
