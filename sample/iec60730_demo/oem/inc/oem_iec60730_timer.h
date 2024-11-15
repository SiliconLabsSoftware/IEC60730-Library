/***************************************************************************/ /**
 * @file ie60730_oem_timer.h
 * @brief Oem timer header
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

#ifndef OEM_IE60730_TIMER_H
#define OEM_IE60730_TIMER_H

typedef enum {
  CLK_SYS  = 0,
  CLK_HFXO = 1
} oem_test_clock_source_t;

/* Test Timer instance */
#if (_SILICON_LABS_32B_SERIES == 2)
#if ((defined SL_IEC60730_NON_SECURE_ENABLE) && (!defined(SL_TRUSTZONE_SECURE)))
#define TIMER_10MS  TIMER0_NS
#define TIMER_100MS LETIMER0_NS
#else
#define TIMER_10MS  TIMER0
#define TIMER_100MS LETIMER0
#endif
#else // Series 1 devices
#define TIMER_10MS  TIMER0
#define TIMER_100MS LETIMER0
#endif // (_SILICON_LABS_32B_SERIES == 2)

/* Init structure for timers */
#if defined(_TIMER_CTRL_MODE_MASK)
#if defined(TIMER_CTRL_X2CNT) \
  && (defined(TIMER_CTRL_ATI) || defined(TIMER_CFG_ATI))
#define TIMER_INIT_TEST_10MS                                                \
  { false,                /* Enable timer when initialization completes. */ \
    false,                /* Stop counter during debug halt. */             \
    timerPrescale4,       /* DIV 16. */                                     \
    timerClkSelHFPerClk,  /* Select HFPER / HFPERB clock. */                \
    false,                /* Not 2x count mode. */                          \
    false,                /* No ATI. */                                     \
    timerInputActionNone, /* No action on falling input edge. */            \
    timerInputActionNone, /* No action on rising input edge. */             \
    _TIMER_CTRL_MODE_UP,  /* Up-counting. */                                \
    false, /* Do not clear DMA requests when DMA channel is active. */      \
    false, /* Select X2 quadrature decode mode (if used). */                \
    false, /* Disable one shot. */                                          \
    false, /* Not started/stopped/reloaded by other timers. */              \
    true }
#else
#define TIMER_INIT_TEST_10MS                                                  \
  {                                                                           \
    false,                  /* Enable timer when initialization completes. */ \
    false,                  /* Stop counter during debug halt. */             \
    timerPrescale4,         /* DIV 16. */                                     \
    timerClkSelHFPerClk,    /* Select HFPER / HFPERB clock. */                \
    timerInputActionNone,   /* No action on falling input edge. */            \
    timerInputActionNone,   /* No action on rising input edge. */             \
    _TIMER_CTRL_MODE_UP,    /* Up-counting. */                                \
    false,   /* Do not clear DMA requests when DMA channel is active. */      \
    false,   /* Select X2 quadrature decode mode (if used). */                \
    false,   /* Disable one shot. */                                          \
    false    /* Not started/stopped/reloaded by other timers. */              \
  }
#endif
#endif

#if defined(_TIMER_CFG_MODE_MASK)
#if defined(TIMER_CTRL_X2CNT) \
  && (defined(TIMER_CTRL_ATI) || defined(TIMER_CFG_ATI))
#if (defined(TIMER_CTRL_DISSYNCOUT) || defined(TIMER_CFG_DISSYNCOUT))
#define TIMER_INIT_TEST_10MS                                                          \
  {                                                                                   \
    true,                 /* Enable timer when initialization completes. */           \
    false,                /* Stop counter during debug halt. */                       \
    timerPrescale4,       /* DIV 16. */                                               \
    timerClkSelHFPerClk,  /* Select HFPER / HFPERB clock. */                          \
    false,                /* Not 2x count mode. */                                    \
    false,                /* No ATI. */                                               \
    false,                /* No RSSCOIST. */                                          \
    timerInputActionNone, /* No action on falling input edge. */                      \
    timerInputActionNone, /* No action on rising input edge. */                       \
    timerModeUp,          /* Up-counting. */                                          \
    false,                /* Do not clear DMA requests when DMA channel is active. */ \
    false,                /* Select X2 quadrature decode mode (if used). */           \
    false,                /* Disable one shot. */                                     \
    false,                /* Not started/stopped/reloaded by other timers. */         \
    false                 /* Disable ability to start/stop/reload other timers. */    \
  };
#else
#define TIMER_INIT_TEST_10MS                                                \
  { false,                /* Enable timer when initialization completes. */ \
    false,                /* Stop counter during debug halt. */             \
    timerPrescale4,       /* DIV 16. */                                     \
    timerClkSelHFPerClk,  /* Select HFPER / HFPERB clock. */                \
    false,                /* Not 2x count mode. */                          \
    false,                /* No ATI. */                                     \
    timerInputActionNone, /* No action on falling input edge. */            \
    timerInputActionNone, /* No action on rising input edge. */             \
    timerModeUp,   /* Up-counting. */                                       \
    false, /* Do not clear DMA requests when DMA channel is active. */      \
    false, /* Select X2 quadrature decode mode (if used). */                \
    false, /* Disable one shot. */                                          \
    false, /* Not started/stopped/reloaded by other timers. */              \
    true }
#endif
#else
#define TIMER_INIT_TEST_10MS                                                \
  {                                                                         \
    false,                /* Enable timer when initialization completes. */ \
    false,                /* Stop counter during debug halt. */             \
    timerPrescale4,       /* DIV 16. */                                     \
    timerClkSelHFPerClk,  /* Select HFPER / HFPERB clock. */                \
    timerInputActionNone, /* No action on falling input edge. */            \
    timerInputActionNone, /* No action on rising input edge. */             \
    timerModeUp,   /* Up-counting. */                                       \
    false, /* Do not clear DMA requests when DMA channel is active. */      \
    false, /* Select X2 quadrature decode mode (if used). */                \
    false, /* Disable one shot. */                                          \
    false  /* Not started/stopped/reloaded by other timers. */              \
  }
#endif
#endif

#if defined(LETIMER_CTRL_RTCC0TEN)
#define TIMER_INIT_TEST_100MS                                                \
  {                                                                          \
    false,           /* Enable timer when initialization completes. */       \
    false,           /* Stop counter during debug halt. */                   \
    false,           /* Do not start counting on RTC COMP0 match. */         \
    false,           /* Do not start counting on RTC COMP1 match. */         \
    true,            /* Load COMP0 into CNT on underflow. */                 \
    false,           /* Do not load COMP1 into COMP0 when REP0 reaches 0. */ \
    0,               /* Idle value 0 for output 0. */                        \
    0,               /* Idle value 0 for output 1. */                        \
    letimerUFOANone, /* No action on underflow on output 0. */               \
    letimerUFOANone, /* No action on underflow on output 1. */               \
    letimerRepeatFree, /* Count until stopped by SW. */                      \
    0                  /* Use default top Value. */                          \
  }
#else
#define TIMER_INIT_TEST_100MS                                                \
  {                                                                          \
    false,           /* Enable timer when initialization completes. */       \
    false,           /* Stop counter during debug halt. */                   \
    true,            /* Load COMP0 into CNT on underflow. */                 \
    false,           /* Do not load COMP1 into COMP0 when REP0 reaches 0. */ \
    0,               /* Idle value 0 for output 0. */                        \
    0,               /* Idle value 0 for output 1. */                        \
    letimerUFOANone, /* No action on underflow on output 0. */               \
    letimerUFOANone, /* No action on underflow on output 1. */               \
    letimerRepeatFree, /* Count until stopped by SW. */                      \
    0                  /* Use default top Value. */                          \
  }
#endif

/* Value of the timer counter */
#define TEST_TIMER_RESET_VAL  0
#define TEST_TIMER_PRESET_VAL 3276

/* Timers is clocked by FSRCO 20Mhz */
/* Timer prescaler is 2 so counter clock is 5Mhz () */
/* Counter value for 10ms timing */
#define TEST_TIMER_10MS_VAL   50000
/* Counter value for 100ms timing */
#define TEST_TIMER_100MS_VAL  3276

void oem_timers_init(void);
void oem_timers_enable(void);
void oem_timers_disable(void);
void oem_adj_top_val_timer(void);

#endif /* OEM_IE60730_TIMER_H */
