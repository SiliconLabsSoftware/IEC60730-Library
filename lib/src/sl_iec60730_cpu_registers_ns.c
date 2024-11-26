/***************************************************************************/ /**
 * @file    sl_iec60730_cpu_registers_ns.c
 * @brief   CPU Registers check non-secure
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
#include "sl_iec60730_cpu_registers.h"

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup IEC60730_CPUREG_Test
 * @{
 *****************************************************************************/
#ifdef UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE
extern sl_iec60730_test_result_t unit_test_iec60730_cpu_registers_bist_ns_mock(void);
#endif // UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE

#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_msp_ns(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_psp_ns(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_msplim_ns(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_psplim_ns(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_control_ns(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_primask_ns(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_basepri_ns(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_faultmask_ns(void);
#endif

/**************************************************************************/ /**
 * public IEC60730 CPU Registers Built In Self Test
 *
 * @returns #sl_iec60730_test_result_t
 * @returns #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function will do a test of all the CPU core registers non-secure
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_bist_ns(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
#if (IEC60370_CPU == IEC60370_CM33)
#if defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)

#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Test Main Stack Pointer Non Secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_msp_ns()) {
    goto CPU_REGISTERS_BIST_NS_DONE;
  }

  /* Test Process Stack Pointer Non Secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_psp_ns()) {
    goto CPU_REGISTERS_BIST_NS_DONE;
  }

  /* Test Main Stack Pointer Limit Non Secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_msplim_ns()) {
    goto CPU_REGISTERS_BIST_NS_DONE;
  }

  /* Test Process Stack Pointer Limit None Secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_psplim_ns()) {
    goto CPU_REGISTERS_BIST_NS_DONE;
  }

  /* Test Control None Secure register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_control_ns()) {
    goto CPU_REGISTERS_BIST_NS_DONE;
  }

  /* Test Primask Non Secure register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_primask_ns()) {
    goto CPU_REGISTERS_BIST_NS_DONE;
  }

  /* Test BasePri Non Secure register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_basepri_ns()) {
    goto CPU_REGISTERS_BIST_NS_DONE;
  }

  /* Test FaultMask Non Secure register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_faultmask_ns()) {
    goto CPU_REGISTERS_BIST_NS_DONE;
  }

#ifdef UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE
  if (SL_IEC60730_TEST_FAILED == unit_test_iec60730_cpu_registers_bist_ns_mock()) {
    goto CPU_REGISTERS_BIST_NS_DONE;
  }
#endif // UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE

  result = SL_IEC60730_TEST_PASSED;

  CPU_REGISTERS_BIST_NS_DONE:

#endif
#else
#error the CM33 without TZ is not supported
#endif // defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
#endif // (IEC60370_CPU == IEC60370_CM33)

  return result;
}

/**************************************************************************/ /**
 * private IEC60730 CPU Register MSP Non-Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the MSP_NS (Main Stack Pointer, Non-Secure) register.
 * If MSP_NS is corrupted, the function stucks in an endless loop with
 * interrupts disabled. This state must be observed by another safety mechanism
 * (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555554
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_msp_ns(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i               \n"     /*Disable IRQ Interrupts*/
    "   MRS R3, CONTROL_NS    \n"     /*Save Control value */
    "   MRS R0, MSP_NS        \n"     /*Save stack value */
    "   LDR R1, =0xAAAAAAA8   \n"
    "   MSR MSP_NS, R1        \n"     /*load stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSP_NS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_MSP_NS_ASM_BKPT::  \n"
#endif
    "   MRS R2, MSP_NS          \n"     /*Get back stack value */
    "   CMP R2, R1              \n"     /*Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_MSP_NS   \n"
    "   LDR R1, =0x55555554     \n"
    "   MSR MSP_NS, R1          \n"     /* load stack value */
    "   MRS R2, MSP_NS          \n"     /* Get back stack value */
    "   CMP R2, R1              \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_MSP_NS   \n"
    "   MSR MSP_NS, R0          \n"     /* Restore stack value */
    "   MSR CONTROL_NS, R3      \n"     /* Restore Control value */
    "   B IEC60730_CPU_REGS_MSP_NS_PASS_ASM_BKPT   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_FAIL_ENDLESS_LOOP_MSP_NS:   \n"
#else
    "IEC60730_FAIL_ENDLESS_LOOP_MSP_NS::  \n"
#endif
    "   MSR MSP_NS, R0          \n"     /* Restore stack value */
    "   MSR CONTROL_NS, R3      \n"     /* Restore Control value */
    "   MOVS %0, #0x0           \n"     /* result equal SL_IEC60730_TEST_FAILED*/
    "   B IEC60730_CPU_REGS_MSP_NS_DONE    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSP_NS_PASS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_MSP_NS_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1           \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i                 \n"     /*Enable IRQ Interrupts*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSP_NS_DONE:   \n"
#else
    "IEC60730_CPU_REGS_MSP_NS_DONE::  \n"
#endif
    : "=r" (result)::"memory");         /* set output variable result*/

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register PSP Non-Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the PSP_NS (Process Stack Pointer, Non-Secure) register.
 * If PSP_NS is corrupted, the function stucks in an endless loop with
 * interrupts disabled. This state must be observed by another safety mechanism
 * (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555554
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_psp_ns(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i               \n"     /*Disable IRQ Interrupts*/
    "   MRS R3, CONTROL_NS    \n"     /* Save Control value */
    "   MRS R0, PSP_NS        \n"     /* Save process stack value */
    "   LDR R1, =0xAAAAAAA8   \n"
    "   MSR PSP_NS, R1        \n"     /* load process stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSP_NS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_PSP_NS_ASM_BKPT::  \n"
#endif
    "   MRS R2, PSP_NS        \n"     /* Get back process stack value */
    "   CMP R2, R1            \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_PSP_NS   \n"
    "   LDR R1, =0x55555554   \n"
    "   MSR PSP_NS, R1        \n"     /* load process stack value */
    "   MRS R2, PSP_NS        \n"     /* Get back process stack value */
    "   CMP R2, R1            \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_PSP_NS   \n"
    "   MSR PSP_NS, R0        \n"     /* Restore process stack value */
    "   MSR CONTROL_NS, R3    \n"     /* Restore Control value */
    "   B IEC60730_CPU_REGS_PSP_NS_PASS_ASM_BKPT   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_FAIL_ENDLESS_LOOP_PSP_NS:   \n"
#else
    "IEC60730_FAIL_ENDLESS_LOOP_PSP_NS::  \n"
#endif
    "   MSR MSP_NS, R0        \n"     /* Restore stack value */
    "   MSR CONTROL_NS, R3    \n"     /* Restore Control value */
    "   MOVS %0, #0x0         \n"     /* result equal SL_IEC60730_TEST_FAILED*/
    "   B IEC60730_CPU_REGS_PSP_NS_DONE    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSP_NS_PASS_ASM_BKPT: \n"
#else
    "IEC60730_CPU_REGS_PSP_NS_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1         \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i               \n"     /*Enable IRQ Interrupts*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSP_NS_DONE:   \n"
#else
    "IEC60730_CPU_REGS_PSP_NS_DONE::  \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register MSPLIM Non-Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the MSPLIM_NS (Main Stack Pointer Limit, Non-Secure)
 * register. If MSPLIM_NS is corrupted, the function stucks in an endless loop
 * with interrupts disabled. This state must be observed by another safety
 * mechanism (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555550
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_msplim_ns(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i                 \n"     /*Disable IRQ Interrupts*/
    "   MRS R3, CONTROL_NS      \n"     /* Save Control value */
    "   MRS R0, MSPLIM_NS       \n"     /* Save stack value */
    "   LDR R1, =0xAAAAAAA8     \n"
    "   MSR MSPLIM_NS, R1       \n"     /* load stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSPLIM_NS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_MSPLIM_NS_ASM_BKPT:: \n"
#endif
    "   MRS R2, MSPLIM_NS       \n"     /* Get back stack value */
    "   CMP R2, R1              \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_NS   \n"
    "   LDR R1, =0x55555550     \n"
    "   MSR MSPLIM_NS, R1       \n"     /* load stack value */
    "   MRS R2, MSPLIM_NS       \n"     /* Get back stack value */
    "   CMP R2, R1              \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_NS   \n"
    "   MSR MSPLIM_NS, R0       \n"     /* Restore stack value */
    "   MSR CONTROL_NS, R3      \n"     /* Restore Control value */
    "   B IEC60730_CPU_REGS_MSPLIM_NS_PASS_ASM_BKPT   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_NS: \n"
#else
    "IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_NS:: \n"
#endif
    "   MSR MSPLIM, R0          \n"     /* Restore stack value */
    "   MSR CONTROL, R3         \n"     /* Restore Control value */
    "   MOVS %0, #0x0           \n"     /* result equal SL_IEC60730_TEST_FAILED*/
    "   B IEC60730_CPU_REGS_MSPLIM_NS_DONE    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSPLIM_NS_PASS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_MSPLIM_NS_PASS_ASM_BKPT::  \n"
#endif
    "   MOVS %0, #0x1           \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i                 \n"     /*Enable IRQ Interrupts*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSPLIM_NS_DONE:  \n"
#else
    "IEC60730_CPU_REGS_MSPLIM_NS_DONE:: \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register PSPLIM Non-Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the PSPLIM_NS (Process Stack Pointer Limit, Non-Secure)
 * register. If PSPLIM_NS is corrupted, the function stucks in an endless loop
 * with interrupts disabled. This state must be observed by another safety
 * mechanism (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555550
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_psplim_ns(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i                   \n"     /*Disable IRQ Interrupts*/
    "   MRS R3, CONTROL_NS        \n"     /* Save Control value */
    "   MRS R0, PSPLIM_NS         \n"     /* Save stack value */
    "   LDR R1, =0xAAAAAAA8       \n"
    "   MSR PSPLIM_NS, R1         \n"     /* load stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSPLIM_NS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_PSPLIM_NS_ASM_BKPT:: \n"
#endif
    "   MRS R2, PSPLIM_NS         \n"     /* Get back process stack value */
    "   CMP R2, R1                \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_NS   \n"
    "   LDR R1, =0x55555550       \n"
    "   MSR PSPLIM_NS, R1         \n"     /* load stack value */
    "   MRS R2, PSPLIM_NS         \n"     /* Get back stack value */
    "   CMP R2, R1                \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_NS   \n"
    "   MSR PSPLIM_NS, R0         \n"     /* Restore stack value */
    "   MSR CONTROL_NS, R3        \n"     /* Restore Control value */
    "   B IEC60730_CPU_REGS_PSPLIM_NS_PASS_ASM_BKPT   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_NS:  \n"
#else
    "IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_NS:: \n"
#endif
    "   MSR PSPLIM_NS, R0         \n"     /* Restore stack value */
    "   MSR CONTROL, R3           \n"     /* Restore Control value */
    "   MOVS %0, #0x0             \n"     /* result equal SL_IEC60730_TEST_FAILED*/
    "   B IEC60730_CPU_REGS_PSPLIM_NS_DONE    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSPLIM_NS_PASS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_PSPLIM_NS_PASS_ASM_BKPT::  \n"
#endif
    "   MOVS %0, #0x1              \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i                    \n"     /*Enable IRQ Interrupts*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSPLIM_NS_DONE:  \n"
#else
    "IEC60730_CPU_REGS_PSPLIM_NS_DONE:: \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register Control Non-Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the Control_NS (Control, Non-Secure) register.
 * Test patterns: 0x00000002, 0x00000004
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_control_ns(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i                      \n"      /*Disable IRQ Interrupts*/
    "   MRS R0, CONTROL_NS           \n"
    "   MOVS R1, #0x2                \n"
    "   MSR CONTROL_NS, R1           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_NS_SPSEL_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_CONTROL_NS_SPSEL_ASM_BKPT::  \n"
#endif
    "   MRS R2, CONTROL_NS            \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_CONTROL_NS_FAIL   \n"
    "   MOVW R1, #0x4                 \n"
    "   MSR CONTROL_NS, R1            \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_NS_FPCA_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CONTROL_NS_FPCA_ASM_BKPT:: \n"
#endif
    "   MRS R2, CONTROL_NS            \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_CONTROL_NS_FAIL   \n"
    "   MSR CONTROL_NS, R0            \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_NS_PASS_ASM_BKPT: \n"
#else
    "IEC60730_CPU_REGS_CONTROL_NS_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1                 \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i                       \n"     /*Enable IRQ Interrupts*/
    "   B IEC60730_CPU_REGS_CONTROL_NS_DONE   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_NS_FAIL:   \n"
#else
    "IEC60730_CPU_REGS_CONTROL_NS_FAIL::  \n"
#endif
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_NS_FAIL_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CONTROL_NS_FAIL_ASM_BKPT:: \n"
#endif
    "   MSR CONTROL_NS, R0            \n"
    "   MOVS %0, #0x0                 \n"     /* result equal SL_IEC60730_TEST_FAILED*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_NS_DONE:   \n"
#else
    "IEC60730_CPU_REGS_CONTROL_NS_DONE::  \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register PriMask Non-Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the PriMask_NS (PriMask, Non-Secure) register.
 * Test patterns: 0x00000001, 0x00000000
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_primask_ns(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i                       \n"     /*Disable IRQ Interrupts*/
    "   MRS R0, PRIMASK_NS            \n"
    "   MOVW R1, #0x0001              \n"
    "   MSR PRIMASK_NS, R1            \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PRIMASK_NS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_PRIMASK_NS_ASM_BKPT::  \n"
#endif
    "   MRS R2, PRIMASK_NS            \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_PRIMASK_NS_FAIL   \n"
    "   MOVW R1, #0x0000              \n"
    "   MSR PRIMASK_NS, R1            \n"
    "   MRS R2, PRIMASK_NS            \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_PRIMASK_NS_FAIL   \n"
    "   MSR PRIMASK_NS, R0            \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PRIMASK_NS_PASS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_PRIMASK_NS_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1                 \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i                       \n"     /*Enable IRQ Interrupts*/
    "   B IEC60730_CPU_REGS_PRIMASK_NS_DONE   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PRIMASK_NS_FAIL:   \n"
#else
    "IEC60730_CPU_REGS_PRIMASK_NS_FAIL::  \n"
#endif
    "   MSR PRIMASK_NS, R0            \n"
    "   MOVS %0, #0x0                 \n"     /* result equal SL_IEC60730_TEST_FAILED*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PRIMASK_NS_DONE: \n"
#else
    "IEC60730_CPU_REGS_PRIMASK_NS_DONE: \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register BasePri Non-Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the BASEPRI_NS (BasePri, Non-Secure) register.
 * Test patterns: 0x000000A0, 0x00000040
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_basepri_ns(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i                         \n"     /*Disable IRQ Interrupts*/
    "   MRS R0, BASEPRI_NS              \n"
    "   MOV R1, #0xA0                   \n"
    "   MSR BASEPRI_NS, R1              \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_BASEPRI_NS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_BASEPRI_NS_ASM_BKPT::  \n"
#endif
    "   MRS R2, BASEPRI_NS              \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_BASEPRI_NS_FAIL   \n"
    "   MOV R1, #0x40                   \n"
    "   MSR BASEPRI_NS, R1              \n"
    "   MRS R2, BASEPRI_NS              \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_BASEPRI_NS_FAIL   \n"
    "   MSR BASEPRI_NS, R0              \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_BASEPRI_NS_PASS_ASM_BKPT: \n"
#else
    "IEC60730_CPU_REGS_BASEPRI_NS_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1                   \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i                         \n"     /*Enable IRQ Interrupts*/
    "   B IEC60730_CPU_REGS_BASEPRI_NS_DONE   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_BASEPRI_NS_FAIL:   \n"
#else
    "IEC60730_CPU_REGS_BASEPRI_NS_FAIL::  \n"
#endif
    "   MSR BASEPRI_NS, R0              \n"
    "   MOVS %0, #0x0                   \n"     /* result equal SL_IEC60730_TEST_FAILED*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_BASEPRI_NS_DONE:   \n"
#else
    "IEC60730_CPU_REGS_BASEPRI_NS_DONE::  \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register FaultMask Non-Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the FAULTMASK_NS (FaultMask, Non-Secure) register.
 * Test patterns: 0x00000001, 0x00000000
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_faultmask_ns(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i                       \n"     /*Disable IRQ Interrupts*/
    "   MRS R0, FAULTMASK_NS          \n"
    "   MOV R1, #1                    \n"
    "   MSR FAULTMASK_NS, R1          \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FAULTMASK_NS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FAULTMASK_NS_ASM_BKPT::  \n"
#endif
    "   MRS R2, FAULTMASK_NS          \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FAULTMASK_NS_FAIL   \n"
    "   MOV R1, #0                    \n"
    "   MSR FAULTMASK_NS, R1          \n"
    "   MRS R2, FAULTMASK_NS          \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FAULTMASK_NS_FAIL   \n"
    "   MSR FAULTMASK_NS, R0          \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FAULTMASK_NS_PASS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FAULTMASK_NS_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1                 \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i                       \n"     /*Enable IRQ Interrupts*/
    "   B IEC60730_CPU_REGS_FAULTMASK_NS_DONE   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FAULTMASK_NS_FAIL:   \n"
#else
    "IEC60730_CPU_REGS_FAULTMASK_NS_FAIL::  \n"
#endif
    "   MSR FAULTMASK_NS, R0          \n"
    "   MOVS %0, #0x0                 \n"     /* result equal SL_IEC60730_TEST_FAILED*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FAULTMASK_NS_DONE:   \n"
#else
    "IEC60730_CPU_REGS_FAULTMASK_NS_DONE::  \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif
