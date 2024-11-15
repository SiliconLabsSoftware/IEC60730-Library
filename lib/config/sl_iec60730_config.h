/***************************************************************************/ /**
 * @file
 * @brief Library configuration
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

#ifndef SL_IEC60730_CONFIG_H
#define SL_IEC60730_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup IEC60730_IRQ_TEST
 * @{
 *****************************************************************************/

// <h> IEC60730_IRQ

// <o SL_IEC60730_IRQ_TYPE_VARIABLE> Data type for iec60730_IRQExecCount variables
/// Data type for iec60730_IRQExecCount variables
// <uint8_t=> uint8_t
// <uint16_t=> uint16_t
// <uint32_t=> uint32_t
// <i> Default: uint8_t
#define SL_IEC60730_IRQ_TYPE_VARIABLE           uint8_t
// <q SL_IEC60730_IRQ_STATUS_ENABLE> Enable function to get the value of failed irqs
/// Enable using sl_iec60730_get_irq_index_failed() function to get the value of failed irqs
// <i> Default: 0
#define SL_IEC60730_IRQ_STATUS_ENABLE           0

// </h>

/** @} (end addtogroup IEC60730_IRQ_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_WDOG_TEST
 * @{
 *****************************************************************************/

// <h> IEC60730_WATCHDOG

// <q SL_IEC60730_WDOG0_ENABLE> Enable Watchdog 0
/// Enable Watchdog 0.
// <i> Default: 1
#ifndef SL_IEC60730_WDOG0_ENABLE
#define SL_IEC60730_WDOG0_ENABLE                1
#endif //SL_IEC60730_WDOG0_ENABLE

// <q SL_IEC60730_WDOG1_ENABLE> Enable Watchdog 1
/// Enable Watchdog 1.
// <i> Default: 0
#ifndef SL_IEC60730_WDOG1_ENABLE
#define SL_IEC60730_WDOG1_ENABLE                0
#endif //SL_IEC60730_WDOG1_ENABLE

// <q SL_IEC60730_SAVE_STAGE_ENABLE> Enable saving iec60730_watchdog_state to backup RAM on Series 2.
/// Enable saving iec60730_watchdog_state to backup RAM on Series 2.
// <i> Default: 0
#define SL_IEC60730_SAVE_STAGE_ENABLE           0

// <q SL_IEC60730_RSTCAUSES_CLEAR_ENABLE> Reset cause flags in the RSTCASUES register.
/// Reset cause flags in the RSTCASUES register.
// <i> Default: 1
#define SL_IEC60730_RSTCAUSES_CLEAR_ENABLE      1

// </h>

/** @} (end addtogroup IEC60730_WDOG_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_SYSTEM_CLOCK_TEST
 * @{
 *****************************************************************************/

// <h> IEC60730_SYS_CLOCK

// <o SL_IEC60730_TEST_CLOCK_MULTIPLIER> Determines how many entrances into sl_iec60730_test_clock_tick() occur before bist frequency test executes
/// Determines how many entrances into sl_iec60730_test_clock_tick() occur before bist frequency test executes
// <i> Default: 10
#define SL_IEC60730_TEST_CLOCK_MULTIPLIER       10

// <o SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ> This value is used to compare with the system clock counter
/// This define holds the ratio value between system clock timer interrupt period and
/// test clock timer interrupt period
// <i> Default: 10
#define SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ     10

// <o SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE> The tolerance of test
/// (OEM set value of the define)
/// is typical (+/-) 10%-49% of the SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ.
// <i> Default: 1
#define SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE    1

// </h>

/** @} (end addtogroup IEC60730_SYSTEM_CLOCK_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_VARIABLE_MEMORY_TEST
 * @{
 *****************************************************************************/

// <h> IEC60730_VMC

// <q SL_IEC60730_USE_MARCHX_ENABLE> Enable used the algorithm used in BIST is March-X.
/// Enable used the algorithm used in BIST is March-X
// <i> Default: 1
#ifndef SL_IEC60730_USE_MARCHX_ENABLE
#define SL_IEC60730_USE_MARCHX_ENABLE           1
#endif // SL_IEC60730_USE_MARCHX_ENABLE

// <o SL_IEC60730_VAR_BLOCKS_PER_BIST> SL_IEC60730_VAR_BLOCKS_PER_BIST
/// Number of ram blocks tested per BIST
// <i> Default: 256
#define SL_IEC60730_VAR_BLOCKS_PER_BIST         256

// <s.14 SL_STACK_OVERFLOW_CONST_GUARD_VALUE_0> Pattern 0 is used for stack overflow
/// Pattern 0 is used for stack overflow
// <i> Default: (0xEEEEEEEEuL)
#define SL_STACK_OVERFLOW_CONST_GUARD_VALUE_0   (0xEEEEEEEEuL)

// <s.14 SL_STACK_OVERFLOW_CONST_GUARD_VALUE_1> Pattern 1 is used for stack overflow
/// Pattern 1 is used for stack overflow
// <i> Default: (0xCCCCCCCCuL)
#define SL_STACK_OVERFLOW_CONST_GUARD_VALUE_1   (0xCCCCCCCCuL)

// <s.14 SL_STACK_OVERFLOW_CONST_GUARD_VALUE_2> Pattern 2 is used for stack overflow
/// Pattern 2 is used for stack overflow
// <i> Default: (0xBBBBBBBBuL)
#define SL_STACK_OVERFLOW_CONST_GUARD_VALUE_2   (0xBBBBBBBBuL)

// <s.14 SL_STACK_OVERFLOW_CONST_GUARD_VALUE_3> Pattern 3 is used for stack overflow
/// Pattern 3 is used for stack overflow
// <i> Default: (0xDDDDDDDDuL)
#define SL_STACK_OVERFLOW_CONST_GUARD_VALUE_3   (0xDDDDDDDDuL)

// </h>

/** @} (end addtogroup IEC60730_VARIABLE_MEMORY_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_INVARIABLE_MEMORY_TEST
 * @{
 *****************************************************************************/

// <h> IEC60730_IMC

// <q SL_IEC60730_CRC_DEBUG_ENABLE> Enable debug IMC module
/// Use this definition for debugging purposes.
// <i> Default: 1
#define SL_IEC60730_CRC_DEBUG_ENABLE            1

// <o SL_IEC60730_INVAR_BLOCKS_PER_BIST> SL_IEC60730_INVAR_BLOCKS_PER_BIST
/// Number of blocks per test bist
// <i> Default: 512
#define SL_IEC60730_INVAR_BLOCKS_PER_BIST       512

// <o SL_IEC60730_FLASH_BLOCK> Definition is size of block of Flash
/// The value of this
/// definition SHOULD be a power of two. The minimal value SHOULD be 4 and
/// maximum value SHOULD be the alignment value that is defined in linker (GCC
/// compiler) or in configuration of IAR. Our example use alignment 64 for
/// #check_sum variable. So that in our example the #SL_IEC60730_FLASH_BLOCK definition
/// is 64. User can refer to our example linker on how to provide #check_sum
/// variable with alignment 64.
// <i> Default: 64
#define SL_IEC60730_FLASH_BLOCK                 64

// <e SL_IEC60730_CRC_USE_SW_ENABLE> Enable CRC calculation using software instead of hardware
/// The library support both hardware and software CRC. Use this definition in
/// case the user use software CRC. The default hardware is used.
// <i> Default: 0
#ifndef SL_IEC60730_CRC_USE_SW_ENABLE
#define SL_IEC60730_CRC_USE_SW_ENABLE           0
#endif // SL_IEC60730_CRC_USE_SW_ENABLE

// <q SL_IEC60730_SW_CRC_TABLE_ENABLE> Enable pre-defined table for calculating CRC
/// If using #SL_IEC60730_CRC_USE_SW_ENABLE definition then the #SL_IEC60730_SW_CRC_TABLE_ENABLE definition is used for
/// using pre-defined table for calculating.
// <i> Default: 0
#ifndef SL_IEC60730_SW_CRC_TABLE_ENABLE
#define SL_IEC60730_SW_CRC_TABLE_ENABLE         0
#endif // SL_IEC60730_SW_CRC_TABLE_ENABLE
// </e>

// <q SL_IEC60730_USE_CRC_32_ENABLE> Enable CRC-32 for calculating the CRC value
/// Use this definition in case the user use CRC-32 for calculating the CRC value.
/// The default CRC-16 is used.
// <i> Default: 0
#ifndef SL_IEC60730_USE_CRC_32_ENABLE
#define SL_IEC60730_USE_CRC_32_ENABLE           0
#endif // SL_IEC60730_USE_CRC_32_ENABLE

// </h>

/** @} (end addtogroup IEC60730_INVARIABLE_MEMORY_TEST) */
/** @} (end addtogroup efr32_iec60730) */

// <<< end of configuration section >>>

#endif /* SL_IEC60730_CONFIG_H */
