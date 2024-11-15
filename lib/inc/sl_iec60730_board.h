/***************************************************************************/ /**
 * @file  iec60730_board
 * @brief Internal definitions
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

#ifndef SL_IEC60730_BOARD_H
#define SL_IEC60730_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// Standard libraries
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup IEC60730_INVARIABLE_MEMORY_TEST
 * @{
 *****************************************************************************/

/* Invariable Memory */
/// Flash start address
extern uint32_t flash_start;

/// The #SL_IEC60730_ROM_START definition describes the start address of Flash for CRC
/// calculation. This definition MUST be alignment of #SL_IEC60730_FLASH_BLOCK and can be
/// changed by user so that the value DOES NOT exceed the end address of Flash
/// (the #SL_IEC60730_ROM_END definition). Otherwise, #sl_iec60730_imc_post and #sl_iec60730_imc_bist
/// return #SL_IEC60730_TEST_FAILED. The default value of the #SL_IEC60730_ROM_START can be found in
/// sl_iec60730_board.h file.
///
/// In case you change value of this definition then you SHOULD change the 5th
/// parameter of the script \ref sl_iec60730_call_crcXY when running the Post Build.
///
/// In our example, we use the variable flash_start provided by the linker file
/// to simply define the start address of Flash. User can redefine #SL_IEC60730_ROM_START
/// as follows.
/// ```
/// #define SL_IEC60730_ROM_START  ((uint32_t *)(&flash_start))
/// ```
#ifndef SL_IEC60730_ROM_END
#define SL_IEC60730_ROM_START ((uint32_t *) (&flash_start))
#endif

/// The #SL_IEC60730_ROM_END definition describes the end address of Flash. It is the
/// address of #check_sum variables. In this version, users SHOULD not
/// change this definition. Refer to the example in
/// \ref invariable_memory_check_example_flowchart, to better understand the
/// #SL_IEC60730_ROM_START, and #SL_IEC60730_ROM_END definitions.
#ifndef SL_IEC60730_ROM_END
#define SL_IEC60730_ROM_END ((uint32_t *) (&check_sum))
#endif

/** @} (end addtogroup IEC60730_INVARIABLE_MEMORY_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_VARIABLE_MEMORY_TEST
 * @{
 *****************************************************************************/

/* Variable Memory */
extern uint32_t classb_start;
extern uint32_t classb_end;
extern uint32_t __StackTop;
extern uint32_t ram_start;
extern uint32_t class_b_limit;

#if defined(__GNUC__)
extern uint32_t stack_check;
#define STACK_CHECK     ((uint32_t *) (&stack_check))
#elif defined(__ICCARM__)
#pragma section = ".stack_bottom"
#define STACK_CHECK     __section_begin(".stack_bottom")
#else // unknown toolchain
#error Unrecognized toolchain in sl_iec60730_toolchain.h
#endif

/// RAM starting address
#define RAM_START       ((uint32_t *) (&ram_start))
/// RAM ending address
#define RAM_END         ((uint32_t *) ((uint32_t) RAM_BACKUP - 1))
/// RAM backup address
#define RAM_BACKUP      ((uint32_t *) (&__StackTop))

/// ClassbRAM starting address
#define CLASSB_START    ((uint32_t *) (&classb_start))
/// ClassbRAM ending address
#define CLASSB_END      ((uint32_t *) ((uint32_t) (&class_b_limit) - 1))

/// Example RAM starting address test
#define RAMTEST_START   CLASSB_START
/// Example RAM ending address test
#define RAMTEST_END     CLASSB_END

/** @} (end addtogroup IEC60730_VARIABLE_MEMORY_TEST) */
/** @} (end addtogroup efr32_iec60730) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SL_IEC60730_BOARD_H */
