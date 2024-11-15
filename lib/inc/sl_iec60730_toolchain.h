/***************************************************************************/ /**
 * @file
 * @brief Compiler abstraction
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

#ifndef SL_IEC60730_TOOLCHAIN_H
#define SL_IEC60730_TOOLCHAIN_H

/**************************************************************************/ /**
 *
 * @addtogroup toolchain_group Toolchain Abstraction
 *
 * @brief Macros for toolchain abstraction.
 *
 * # Introduction #
 *
 * This header file contains macros that are used to provide an abstraction
 * for toolchain use in source code.  The EFR32 compiler requires C-language
 * extensions in order to fully use features of the EFR32 architecture.  All
 * compilers for EFR32 implement a set of extensions but use different names
 * and ways of implementing those extensions.  This header file provides
 * macros that are defined for each supported toolchain and can be used in
 * the source code.  This allows the source code to use EFR32 extensions and
 * remain independent of which toolchain is used for compilation.
 *
 * ## Prior Toolchain Abstraction Header File ##
 * Refer file: \link sl_iec60730_toolchain.h \endlink
 * ## Supported Toolchains ##
 *
 * - GNU GCC
 * - IAR ICCARM
 *
 * @{
 *
 *****************************************************************************/

/// Make sure there is a NULL defined if the toolchain does not provide it.
#ifndef NULL
#define NULL ((void *) 0)
#endif

// -------------------------------
// GCC for ARM Cortex-M
//
#if defined(__GNUC__)

/// __NOP should be declared in cmsis header core_cmInstr.h
/// extern void __NOP(void);
/// Macro to insert a no-operation (NOP) instruction.
#define NOP() __NOP()

/// No init at startup
#define IEC60730_VAR_NO_INIT              __attribute__((section(".noinit")))
/// No clear at startup
#define IEC60730_DATA_NO_CLEAR            __attribute__((section(".ram_no_clear")))

/// The #__STACK_BOTTOM macro is used to define
/// (.stack_bottom) section.
#ifndef __STACK_BOTTOM
#define __STACK_BOTTOM                    __attribute__((section(".stack_bottom")))
#endif /* __STACK_BOTTOM */

/// The #__CLASSB_RAM macro is used to define
/// (.classb_ram) section.
#ifndef __CLASSB_RAM
#define __CLASSB_RAM                      __attribute__((section(".classb_ram")))
#endif /* __CLASSB_RAM */

/// The #__RT_BUF macro is used to define
/// (.rt_buf) section.
#ifndef __RT_BUF
#define __RT_BUF                          __attribute__((section(".rt_buf")))
#endif /* __RT_BUF */

/// The #__OVERLAP macro is used to define
/// (.overlap) section.
#ifndef __OVERLAP
#define __OVERLAP                         __attribute__((section(".overlap")))
#endif /* __OVERLAP */

/// Add assembly code
#define LB_ASM        __asm__
/// Add label
#define LB_XSTR(x)    XSTR(x:)

// -------------------------------
// IAR for ARM Cortex-M
//
#elif defined(__ICCARM__)

// __NOP should be declared in cmsis header core_cmInstr.h
// extern void __NOP(void);
/// Macro to insert a no-operation (NOP) instruction.
#define NOP() __NOP()

// No init at startup
#define IEC60730_VAR_NO_INIT            __attribute__((section(".noinit")))
/// No clear at startup
#define IEC60730_DATA_NO_CLEAR          __attribute__((section(".ram_no_clear")))

/// The #__STACK_BOTTOM macro is used to define
/// (.stack_bottom) section.
#ifndef __STACK_BOTTOM
#define __STACK_BOTTOM                  __attribute__((section(".stack_bottom")))
#endif /* __STACK_BOTTOM */

/// The #__CLASSB_RAM macro is used to define
/// (.classb_ram) section.
#ifndef __CLASSB_RAM
#define __CLASSB_RAM                    __attribute__((section(".classb_ram")))
#endif /* __CLASSB_RAM */

/// The #__RT_BUF macro is used to define
/// (.rt_buf) section.
#ifndef __RT_BUF
#define __RT_BUF                        __attribute__((section(".rt_buf")))
#endif /* __RT_BUF */

/// The #__OVERLAP macro is used to define
/// (.overlap) section.
#ifndef __OVERLAP
#define __OVERLAP                       __attribute__((section(".overlap")))
#endif /* __OVERLAP */

#define LB_ASM        __asm
#define LB_XSTR(x)    XSTR(x::)

#else // unknown toolchain
#error Unrecognized toolchain in sl_iec60730_toolchain.h
#endif

/** @} */

#endif /* SL_IEC60730_TOOLCHAIN_H */
