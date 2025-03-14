/***************************************************************************/ /**
 * @file integration_test_iec60730_watchdog.c
 * @brief setup integration test watchdog
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
