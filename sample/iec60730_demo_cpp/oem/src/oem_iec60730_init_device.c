/***************************************************************************/ /**
 * @file oem_oem_iec60730_init_device.c
 * @brief Init device
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "oem_iec60730.h"

// Wdg config
const WDOG_Init_TypeDef oem_wdog0_init  = WDOG_INIT_DEFAULT;
const WDOG_Init_TypeDef oem_wdog1_init = WDOG_INIT_DEFAULT;
// HFXO config
const CMU_HFXOInit_TypeDef oem_hfxo_init = CMU_HFXOINIT_DEFAULT;
#if (_SILICON_LABS_32B_SERIES_2_CONFIG >= 1)
// DPLL configured default 80Mhz
const CMU_DPLLInit_TypeDef oem_dpll_init = CMU_DPLLINIT_DEFAULT;
#endif

static void oem_clock_config(void);
static void oem_nvic_init(void);

void oem_system_config(void)
{
  oem_clock_config();
  oem_nvic_init();
  oem_timers_init();
#ifndef IEC60730_DISABLE_COMM
  oem_comm_init();
#endif
#if (defined(WDOG0) && (SL_IEC60730_WDOG0_ENABLE == 1))
  WDOGn_Init(SL_IEC60730_WDOG_INST(0), &oem_wdog0_init);
#endif
#if (defined(WDOG1) && (SL_IEC60730_WDOG1_ENABLE == 1))
  WDOGn_Init(SL_IEC60730_WDOG_INST(1), &oem_wdog1_init);
#endif

  CORE_ATOMIC_IRQ_ENABLE();
}

void oem_clock_config(void)
{
#if (_SILICON_LABS_32B_SERIES_2_CONFIG >= 1)
#if (_SILICON_LABS_32B_SERIES_2_CONFIG > 1)
  CMU_ClockEnable(cmuClock_HFXO, true);
  CMU_ClockEnable(cmuClock_DPLL0, true);

  /* Enable HFXO */
  CMU_HFXOInit(&oem_hfxo_init);
  CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
  /* Configure DPLL */
  // CMU_HFRCODPLLBandSet(cmuHFRCODPLLFreq_80M0Hz);
  if (!CMU_DPLLLock(&oem_dpll_init)) {
    while (1)
      ;
  }

  /* System clock is default FSRCO - 20Mhz */
  /* Watchdog and LETIMER clock is default LFRCO - 32Khz */
  /* FSRCO clocks for timers at fixed 20Mhz */
  CMU_ClockSelectSet(cmuClock_TIMER0, cmuSelect_FSRCO);

  CMU_ClockSelectSet(cmuClock_LETIMER0, cmuSelect_LFRCO);

#if (_SILICON_LABS_32B_SERIES_2_CONFIG > 1)
  /* System clock is DPLL - 80Mhz */
  // CMU_ClockSelectSet(cmuClock_SYSCLK, cmuSelect_HFRCODPLLRT);
  // CMU_ClockSelectSet(cmuClock_TIMER0, cmuSelect_HFRCODPLLRT);
  /* Watchdog and LETIMER clock is default LFRCO - 32Khz */

  CMU_ClockEnable(cmuClock_TIMER0, true);
  CMU_ClockEnable(cmuClock_LETIMER0, true);
#if (defined(WDOG0) && (SL_IEC60730_WDOG0_ENABLE == 1))
  CMU_ClockEnable(cmuClock_WDOG0, true);
#endif
#if (defined(WDOG1) && (SL_IEC60730_WDOG1_ENABLE == 1))
  CMU_ClockEnable(cmuClock_WDOG1, true);
#endif
  CMU_ClockEnable(cmuClock_GPCRC, true);
  CMU_ClockEnable(cmuClock_BURAM, true);
#else
  /* System clock is DPLL - 80Mhz */
  CMU_ClockSelectSet(cmuClock_SYSCLK, cmuSelect_HFRCODPLL);
  CMU_ClockSelectSet(cmuClock_TIMER0, cmuSelect_HFRCODPLL);
/* Watchdog and LETIMER clock is default LFRCO - 32Khz */
#endif
#endif
#else
  /* System clock is default HFRCO - 19Mhz */
  /* Watchdog clock is default LFRCO - 32Khz */
  /* FSRCO clocks for timers at 19Mhz */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);
/* Enable Low energy Peripherals clock */
#if defined(CMU_HFCORECLKEN0_LE)
  CMU_ClockEnable(cmuClock_HFLE, true);
#else
  CMU->HFBUSCLKEN0 |= (CMU_HFBUSCLKEN0_LE & _CMU_HFBUSCLKEN0_MASK);
#endif

  /* LFACLK clock is selected as LFRCO */
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
  /* Enable clock for LETIMER0 as LFRCO - 32.768 kHz*/
  CMU_ClockEnable(cmuClock_LETIMER0, true);
  /* Enable clock of CRC */
  CMU_ClockEnable(cmuClock_GPCRC, true);
#endif
}

void oem_nvic_init(void)
{
  /* One Group Priority */
  NVIC_SetPriorityGrouping(7);
  /* Timer 0 */
  NVIC_SetPriority(TIMER0_IRQn, 1);
  NVIC_EnableIRQ(TIMER0_IRQn);

  /* LETIMER 0 */
  NVIC_SetPriority(LETIMER0_IRQn, 1);
  NVIC_EnableIRQ(LETIMER0_IRQn);
}
