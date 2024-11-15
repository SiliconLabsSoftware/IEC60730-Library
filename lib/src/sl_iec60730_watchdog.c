/***************************************************************************/ /**
 * @file
 * @brief Watchdog check
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

#include "sl_iec60730_internal.h"

#ifdef UNIT_TEST_IEC60730_WATCHDOG_ENABLE
#include "unit_test_iec60730_watchdog.h"
#endif // UNIT_TEST_IEC60730_WATCHDOG_ENABLE

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup IEC60730_WDOG_Test
 * @{
 *****************************************************************************/

/// Global variable to store Watchdog states. With load in IEC60730_DATA_NO_CLEAR is region No clear at startup.
volatile sl_iec60730_test_watchdog_t iec60730_watchdog_state IEC60730_DATA_NO_CLEAR;
/// Local variable to store Watchdog instance under testing. With load in IEC60730_DATA_NO_CLEAR is region No clear at startup.
static uint8_t iec60730_watchdog_count IEC60730_DATA_NO_CLEAR;

// Define Watchdog configuration
#if (_SILICON_LABS_32B_SERIES < 2)
const sl_iec60730_watchdog_t  SL_IEC60730_WDOG_INST_ARR[SL_IEC60730_WDOGINST_NUMB] = {
#if (SL_IEC60730_WDOG0_ENABLE == 1)
  { .SL_WDOG = SL_IEC60730_WDOG_INST(0), .rst = RMU_RSTCAUSE_WDOGRST },
#endif //(SL_IEC60730_WDOG0_ENABLE == 1)
#if (SL_IEC60730_WDOG1_ENABLE == 1)
  { .SL_WDOG = SL_IEC60730_WDOG_INST(1), .rst = RMU_RSTCAUSE_WDOGRST },
#endif //(SL_IEC60730_WDOG1_ENABLE == 1)
};
#else
const sl_iec60730_watchdog_t  SL_IEC60730_WDOG_INST_ARR[SL_IEC60730_WDOGINST_NUMB] = {
#if (SL_IEC60730_WDOG0_ENABLE == 1)
  { .SL_WDOG = SL_IEC60730_WDOG_INST(0), .rst = EMU_RSTCAUSE_WDOG0 },
#endif //(SL_IEC60730_WDOG0_ENABLE == 1)
#if (SL_IEC60730_WDOG1_ENABLE == 1)
  { .SL_WDOG = SL_IEC60730_WDOG_INST(1), .rst = EMU_RSTCAUSE_WDOG0 },
#endif //(SL_IEC60730_WDOG1_ENABLE == 1)
};
#endif

/**************************************************************************/ /**
 * private IEC60730 Watchdog Restart
 *
 * @returns None.
 *
 * This function abstracts the differences between the EFR32 family watchdog
 * timers.
 *****************************************************************************/
static void sli_iec60730_restart_watchdog(WDOG_TypeDef *wdog);

/**************************************************************************/ /**
 * private IEC60730 Set Watchdog Timeout Min
 *
 * @param  iec60730_wachdog input pointer point to watchdog want to set timeout min
 *
 * @returns None.
 *
 * This function set config timeout for watchdog passing down the min value
 *****************************************************************************/
static void sli_iec60730_set_watchdog_timout_min(const sl_iec60730_watchdog_t* iec60730_wachdog);

void sli_iec60730_set_watchdog_timout_min(const sl_iec60730_watchdog_t* iec60730_wachdog)
{
#ifndef UNIT_TEST_IEC60730_WATCHDOG_ENABLE
// Min value for PERSEL is zero
#if (_SILICON_LABS_32B_SERIES < 2)
  do {
    while ((iec60730_wachdog->SL_WDOG->SYNCBUSY & WDOG_SYNCBUSY_CTRL) != 0U) {
      // wait syncbusy
    }
    iec60730_wachdog->SL_WDOG->CTRL &= ~(uint32_t) _WDOG_CTRL_PERSEL_MASK;
  } while (0);
#else // Series 2 devices
#ifdef WDOG_HAS_SET_CLEAR
  do {
    iec60730_wachdog->SL_WDOG->EN_CLR = WDOG_EN_EN;
#if (defined _SILICON_LABS_32B_SERIES_2_CONFIG_3) \
    || (defined _SILICON_LABS_32B_SERIES_2_CONFIG_4)
    while ((iec60730_wachdog->SL_WDOG->EN & WDOG_EN_DISABLING) != 0U) {
      // wait disabling watchdog
    }
#endif // _SILICON_LABS_32B_SERIES_2_CONFIG_3 || _SILICON_LABS_32B_SERIES_2_CONFIG_4
    iec60730_wachdog->SL_WDOG->CFG_CLR = _WDOG_CFG_PERSEL_MASK;
    iec60730_wachdog->SL_WDOG->EN_SET  = WDOG_EN_EN;
  } while (0);
#else
  do {
    iec60730_wachdog->SL_WDOG->EN &= ~_WDOG_EN_MASK;
#if (defined _SILICON_LABS_32B_SERIES_2_CONFIG_3) \
    || (defined _SILICON_LABS_32B_SERIES_2_CONFIG_4)
    while ((iec60730_wachdog->SL_WDOG->EN & WDOG_EN_DISABLING) != 0U) {
      // wait disabling watchdog
    }
#endif // _SILICON_LABS_32B_SERIES_2_CONFIG_3 || _SILICON_LABS_32B_SERIES_2_CONFIG_4
    iec60730_wachdog->SL_WDOG->CFG &= ~(uint32_t) _WDOG_CFG_PERSEL_MASK;
    iec60730_wachdog->SL_WDOG->EN |= WDOG_EN_EN;
  } while (0);
#endif // WDOG_HAS_SET_CLEAR
#endif // (_SILICON_LABS_32B_SERIES < 2)
#else
  (void) iec60730_wachdog;
  unit_test_iec60730_watchdog_mock_set_watchdog_timout_min();
#endif // UNIT_TEST_IEC60730_WATCHDOG_ENABLE
}

void sli_iec60730_restart_watchdog(WDOG_TypeDef *wdog)
{
  if (NULL == wdog) {
    return;
  }
#if (_SILICON_LABS_32B_SERIES < 2)
  // WDOG should not be fed while it is disabled.
  if (!(wdog->CTRL & WDOG_CTRL_EN)) {
    return;
  }

  // If a previous clearing is synchronized to the LF domain, there
  // is no point in waiting for it to complete before clearing over again.
  // This avoids stalling the core in the typical use case where some idle loop
  // keeps clearing WDOG.
  if (wdog->SYNCBUSY & WDOG_SYNCBUSY_CMD) {
    return;
  }
  // Before writing to the WDOG_CMD register, make sure that
  // any previous write to the WDOG_CTRL is complete.
  while ((wdog->SYNCBUSY & WDOG_SYNCBUSY_CTRL) != 0U) {
  }

  wdog->CMD = WDOG_CMD_CLEAR;

#else // Series 2 devices

  SL_IEC60370_DECLARE_IRQ_STATE

  // WDOG should not be fed while it is disabled.
  if ((wdog->EN & WDOG_EN_EN) == 0U) {
    return;
  }

  // We need an atomic section around the check for sync and the clear command
  // because sending a clear command while a previous command is being synchronized
  // will cause a BusFault.
  SL_IEC60370_ENTER_ATOMIC();
  if ((wdog->SYNCBUSY & WDOG_SYNCBUSY_CMD) == 0U) {
    wdog->CMD = WDOG_CMD_CLEAR;
  }
  SL_IEC60370_EXIT_ATOMIC();

#endif
}

void sl_iec60730_restart_watchdogs(void)
{
#if (defined(WDOG0) && (SL_IEC60730_WDOG0_ENABLE == 1))
  sli_iec60730_restart_watchdog(SL_IEC60730_WDOG_INST(0));
#endif
#if (defined(WDOG1) && (SL_IEC60730_WDOG1_ENABLE == 1))
  sli_iec60730_restart_watchdog(SL_IEC60730_WDOG_INST(1));
#endif
}

#ifdef UNIT_TEST_IEC60730_WATCHDOG_ENABLE
void sl_iec60730_watchdog_count_reset(void)
{
  iec60730_watchdog_count = 0;
}

void sl_iec60730_watchdog_count_set(uint8_t count)
{
  iec60730_watchdog_count = count;
}
#endif // UNIT_TEST_IEC60730_WATCHDOG_ENABLE

sl_iec60730_test_result_t sl_iec60730_watchdog_post(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  volatile uint32_t time_out;
  // Check for the power on reset and watchdog reset condition
  if (SL_IEC60730_RST_POR && !SL_IEC60730_RST_WDOGS) {
    LABEL_DEF(IEC60730_WATCHDOG_POST_POR_RESET_BKPT);
    iec60730_watchdog_count = 0;
    // Set watchdog state to TESTING
    iec60730_watchdog_state = SL_IEC60730_WATCHDOG_TESTING;
  } else { // It's not power on reset context
    LABEL_DEF(IEC60730_WATCHDOG_POST_WORKING_BKPT);
#ifndef UNIT_TEST_IEC60730_WATCHDOG_ENABLE
#if (SL_IEC60730_SAVE_STAGE_ENABLE == 1) && (_SILICON_LABS_32B_SERIES == 2)
    if (SL_IEC60730_RST_EM4) {
      iec60730_watchdog_state =
        (sl_iec60730_test_watchdog_t) SL_IEC60730_BURAM_READ(SL_IEC60730_BURAM,
                                                             SL_IEC60730_BURAM_IDX);
    }
#endif
#endif // UNIT_TEST_IEC60730_WATCHDOG_ENABLE
    if (iec60730_watchdog_state == SL_IEC60730_WATCHDOG_TESTING) {
      LABEL_DEF(IEC60730_WATCHDOG_POST_TESTING_BKPT);
      // Prevent unexpected changed value when reset or
      // return from power saving mode
      if (iec60730_watchdog_count >= SL_IEC60730_WDOGINST_NUMB) {
        goto WATCHDOG_POST_DONE;
      }
      // Check for the validity of the Watchdog reset reason
      if (SL_IEC60730_WDOG_INST_ARR[iec60730_watchdog_count].rst & SL_IEC60730_RSTCAUSE) {
        // Next Watchdog to test
        iec60730_watchdog_count++;

        // If all Watchdogs are tested then finish
        if (iec60730_watchdog_count >= SL_IEC60730_WDOGINST_NUMB) {
          iec60730_watchdog_state = SL_IEC60730_WATCHDOG_VALID;

#ifndef UNIT_TEST_IEC60730_WATCHDOG_ENABLE
#if (SL_IEC60730_SAVE_STAGE_ENABLE == 1) && (_SILICON_LABS_32B_SERIES == 2)
          // Write to backup ram
          SL_IEC60730_BURAM_WRITE(SL_IEC60730_BURAM,
                                  SL_IEC60730_BURAM_IDX,
                                  iec60730_watchdog_state);
#endif
#endif // UNIT_TEST_IEC60730_WATCHDOG_ENABLE
          result = SL_IEC60730_TEST_PASSED;
// Clear reset flags of the reset causes register
#if  (SL_IEC60730_RSTCAUSES_CLEAR_ENABLE == 1)
          SL_IEC60730_RSTCAUSES_CLEAR();
#endif
        }
      } else { // Watchdog reset checking failed
        goto WATCHDOG_POST_DONE;
      }
    } else { // Watchdog is not in the testing state
      // If reset causes are not POR and not Watchdog reset then no action
      if (!SL_IEC60730_RST_POR && !SL_IEC60730_RST_WDOGS) {
        result = SL_IEC60730_TEST_PASSED;
      } else { // Otherwise system has failed
        goto WATCHDOG_POST_DONE;
      }
    }
  }

  if (iec60730_watchdog_state == SL_IEC60730_WATCHDOG_TESTING) {
    LABEL_DEF(IEC60730_WATCHDOG_POST_TIMEOUT_BKPT);
    // Set timeout to minimum
    sli_iec60730_set_watchdog_timout_min(&SL_IEC60730_WDOG_INST_ARR[iec60730_watchdog_count]);

    // Wait for watchdog reset
    time_out = SL_IEC60730_WDOG_WAIT_TIMEOUT;
    while (time_out > 0) {
      time_out--;
    }
  }
  LABEL_DEF(IEC60730_WATCHDOG_POST_END_BKPT);

  WATCHDOG_POST_DONE:
  return result;
}

/** @} (end addtogroup IEC60730_WDOG_Test) */
/** @} (end addtogroup efr32_iec60730) */
