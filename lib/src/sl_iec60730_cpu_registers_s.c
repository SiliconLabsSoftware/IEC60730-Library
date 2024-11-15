/***************************************************************************/ /**
 * @file    sl_iec60730_cpu_registers_s.c
 * @brief   CPU Registers check secure
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
extern sl_iec60730_test_result_t unit_test_iec60730_cpu_registers_bist_s_mock(void);
#endif // UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE

#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_msp_s(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_psp_s(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_msplim_s(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_psplim_s(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_control_s(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_primask_s(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_basepri_s(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_faultmask_s(void);
#endif

/**************************************************************************/ /**
 * public IEC60730 CPU Registers Built In Self Test
 *
 * @returns #sl_iec60730_test_result_t
 * @returns #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function will do a test of all the CPU core registers secure
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_bist_s(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;

#if (IEC60370_CPU == IEC60370_CM33)
#if defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)

#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Test Main Stack Pointer Secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_msp_s()) {
    goto CPU_REGISTERS_BIST_S_DONE;
  }

  /* Test Process Stack Pointer Secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_psp_s()) {
    goto CPU_REGISTERS_BIST_S_DONE;
  }

  /* Test Main Stack Pointer Limit Secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_msplim_s()) {
    goto CPU_REGISTERS_BIST_S_DONE;
  }

  /* Test Process Stack Pointer Limit Secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_psplim_s()) {
    goto CPU_REGISTERS_BIST_S_DONE;
  }

  /* Test Control Secure register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_control_s()) {
    goto CPU_REGISTERS_BIST_S_DONE;
  }

  /* Test Primask Secure register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_primask_s()) {
    goto CPU_REGISTERS_BIST_S_DONE;
  }

  /* Test BasePri Secure register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_basepri_s()) {
    goto CPU_REGISTERS_BIST_S_DONE;
  }

  /* Test FaultMask Secure register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_faultmask_s()) {
    goto CPU_REGISTERS_BIST_S_DONE;
  }

#ifdef UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE
  if (SL_IEC60730_TEST_FAILED == unit_test_iec60730_cpu_registers_bist_s_mock()) {
    goto CPU_REGISTERS_BIST_S_DONE;
  }
#endif // UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE

  result = SL_IEC60730_TEST_PASSED;

  CPU_REGISTERS_BIST_S_DONE:

#endif
#else
#error the CM33 without TZ is not supported
#endif // defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
#endif // (IEC60370_CPU == IEC60370_CM33)

  return result;
}

/**************************************************************************/ /**
 * private IEC60730 CPU Register MSP Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the MSPS (Main Stack Pointer, Secure) register.
 * If MSP_S is corrupted, the function stucks in an endless loop with
 * interrupts disabled. This state must be observed by another safety mechanism
 * (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555554
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_msp_s(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i               \n"     /*Disable IRQ Interrupts*/
    "   MRS R3, CONTROL       \n"     /* Save Control value */
    "   MRS R0, MSP           \n"     /* Save stack value */
    "   LDR R1, =0xAAAAAAA8   \n"
    "   MSR MSP, R1           \n"     /* load stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSP_S_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_MSP_S_ASM_BKPT:: \n"
#endif
    "   MRS R2, MSP           \n"     /* Get back stack value */
    "   CMP R2, R1            \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_MSP_S   \n"
    "   LDR R1, =0x55555554   \n"
    "   MSR MSP, R1           \n"     /* load stack value */
    "   MRS R2, MSP           \n"     /* Get back stack value */
    "   CMP R2, R1            \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_MSP_S   \n"
    "   MSR MSP, R0           \n"     /* Restore stack value */
    "   MSR CONTROL, R3       \n"     /* Restore Control value */
    "   B IEC60730_CPU_REGS_MSP_S_PASS_ASM_BKPT   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_FAIL_ENDLESS_LOOP_MSP_S:  \n"
#else
    "IEC60730_FAIL_ENDLESS_LOOP_MSP_S:: \n"
#endif
    "   MSR MSP, R0           \n"     /* Restore stack value */
    "   MSR CONTROL, R3       \n"     /* Restore Control value */
    "   MOVS %0, #0x0         \n"     /* result equal SL_IEC60730_TEST_FAILED*/
    "   B IEC60730_CPU_REGS_MSP_S_DONE    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSP_S_PASS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_MSP_S_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1       \n"       /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i             \n"       /*Enable IRQ Interrupts*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSP_S_DONE:  \n"
#else
    "IEC60730_CPU_REGS_MSP_S_DONE:: \n"
#endif
    : "=r" (result)::"memory");      /* set output variable result*/

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register PSP Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the PSP_S (Process Stack Pointer, Secure) register.
 * If PSP_S is corrupted, the function stucks in an endless loop with
 * interrupts disabled. This state must be observed by another safety mechanism
 * (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555554
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_psp_s(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i             \n"     /*Disable IRQ Interrupts*/
    "   MRS R3, CONTROL     \n"     /* Save Control value */
    "   MRS R0, PSP         \n"     /* Save process stack value */
    "   LDR R1, =0xAAAAAAA8 \n"
    "   MSR PSP, R1         \n"     /* load process stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSP_S_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_PSP_S_ASM_BKPT:: \n"
#endif
    "   MRS R2, PSP         \n"     /* Get back process stack value */
    "   CMP R2, R1          \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_PSP_S   \n"
    "   LDR R1, =0x55555554 \n"
    "   MSR PSP, R1         \n"     /* load process stack value */
    "   MRS R2, PSP         \n"     /* Get back process stack value */
    "   CMP R2, R1          \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_PSP_S   \n"
    "   MSR PSP, R0         \n"     /* Restore process stack value */
    "   MSR CONTROL, R3     \n"     /* Restore Control value */
    "   B IEC60730_CPU_REGS_PSP_S_PASS_ASM_BKPT   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_FAIL_ENDLESS_LOOP_PSP_S:  \n"
#else
    "IEC60730_FAIL_ENDLESS_LOOP_PSP_S:: \n"
#endif
    "   MSR PSP, R0         \n"     /* Restore stack value */
    "   MSR CONTROL, R3     \n"     /* Restore Control value */
    "   MOVS %0, #0x0       \n"     /* result equal SL_IEC60730_TEST_FAILED*/
    "   B IEC60730_CPU_REGS_PSP_S_DONE    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSP_S_PASS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_PSP_S_PASS_ASM_BKPT::  \n"
#endif

    "   MOVS %0, #0x1       \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i             \n"     /*Enable IRQ Interrupts*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSP_S_DONE:  \n"
#else
    "IEC60730_CPU_REGS_PSP_S_DONE:: \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register MSPLIM Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the MSPLIM_S (Main Stack Pointer Limit, Secure)
 * register. If MSPLIM_S is corrupted, the function stucks in an endless loop
 * with interrupts disabled. This state must be observed by another safety
 * mechanism (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555550
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_msplim_s(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i               \n"     /*Disable IRQ Interrupts*/
    "   MRS R3, CONTROL       \n"     /* Save Control value */
    "   MRS R0, MSPLIM        \n"     /* Save stack value */
    "   LDR R1, =0xAAAAAAA8   \n"
    "   MSR MSPLIM, R1        \n"     /* load stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSPLIM_S_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_MSPLIM_S_ASM_BKPT::  \n"
#endif
    "   MRS R2, MSPLIM        \n"     /* Get back stack value */
    "   CMP R2, R1            \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_S     \n"
    "   LDR R1, =0x55555550   \n"
    "   MSR MSPLIM, R1        \n"     /* load stack value */
    "   MRS R2, MSPLIM        \n"     /* Get back stack value */
    "   CMP R2, R1            \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_S     \n"
    "   MSR MSPLIM, R0        \n"     /* Restore stack value */
    "   MSR CONTROL, R3       \n"     /* Restore Control value */
    "   B IEC60730_CPU_REGS_MSPLIM_S_PASS_ASM_BKPT  \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_S:   \n"
#else
    "IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_S::  \n"
#endif
    "   MSR MSPLIM, R0        \n"     /* Restore stack value */
    "   MSR CONTROL, R3       \n"     /* Restore Control value */
    "   MOVS %0, #0x0         \n"     /* result equal SL_IEC60730_TEST_FAILED*/
    "   B IEC60730_CPU_REGS_MSPLIM_S_DONE    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSPLIM_S_PASS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_MSPLIM_S_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1         \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i               \n"     /*Enable IRQ Interrupts*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_MSPLIM_S_DONE:  \n"
#else
    "IEC60730_CPU_REGS_MSPLIM_S_DONE:: \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register PSPLIM Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the PSPLIM_S (Process Stack Pointer Limit, Secure)
 * register. If PSPLIM_S is corrupted, the function stucks in an endless loop
 * with interrupts disabled. This state must be observed by another safety
 * mechanism (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555550
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_psplim_s(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i               \n"     /*Disable IRQ Interrupts*/
    "   MRS R3, CONTROL       \n"     /* Save Control value */
    "   MRS R0, PSPLIM        \n"     /* Save process stack value */
    "   LDR R1, =0xAAAAAAA8   \n"
    "   MSR PSPLIM, R1        \n"     /* load process stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSPLIM_S_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_PSPLIM_S_ASM_BKPT::  \n"
#endif
    "   MRS R2, PSPLIM        \n"     /* Get back process stack value */
    "   CMP R2, R1            \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_S   \n"
    "   LDR R1, =0x55555550   \n"
    "   MSR PSPLIM, R1        \n"     /* load process stack value */
    "   MRS R2, PSPLIM        \n"     /* Get back process stack value */
    "   CMP R2, R1            \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_S   \n"
    "   MSR PSPLIM, R0        \n"     /* Restore process stack value */
    "   MSR CONTROL, R3       \n"     /* Restore Control value */
    "   B IEC60730_CPU_REGS_PSPLIM_S_PASS_ASM_BKPT   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_S:   \n"
#else
    "IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_S::  \n"
#endif
    "   MSR PSPLIM, R0        \n"     /* Restore stack value */
    "   MSR CONTROL, R3       \n"     /* Restore Control value */
    "   MOVS %0, #0x0         \n"     /* result equal SL_IEC60730_TEST_FAILED*/
    "   B IEC60730_CPU_REGS_PSPLIM_S_DONE    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSPLIM_S_PASS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_PSPLIM_S_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1         \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i               \n"     /*Enable IRQ Interrupts*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PSPLIM_S_DONE:   \n"
#else
    "IEC60730_CPU_REGS_PSPLIM_S_DONE::  \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register Control Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the Control_S (Control, Secure) register.
 * Test patterns: 0x0000000A, 0x00000004
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_control_s(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i              \n"     /*Disable IRQ Interrupts*/
    "   MRS R0, CONTROL     \n"
    "   MOVW R1, #0x000A    \n"
    "   MSR CONTROL, R1     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_S_SPSEL_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CONTROL_S_SPSEL_ASM_BKPT:: \n"
#endif
    "   MRS R2, CONTROL     \n"
    "   CMP R2, R1          \n"
    "   BNE IEC60730_CPU_REGS_CONTROL_S_FAIL   \n"
    "   MOVW R1, #0x4       \n"
    "   MSR CONTROL, R1     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_S_FPCA_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_CONTROL_S_FPCA_ASM_BKPT::  \n"
#endif
    "   MRS R2, CONTROL     \n"
    "   CMP R2, R1          \n"
    "   BNE IEC60730_CPU_REGS_CONTROL_S_FAIL   \n"
    "   MSR CONTROL, R0     \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_S_PASS_ASM_BKPT: \n"
#else
    "IEC60730_CPU_REGS_CONTROL_S_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1       \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i             \n"     /*Enable IRQ Interrupts*/
    "   B IEC60730_CPU_REGS_CONTROL_S_DONE    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_S_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_CONTROL_S_FAIL:: \n"
#endif
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_S_FAIL_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_CONTROL_S_FAIL_ASM_BKPT::  \n"
#endif
    "   MSR CONTROL, R0     \n"
    "   MOVS %0, #0x0       \n"     /* result equal SL_IEC60730_TEST_FAILED*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_S_DONE:  \n"
#else
    "IEC60730_CPU_REGS_CONTROL_S_DONE:: \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register PriMask Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the PriMask_S (PriMask, Secure) register.
 * Test patterns: 0x00000001, 0x00000000
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_primask_s(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i             \n"     /*Disable IRQ Interrupts*/
    "   MRS R0, PRIMASK     \n"
    "   MOVW R1, #0x0001    \n"
    "   MSR PRIMASK, R1     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PRIMASK_S_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_PRIMASK_S_ASM_BKPT:: \n"
#endif
    "   MRS R2, PRIMASK     \n"
    "   CMP R2, R1          \n"
    "   BNE IEC60730_CPU_REGS_PRIMASK_S_FAIL   \n"
    "   MOVW R1, #0x0000    \n"
    "   MSR PRIMASK, R1     \n"
    "   MRS R2, PRIMASK     \n"
    "   CMP R2, R1          \n"
    "   BNE IEC60730_CPU_REGS_PRIMASK_S_FAIL   \n"
    "   MSR PRIMASK, R0     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PRIMASK_S_PASS_ASM_BKPT: \n"
#else
    "IEC60730_CPU_REGS_PRIMASK_S_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1      \n"      /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i            \n"      /*Enable IRQ Interrupts*/
    "   B IEC60730_CPU_REGS_PRIMASK_S_DONE    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PRIMASK_S_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_PRIMASK_S_FAIL:: \n"
#endif
    "   MSR PRIMASK, R0     \n"
    "   MOVS %0, #0x0       \n"     /* result equal SL_IEC60730_TEST_FAILED*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_PRIMASK_S_DONE:  \n"
#else
    "IEC60730_CPU_REGS_PRIMASK_S_DONE:: \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private IEC60730 CPU Register BasePri Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the BASEPRI (BasePri, Secure) register.
 * Test patterns: 0x000000A0, 0x00000040
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_basepri_s(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i             \n"     /*Disable IRQ Interrupts*/
    "   MRS R0, BASEPRI     \n"
    "   MOV R1, #0xA0       \n"
    "   MSR BASEPRI, R1     \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_BASEPRI_S_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_BASEPRI_S_ASM_BKPT:: \n"
#endif
    "   MRS R2, BASEPRI     \n"
    "   CMP R2, R1          \n"
    "   BNE IEC60730_CPU_REGS_BASEPRI_S_FAIL   \n"
    "   MOV R1, #0x40       \n"
    "   MSR BASEPRI, R1     \n"
    "   MRS R2, BASEPRI     \n"
    "   CMP R2, R1          \n"
    "   BNE IEC60730_CPU_REGS_BASEPRI_S_FAIL   \n"
    "   MSR BASEPRI, R0     \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_BASEPRI_S_PASS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_BASEPRI_S_PASS_ASM_BKPT::  \n"
#endif
    "   MOVS %0, #0x1       \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i             \n"     /*Enable IRQ Interrupts*/
    "   B IEC60730_CPU_REGS_BASEPRI_S_DONE    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_BASEPRI_S_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_BASEPRI_S_FAIL:: \n"
#endif
    "   MSR BASEPRI, R0     \n"
    "   MOVS %0, #0x0       \n"     /* result equal SL_IEC60730_TEST_FAILED*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_BASEPRI_S_DONE:  \n"
#else
    "IEC60730_CPU_REGS_BASEPRI_S_DONE:: \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif

/**************************************************************************/ /**
 * private  IEC60730 CPU Register FaultMask Secure Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the FAULTMASK (FaultMask, Secure) register.
 * Test patterns: 0x00000001, 0x00000000
 * @warning This function cannot be interrupted. The core must be in the
 * secure state.
 *****************************************************************************/
#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_faultmask_s(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i               \n"     /*Disable IRQ Interrupts*/
    "   MRS R0, FAULTMASK     \n"
    "   MOV R1, #1            \n"
    "   MSR FAULTMASK, R1     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FAULTMASK_S_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FAULTMASK_S_ASM_BKPT:: \n"
#endif
    "   MRS R2, FAULTMASK     \n"
    "   CMP R2, R1            \n"
    "   BNE IEC60730_CPU_REGS_FAULTMASK_S_FAIL   \n"
    "   MOV R1, #0            \n"
    "   MSR FAULTMASK, R1     \n"
    "   MRS R2, FAULTMASK     \n"
    "   CMP R2, R1            \n"
    "   BNE IEC60730_CPU_REGS_FAULTMASK_S_FAIL   \n"
    "   MSR FAULTMASK, R0     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FAULTMASK_S_PASS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FAULTMASK_S_PASS_ASM_BKPT::  \n"
#endif
    "   MOVS %0, #0x1         \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   CPSIE i               \n"     /*Enable IRQ Interrupts*/
    "   B IEC60730_CPU_REGS_FAULTMASK_S_DONE    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FAULTMASK_S_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_FAULTMASK_S_FAIL:: \n"
#endif
    "   MSR FAULTMASK, R0     \n"
    "   MOVS %0, #0x0         \n"     /* result equal SL_IEC60730_TEST_FAILED*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FAULTMASK_S_DONE:  \n"
#else
    "IEC60730_CPU_REGS_FAULTMASK_S_DONE:: \n"
#endif
    : "=r" (result)::"memory");

  return result;
}
#endif
