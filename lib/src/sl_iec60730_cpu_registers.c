/***************************************************************************/ /**
 * @file    sl_iec60730_cpu_registers.c
 * @brief CPU Registers check
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
extern sl_iec60730_test_result_t unit_test_iec60730_cpu_registers_bist_mock(void);
#endif // UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE

static sl_iec60730_test_result_t sl_iec60730_cpu_registers_core(void);

#if (IEC60370_CPU == IEC60370_CM4)
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_msp(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_psp(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_control(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_primask(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_basepri(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_faultmask(void);
#endif //(IEC60370_CPU == IEC60370_CM4)

#if ((defined(__FPU_PRESENT) && (__FPU_PRESENT == 1U)) \
  && (defined(__FPU_USED) && (__FPU_USED == 1U)))
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_fpu_fpscr(void);
static sl_iec60730_test_result_t sl_iec60730_cpu_registers_fpu_fpsx(void);
#endif // ((defined (__FPU_PRESENT) && (__FPU_USED == 1U))

/**************************************************************************/ /**
 * public IEC60730 CPU Registers Built In Self Test
 *
 * @returns #sl_iec60730_test_result_t
 * @returns #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function will do a test of all the CPU core registers.
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_bist(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  /* Test core register: APSR, R0-R12, LR */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_core()) {
    goto CPU_REGISTERS_BIST_DONE;
  }

#if (IEC60370_CPU == IEC60370_CM33)
#if defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)

#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Test cpu register non-secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_bist_ns()) {
    goto CPU_REGISTERS_BIST_DONE;
  }
  /* Test cpu register secure */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_bist_s()) {
    goto CPU_REGISTERS_BIST_DONE;
  }

#endif
#else
#error the CM33 without TZ is not supported
#endif // defined(__TZ_PRESENT) && (__TZ_PRESENT == 1)
#endif // (IEC60370_CPU == IEC60370_CM33)

#if (IEC60370_CPU == IEC60370_CM4)
  /* Test Main Stack Pointer */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_msp()) {
    goto CPU_REGISTERS_BIST_DONE;
  }

  /* Test Process Stack Pointer */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_psp()) {
    goto CPU_REGISTERS_BIST_DONE;
  }

  /* Test Control register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_control()) {
    goto CPU_REGISTERS_BIST_DONE;
  }

  /* Test Primask register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_primask()) {
    goto CPU_REGISTERS_BIST_DONE;
  }

  /* Test BasePri register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_basepri()) {
    goto CPU_REGISTERS_BIST_DONE;
  }

  /* Test FaultMask register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_faultmask()) {
    goto CPU_REGISTERS_BIST_DONE;
  }
#endif

  /* Test FPU FPSCR register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_fpu_fpscr()) {
    goto CPU_REGISTERS_BIST_DONE;
  }

  /* Test FPU So-S31 register */
  if (SL_IEC60730_TEST_FAILED == sl_iec60730_cpu_registers_fpu_fpsx()) {
    goto CPU_REGISTERS_BIST_DONE;
  }

#ifdef UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE
  if (SL_IEC60730_TEST_FAILED == unit_test_iec60730_cpu_registers_bist_mock()) {
    goto CPU_REGISTERS_BIST_DONE;
  }
#endif // UNIT_TEST_IEC60730_CPU_REGISTERS_ENABLE

  result = SL_IEC60730_TEST_PASSED;
  sl_iec60730_program_counter_check |= (IEC60730_CPU_REGS_COMPLETE);

  CPU_REGISTERS_BIST_DONE:

  return result;
}

/**************************************************************************/ /**
 * public IEC60730 CPU Registers Power On Self Test
 *
 * @returns #sl_iec60730_test_result_t
 *
 * This function simply calls the sl_iec60730_cpu_registers_bist() function.
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_post(void)
{
  return sl_iec60730_cpu_registers_bist();
}

/**************************************************************************/ /**
 * private IEC60730 CPU Registers Core Check
 *
 * @returns #sl_iec60730_test_result_t
 * @returns #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the following CPU registers in a sequence: APSR, R0-R12,
 * LR
 * Test patterns: R0�R12, LR: 0xAAAAAAAA, 0x55555555
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_core(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i                       \n"     /*Disable IRQ Interrupts*/
    /* Safe critical registers. According calling convention. */
    "STMDB SP!, {R4, R5, R6, R7, R8, R9, R10, R11, R14}   \n"

    /* Register APSR */
    "MOVS R0, #0x0000                 \n"     /* Set Z(ero) Flag */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_Z_FLAG_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_Z_FLAG_ASM_BKPT:: \n"
#endif
    "   BNE.W IEC60730_FAIL_ENDLESS_LOOP_CORE     \n"     /* Fails if Z clear */
    "   SUBS R0,#1                    \n"     /* Set N(egative) Flag */
#ifndef IAR_TESTING  /* GCC */
    "IEC60730_CPU_REGS_CORE_N_FLAG_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_N_FLAG_ASM_BKPT:: \n"
#endif
    "   BPL.W IEC60730_FAIL_ENDLESS_LOOP_CORE     \n"     /* Fails if N clear */
    "   ADDS R0,#2                    \n"     /* Set C(array) Flag and do not set Z*/
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_C_FLAG_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_C_FLAG_ASM_BKPT:: \n"
#endif
    "   BCC.W IEC60730_FAIL_ENDLESS_LOOP_CORE   \n"     /* Fails if C clear */
    "   LDR R0, =0x80000000           \n"               /* Prepares Overflow test */
    "   ADDS R0, R0, R0               \n"               /* Set V(overflow) Flag */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_V_FLAG_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_V_FLAG_ASM_BKPT:: \n"
#endif
    "   BVC.W IEC60730_FAIL_ENDLESS_LOOP_CORE   \n"     /* Fails if V clear */
    "   LDR R0, =0xFFFFFFFF           \n"     /* Prepares Saturation test */
    "   USAT R1,#10,R0                \n"     /* Set Q(saturation) Flag */
    "   MRS R0, APSR                  \n"     /* Get Status register */
    "   CMP R0, #0x78000000           \n"     /* Verifies that Z=C=V=Q=1 */
    "   BNE.W IEC60730_FAIL_ENDLESS_LOOP_CORE   \n"     /* Fails if Z=C=V=Q clear */
    /* Register R0 (holds value returned by the function) */
    "   LDR R0, =0xAAAAAAAA           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R0_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R0_ASM_BKPT:: \n"
#endif
    "   CMP R0, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_FAIL_ENDLESS_LOOP_CORE   \n"
    "   LDR R0, =0x55555555           \n"
    "   CMP R0, #0x55555555           \n"
    "   BNE.W IEC60730_FAIL_ENDLESS_LOOP_CORE   \n"

    /* Register R1 */
    "   LDR.W R1, =0xAAAAAAAA         \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R1_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R1_ASM_BKPT:: \n"
#endif
    "   CMP R1, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_FAIL_ENDLESS_LOOP_CORE   \n"
    "   LDR.W R1, =0x55555555         \n"
    "   CMP R1, #0x55555555           \n"
    "   BNE.W IEC60730_FAIL_ENDLESS_LOOP_CORE   \n"

    /* Register R2 */
    "   LDR R2, =0xAAAAAAAA           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R2_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R2_ASM_BKPT:: \n"
#endif
    "   CMP R2, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R2, =0x55555555           \n"
    "   CMP R2, #0x55555555           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R3 */
    "   LDR R3, =0xAAAAAAAA           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R3_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R3_ASM_BKPT:: \n"
#endif
    "   CMP R3, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R3, =0x55555555           \n"
    "   CMP R3, #0x55555555           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R4 */
    "   LDR R4, =0xAAAAAAAA           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R4_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R4_ASM_BKPT:: \n"
#endif
    "   CMP R4, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R4, =0x55555555           \n"
    "   CMP R4, #0x55555555           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R5 */
    "   LDR R5, =0xAAAAAAAA           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R5_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R5_ASM_BKPT:: \n"
#endif
    "   CMP R5, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R5, =0x55555555           \n"
    "   CMP R5, #0x55555555           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R6 */
    "   LDR R6, =0xAAAAAAAA           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R6_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R6_ASM_BKPT:: \n"
#endif
    "   CMP R6, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R6, =0x55555555           \n"
    "   CMP R6, #0x55555555           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R7 */
    "   LDR R7, =0xAAAAAAAA           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R7_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R7_ASM_BKPT:: \n"
#endif
    "   CMP R7, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R7, =0x55555555           \n"
    "   CMP R7, #0x55555555           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R8 */
    "   LDR R8, =0xAAAAAAAA           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R8_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R8_ASM_BKPT:: \n"
#endif
    "   CMP R8, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R8, =0x55555555           \n"
    "   CMP R8, #0x55555555           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R9 */
    "   LDR R9, =0xAAAAAAAA           \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R9_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_R9_ASM_BKPT:: \n"
#endif
    "   CMP R9, #0xAAAAAAAA           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R9, =0x55555555           \n"
    "   CMP R9, #0x55555555           \n"
    "   BNE.W IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R10 */
    "   LDR R10, =0xAAAAAAAA          \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R10_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_CORE_R10_ASM_BKPT::  \n"
#endif
    "   CMP R10, #0xAAAAAAAA          \n"
    "   BNE IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R10, =0x55555555          \n"
    "   CMP R10, #0x55555555          \n"
    "   BNE IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R11 */
    "   LDR R11, =0xAAAAAAAA          \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R11_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_CORE_R11_ASM_BKPT::  \n"
#endif
    "   CMP R11, #0xAAAAAAAA          \n"
    "   BNE IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R11, =0x55555555          \n"
    "   CMP R11, #0x55555555          \n"
    "   BNE IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register R12 */
    "   LDR R12, =0xAAAAAAAA          \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_R12_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_CORE_R12_ASM_BKPT::  \n"
#endif
    "   CMP R12, #0xAAAAAAAA          \n"
    "   BNE IEC60730_CPU_REGS_CORE_FAIL   \n"
    "   LDR R12, =0x55555555          \n"
    "   CMP R12, #0x55555555          \n"
    "   BNE IEC60730_CPU_REGS_CORE_FAIL   \n"

    /* Register LR (R14) */
    "   LDR.W R1, =0xAAAAAAAA         \n"
    "   MOV LR, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_LR_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_LR_ASM_BKPT:: \n"
#endif
    "   MOV R2, LR                    \n"
    "   CMP R2, R1                    \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_CORE   \n"
    "   LDR.W R1, =0x55555555         \n"
    "   MOV LR, R1                    \n"
    "   MOV R2, LR                    \n"
    "   CMP R2, R1                    \n"     /* Verify value */
    "   BNE IEC60730_FAIL_ENDLESS_LOOP_CORE     \n"
    "   B IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT  \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_FAIL_ENDLESS_LOOP_CORE:   \n"
#else
    "IEC60730_FAIL_ENDLESS_LOOP_CORE::  \n"
#endif
    "   B IEC60730_CPU_REGS_CORE_FAIL       \n"

/*Test passed, return 1 instead of 0 in global variable*/
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1                   \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   B IEC60730_CPU_REGS_CORE_DONE   \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_FAIL:       \n"
#else
    "IEC60730_CPU_REGS_CORE_FAIL::      \n"
#endif
    "   MOVS %0, #0x0                   \n"     /* result equal SL_IEC60730_TEST_FAILED*/

/* Restore critical registers */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CORE_DONE:   \n"
#else
    "IEC60730_CPU_REGS_CORE_DONE::  \n"
#endif
    "   LDMIA SP!, {R4, R5, R6, R7, R8, R9, R10, R11,R14}   \n"
    : "=r" (result)::"memory");     /* set output variable result*/

  return result;
}

/**************************************************************************/ /**
 * private IEC60730 CPU Register MSP Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the MSP (Main Stack Pointer) register.
 * If MSP is corrupted, the function stucks in an endless loop with
 * interrupts disabled. This state must be observed by another safety mechanism
 * (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555554
 * @warning This function cannot be interrupted.
 *****************************************************************************/
#if (IEC60370_CPU == IEC60370_CM4)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_msp(void)
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
 * private IEC60730 CPU Register PSP Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the PSP (Process Stack Pointer) register.
 * If PSP is corrupted, the function stucks in an endless loop with
 * interrupts disabled. This state must be observed by another safety mechanism
 * (for example, watchdog).
 * Test patterns: 0xAAAAAAA8, 0x55555554
 * @warning This function cannot be interrupted.
 *****************************************************************************/
#if (IEC60370_CPU == IEC60370_CM4)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_psp(void)
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
 * private IEC60730 CPU Register Control Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the Control register.
 * Test patterns: 0x00000002, 0x00000004
 * @warning This function cannot be interrupted.
 *****************************************************************************/
#if (IEC60370_CPU == IEC60370_CM4)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_control(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

  __asm volatile(
    "   CPSID i             \n"     /*Disable IRQ Interrupts*/
    "   MRS R0, CONTROL     \n"
    "   MOVW R1, #0x0002    \n"
    "   MSR CONTROL, R1     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_CONTROL_S_SPSEL_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_CONTROL_S_SPSEL_ASM_BKPT:: \n"
#endif
    "   MRS R2, CONTROL     \n"
    "   CMP R2, R1          \n"
    "   BNE IEC60730_CPU_REGS_CONTROL_S_FAIL   \n"
    "   MOVW R1, #0x0004    \n"
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
 * private IEC60730 CPU Register PriMask Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the PriMask register.
 * Test patterns: 0x00000001, 0x00000000
 * @warning This function cannot be interrupted.
 *****************************************************************************/
#if (IEC60370_CPU == IEC60370_CM4)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_primask(void)
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
 * private IEC60730 CPU Register BasePri Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the BASEPRI register.
 * Test patterns: 0x000000A0, 0x00000040
 * @warning This function cannot be interrupted.
 *****************************************************************************/
#if (IEC60370_CPU == IEC60370_CM4)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_basepri(void)
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
 * private IEC60730 CPU Register FaultMask Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the FAULTMASK register.
 * Test patterns: 0x00000001, 0x00000000
 * @warning This function cannot be interrupted.
 *****************************************************************************/
#if (IEC60370_CPU == IEC60370_CM4)
sl_iec60730_test_result_t sl_iec60730_cpu_registers_faultmask(void)
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

#if ((defined(__FPU_PRESENT) && (__FPU_PRESENT == 1U)) \
  && (defined(__FPU_USED) && (__FPU_USED == 1U)))
/**************************************************************************/ /**
 * private IEC60730 FPU Register FPSCR Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the FPSCR (Floating-point Status and Control) register.
 * Test patterns: 0x55400015, 0xA280008A
 * @warning The core must be in the secure state.Only for devices with the
 * Floating Point Unit (FPU).
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_fpu_fpscr(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;

#if ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                     \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  __asm volatile(
    "   CPSID i                     \n"     /*Disable IRQ Interrupts*/
    "   STMDB SP!, {R4, R5, R6}     \n"
    "   LDR R0, =0xE000ED88         \n"
    "   LDR R5, [R0, #0]            \n"     /* Save CPACR to R5 */
    "   LDR R0, =0xE000EF34         \n"
    "   LDR R6, [R0, #0]            \n"     /* Save FPCCR to R6 */
    "   VMRS R4, FPSCR              \n"     /* Save FPSCR register to R4 */
    "   MOV R1, R6                  \n"     /* Copy FPCCR to R1 */
    "   ORR R1, R1, #0xC0000000     \n"     /* Set ASPEN_MASK and LSPEN_MASK */
    "   STR R1, [R0, #0]            \n"     /* Write back to FPCCR */
    "   MOV R1, R5                  \n"     /* Load CPACR value from R5 */
    "   ORR R1, R1, #0xF00000       \n"     /* Enable FPU on CPACR */
    "   LDR R0, =0xE000ED88         \n"
    "   STR R1, [R0, #0]            \n"     /* Write back to CPACR */
    "   LDR R1, =0x55400015         \n"
    "   VMSR FPSCR, R1              \n"     /* load process stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_ASM_BKPT:: \n"
#endif
    "   VMRS R2, FPSCR              \n"     /* Get back process stack value */
    "   CMP R2, R1                  \n"     /* Verify value */
    "   BNE IEC60730_CPU_REGS_FPU_FPSCR_FAIL   \n"
    "   LDR R1, =0xA280008A         \n"
    "   VMSR FPSCR, R1              \n"
    "   VMRS R2, FPSCR              \n"
    "   CMP R2, R1                  \n"
    "   BNE IEC60730_CPU_REGS_FPU_FPSCR_FAIL   \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_PASS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_PASS_ASM_BKPT::  \n"
#endif
    "   MOVS %0, #0x1               \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   B IEC60730_CPU_REGS_FPU_FPSCR_DONE    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_FAIL:: \n"
#endif
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_FAIL_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_FAIL_ASM_BKPT::  \n"
#endif
    "   MOVS %0, #0x0               \n"      /* result equal SL_IEC60730_TEST_FAILED*/

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_DONE:  \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_DONE:: \n"
#endif
    "   LDR R1, =0xE000ED88         \n"
    "   STR R5, [R1, #0]            \n"     /* Restore CPACR */
    "   LDR R1, =0xE000EF34         \n"
    "   STR R6, [R1, #0]            \n"     /* Restore FPCCR */
    "   VMSR FPSCR, R4              \n"     /* Restore FPSCR */
    "   LDMIA SP!, {R4, R5, R6}     \n"
    "   CPSIE i                     \n"     /*Enable IRQ Interrupts*/
    : "=r" (result)::"memory");
#endif

#if (IEC60370_CPU == IEC60370_CM4)
  __asm volatile(
    "   CPSID i                     \n"     /*Disable IRQ Interrupts*/
    "   STMDB SP!, {R4, R5, R6}     \n"
    "   MOVW R1, #0x0004            \n"
    "   MSR CONTROL, R1             \n"
    "   LDR.W R0, =0xE000ED88       \n"
    "   LDR R5, [R0, #0]            \n"     /* Save CPACR to R5 */
    "   LDR R0, =0xE000EF34         \n"
    "   LDR R6, [R0, #0]            \n"     /* Save FPCCR to R6 */
    "   VMRS R4, FPSCR              \n"     /* Save FPSCR register to R4 */
    "   MOV R1, R6                  \n"     /* Copy FPCCR to R1 */
    "   ORR R1, R1, #0xC0000000     \n"     /* Set ASPEN_MASK and LSPEN_MASK */
    "   STR R1, [R0, #0]            \n"     /* Write back to FPCCR */
    "   MOV R1, R5                  \n"     /* Load CPACR value from R5 */
    "   ORR R1, R1, #0xF00000       \n"     /* Enable FPU on CPACR */
    "   LDR R0, =0xE000ED88         \n"
    "   STR R1, [R0, #0]            \n"     /* Write back to CPACR */
    "   LDR R1, =0x55400015         \n"
    "   VMSR FPSCR, R1              \n"     /* load process stack value */
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_ASM_BKPT:: \n"
#endif
    "   VMRS R2, FPSCR              \n"     /* Get back process stack value */
    "   CMP R2, R1                  \n"     /* Verify value */
    "   BNE IEC60730_CPU_REGS_FPU_FPSCR_FAIL   \n"
    "   LDR R1, =0xA280008A         \n"
    "   VMSR FPSCR, R1              \n"
    "   VMRS R2, FPSCR              \n"
    "   CMP R2, R1                  \n"
    "   BNE IEC60730_CPU_REGS_FPU_FPSCR_FAIL   \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_PASS_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_PASS_ASM_BKPT::  \n"
#endif
    "   MOVS %0, #0x1               \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   B IEC60730_CPU_REGS_FPU_FPSCR_DONE    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_FAIL:: \n"
#endif
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_FAIL_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_FAIL_ASM_BKPT::  \n"
#endif
    "   MOVS %0, #0x0                 \n"   /* result equal SL_IEC60730_TEST_FAILED*/
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_FPSCR_DONE:  \n"
#else
    "IEC60730_CPU_REGS_FPU_FPSCR_DONE:: \n"
#endif
    "   LDR R1, =0xE000ED88           \n"
    "   STR R5, [R1, #0]              \n"     /* Restore CPACR */
    "   LDR R1, =0xE000EF34           \n"
    "   STR R6, [R1, #0]              \n"     /* Restore FPCCR */
    "   VMSR FPSCR, R4                \n"     /* Restore FPSCR */
    "   LDMIA SP!, {R4, R5, R6}       \n"
    "   CPSIE i                       \n"     /*Enable IRQ Interrupts*/
    : "=r" (result)::"memory");
#endif

  return result;
}

/**************************************************************************/ /**
 * private IEC60730 FPU Registers FPS0-FPS31 Check
 *
 * @returns #sl_iec60730_test_result_t
 *          #SL_IEC60730_TEST_FAILED means test failed,
 *          #SL_IEC60730_TEST_PASSED means test passed.
 *
 * This function tests the FPS0-FPS31 register.
 * Test patterns: 0x55555555, 0xAAAAAAAA
 * @warning The core must be in the secure state.Only for devices with the
 * Floating Point Unit (FPU).
 *****************************************************************************/
sl_iec60730_test_result_t sl_iec60730_cpu_registers_fpu_fpsx(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_PASSED;
#if (IEC60370_CPU == IEC60370_CM4)                             \
  || ((IEC60370_CPU == IEC60370_CM33) && defined(__TZ_PRESENT) \
  && (__TZ_PRESENT == 1))                                      \
  && defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  __asm volatile(
    "   CPSID i                       \n"     /*Disable IRQ Interrupts*/
    "   STMDB SP!, {R4, R5, R6}       \n"
    "   LDR.W R0, =0xE000ED88         \n"
    "   LDR R5, [R0, #0]              \n"     /* Save CPACR to R5 */
    "   LDR.W R0, =0xE000EF34         \n"
    "   LDR R6, [R0, #0]              \n"     /* Save FPCCR to R6 */
    "   VMRS R4, FPSCR                \n"     /* Save FPSCR register to R4 */
    "   MOV R1, R6                    \n"     /* Copy FPCCR to R1 */
    "   ORR R1, R1, #0xC0000000       \n"     /* Set ASPEN_MASK and LSPEN_MASK */
    "   STR R1, [R0, #0]              \n"     /* Write back to FPCCR */
    "   MOV R1, R5                    \n"     /* Load CPACR value from R5 */
    "   ORR R1, R1, #0xF00000         \n"     /* Enable FPU on CPACR */
    "   LDR.W R0, =0xE000ED88         \n"
    "   STR R1, [R0, #0]              \n"     /* Write back to CPACR */
    "   VMOV R0, S0                   \n"   /*S0 */
    "   LDR.W R1, =0x55555555         \n"
    "   VMOV S0, R1                   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S0_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S0_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S0                   \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FPU_S0_S3_FAIL    \n"
    "   LDR.W R1, =0xAAAAAAAA         \n"
    "   VMOV S0, R1                   \n"
    "   VMOV R2, S0                   \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FPU_S0_S3_FAIL    \n"
    "   VMOV S0, R0                   \n"

    "   VMOV R0, S1                   \n"   /* S1 */
    "   LDR.W R1, =0x55555555         \n"
    "   VMOV S1, R1                   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S1_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S1_ASM_BKPT::  \n"
#endif
    "   VMOV R2, S1                   \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FPU_S0_S3_FAIL    \n"
    "   LDR.W R1, =0xAAAAAAAA         \n"
    "   VMOV S1, R1                   \n"
    "   VMOV R2, S1                   \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FPU_S0_S3_FAIL    \n"
    "   VMOV S1, R0                   \n"

    "   VMOV R0, S2                   \n"   /* S2 */
    "   LDR.W R1, =0x55555555         \n"
    "   VMOV S2, R1                   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S2_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S2_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S2                   \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FPU_S0_S3_FAIL    \n"
    "   LDR.W R1, =0xAAAAAAAA         \n"
    "   VMOV S2, R1                   \n"
    "   VMOV R2, S2                   \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FPU_S0_S3_FAIL    \n"
    "   VMOV S2, R0                   \n"

    "   VMOV R0, S3                   \n"   /* S3 */
    "   LDR.W R1, =0x55555555         \n"
    "   VMOV S3, R1                   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S3_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S3_ASM_BKPT::  \n"
#endif
    "   VMOV R2, S3                   \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FPU_S0_S3_FAIL    \n"
    "   LDR.W R1, =0xAAAAAAAA         \n"
    "   VMOV S3, R1                   \n"
    "   VMOV R2, S3                   \n"
    "   CMP R2, R1                    \n"
    "   BNE IEC60730_CPU_REGS_FPU_S0_S3_FAIL    \n"
    "   VMOV S3, R0                   \n"

    "   B IEC60730_CPU_REGS_FPU_S0_S3_PASS      \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S0_S3_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S0_S3_FAIL:: \n"
#endif
    "   B IEC60730_CPU_REGS_FPU_S4_S7_FAIL      \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S0_S3_PASS:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S0_S3_PASS:: \n"
#endif

    "   VMOV R0, S4                     \n"   /* S4 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S4, R1                     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S4_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S4_ASM_BKPT::  \n"
#endif
    "   VMOV R2, S4                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S4_S7_FAIL    \n"
    "   LDR.W R1, =0xAAAAAAAA           \n"
    "   VMOV S4, R1                     \n"
    "   VMOV R2, S4                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S4_S7_FAIL    \n"
    "   VMOV S4, R0                     \n"

    "   VMOV R0, S5                     \n"   /* S5 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S5, R1                     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S5_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S5_ASM_BKPT::  \n"
#endif
    "   VMOV R2, S5                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S4_S7_FAIL    \n"
    "   LDR.W R1, =0xAAAAAAAA           \n"
    "   VMOV S5, R1                     \n"
    "   VMOV R2, S5                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S4_S7_FAIL    \n"
    "   VMOV S5, R0                     \n"

    "   VMOV R0, S6                     \n"   /* S6 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S6, R1                     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S6_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S6_ASM_BKPT::  \n"
#endif
    "   VMOV R2, S6                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S4_S7_FAIL    \n"
    "   LDR.W R1, =0xAAAAAAAA           \n"
    "   VMOV S6, R1                     \n"
    "   VMOV R2, S6                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S4_S7_FAIL    \n"
    "   VMOV S6, R0                     \n"

    "   VMOV R0, S7                     \n"   /* S7 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S7, R1                     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S7_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S7_ASM_BKPT::  \n"
#endif
    "   VMOV R2, S7                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S4_S7_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S7, R1                     \n"
    "   VMOV R2, S7                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S4_S7_FAIL    \n"
    "   VMOV S7, R0   \n"

    "   B IEC60730_CPU_REGS_FPU_S4_S7_PASS      \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S4_S7_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S4_S7_FAIL:: \n"
#endif
    "   B IEC60730_CPU_REGS_FPU_S8_S11_FAIL     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S4_S7_PASS:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S4_S7_PASS:: \n"
#endif

    "   VMOV R0, S8                     \n"   /* S8 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S8, R1                     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S8_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S8_ASM_BKPT::  \n"
#endif
    "   VMOV R2, S8                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S8_S11_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S8, R1                     \n"
    "   VMOV R2, S8                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S8_S11_FAIL   \n"
    "   VMOV S8, R0                     \n"

    "   VMOV R0, S9                     \n"   /* S9 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S9, R1                     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S9_ASM_BKPT:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S9_ASM_BKPT::  \n"
#endif
    "   VMOV R2, S9                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S8_S11_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S9, R1                     \n"
    "   VMOV R2, S9                     \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S8_S11_FAIL   \n"
    "   VMOV S9, R0                     \n"

    "   VMOV R0, S10                    \n"   /* S10 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S10, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S10_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S10_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S10                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S8_S11_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S10, R1                    \n"
    "   VMOV R2, S10                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S8_S11_FAIL   \n"
    "   VMOV S10, R0                    \n"

    "   VMOV R0, S11                    \n"   /* S11 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S11, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S11_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S11_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S11                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S8_S11_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S11, R1                    \n"
    "   VMOV R2, S11                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S8_S11_FAIL   \n"
    "   VMOV S11, R0                    \n"

    "   B IEC60730_CPU_REGS_FPU_S8_S11_PASS     \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S8_S11_FAIL:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S8_S11_FAIL::  \n"
#endif
    "   B IEC60730_CPU_REGS_FPU_S12_S15_FAIL    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S8_S11_PASS:   \n"
#else
    "IEC60730_CPU_REGS_FPU_S8_S11_PASS::  \n"
#endif

    "   VMOV R0, S12                    \n"   /* S12 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S12, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S12_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S12_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S12                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S12_S15_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S12, R1                    \n"
    "   VMOV R2, S12                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S12_S15_FAIL    \n"
    "   VMOV S12, R0                    \n"

    "   VMOV R0, S13                    \n"   /* S13 */
    "   LDR R1, =0x55555555 \n"
    "   VMOV S13, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S13_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S13_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S13                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S12_S15_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S13, R1                    \n"
    "   VMOV R2, S13                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S12_S15_FAIL    \n"
    "   VMOV S13, R0                    \n"

    "   VMOV R0, S14                    \n"   /* S14 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S14, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S14_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S14_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S14                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S12_S15_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S14, R1                    \n"
    "   VMOV R2, S14                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S12_S15_FAIL    \n"
    "   VMOV S14, R0                    \n"

    "   VMOV R0, S15                    \n"   /* S15 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S15, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S15_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S15_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S15                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S12_S15_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S15, R1                    \n"
    "   VMOV R2, S15                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S12_S15_FAIL    \n"
    "   VMOV S15, R0                    \n"

    "   B IEC60730_CPU_REGS_FPU_S12_S15_PASS      \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S12_S15_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S12_S15_FAIL:: \n"
#endif
    "   B IEC60730_CPU_REGS_FPU_S16_S19_FAIL      \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S12_S15_PASS:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S12_S15_PASS:: \n"
#endif

    "   VMOV R0, S16                    \n"   /* S16 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S16, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S16_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S16_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S16                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S16_S19_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S16, R1                    \n"
    "   VMOV R2, S16                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S16_S19_FAIL    \n"
    "   VMOV S16, R0                    \n"

    "   VMOV R0, S17                    \n"   /* S17 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S17, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S17_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S17_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S17                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S16_S19_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S17, R1                    \n"
    "   VMOV R2, S17                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S16_S19_FAIL    \n"
    "   VMOV S17, R0                    \n"

    "   VMOV R0, S18                    \n"   /* S18 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S18, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S18_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S18_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S18                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S16_S19_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S18, R1                    \n"
    "   VMOV R2, S18                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S16_S19_FAIL    \n"
    "   VMOV S18, R0                    \n"

    "   VMOV R0, S19                    \n"   /* S19 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S19, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S19_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S19_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S19                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S16_S19_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S19, R1                    \n"
    "   VMOV R2, S19                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S16_S19_FAIL    \n"
    "   VMOV S19, R0                    \n"

    "   B IEC60730_CPU_REGS_FPU_S16_S19_PASS      \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S16_S19_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S16_S19_FAIL:: \n"
#endif
    "   B IEC60730_CPU_REGS_FPU_S20_S23_FAIL      \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S16_S19_PASS:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S16_S19_PASS:: \n"
#endif

    "   VMOV R0, S20                    \n"   /* S20 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S20, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S20_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S20_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S20                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S20_S23_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S20, R1                    \n"
    "   VMOV R2, S20                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S20_S23_FAIL   \n"
    "   VMOV S20, R0                    \n"

    "   VMOV R0, S21                    \n"   /* S21 */
    "   LDR R1, =0x55555555 \n"
    "   VMOV S21, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S21_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S21_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S21                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S20_S23_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S21, R1                    \n"
    "   VMOV R2, S21                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S20_S23_FAIL   \n"
    "   VMOV S21, R0                    \n"

    "   VMOV R0, S22                    \n"   /* S22 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S22, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S22_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S22_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S22                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S20_S23_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S22, R1                    \n"
    "   VMOV R2, S22                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S20_S23_FAIL   \n"
    "   VMOV S22, R0                    \n"

    "   VMOV R0, S23                    \n"   /* S23 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S23, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S23_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S23_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S23                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S20_S23_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S23, R1                    \n"
    "   VMOV R2, S23                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S20_S23_FAIL   \n"
    "   VMOV S23, R0                    \n"

    "   B IEC60730_CPU_REGS_FPU_S20_S23_PASS    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S20_S23_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S20_S23_FAIL:: \n"
#endif
    "   B IEC60730_CPU_REGS_FPU_S24_S27_FAIL   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S20_S23_PASS:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S20_S23_PASS:: \n"
#endif

    "   VMOV R0, S24                    \n"   /* S24 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S24, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S24_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S24_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S24                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S24_S27_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S24, R1                    \n"
    "   VMOV R2, S24                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S24_S27_FAIL   \n"
    "   VMOV S24, R0                    \n"

    "   VMOV R0, S25                    \n"   /* S25 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S25, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S25_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S25_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S25                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S24_S27_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S25, R1                    \n"
    "   VMOV R2, S25                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S24_S27_FAIL   \n"
    "   VMOV S25, R0                    \n"

    "   VMOV R0, S26                    \n"   /* S26 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S26, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S26_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S26_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S26                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S24_S27_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S26, R1                    \n"
    "   VMOV R2, S26                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S24_S27_FAIL   \n"
    "   VMOV S26, R0                    \n"

    "   VMOV R0, S27                    \n"   /* S27 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S27, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S27_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S27_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S27                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S24_S27_FAIL    \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S27, R1                    \n"
    "   VMOV R2, S27                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_S24_S27_FAIL    \n"
    "   VMOV S27, R0                    \n"

    "   B IEC60730_CPU_REGS_FPU_S24_S27_PASS      \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S24_S27_FAIL:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S24_S27_FAIL:: \n"
#endif
    "   B IEC60730_CPU_REGS_FPU_Sx_FAIL   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S24_S27_PASS:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S24_S27_PASS:: \n"
#endif

    "   VMOV R0, S28                    \n"   /* S28 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S28, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S28_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S28_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S28                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_Sx_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S28, R1                    \n"
    "   VMOV R2, S28                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_Sx_FAIL   \n"
    "   VMOV S28, R0                    \n"

    "   VMOV R0, S29                    \n"   /* S29 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S29, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S29_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S29_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S29                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_Sx_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S29, R1                    \n"
    "   VMOV R2, S29                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_Sx_FAIL   \n"
    "   VMOV S29, R0                    \n"

    "   VMOV R0, S30                    \n"   /* S30 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S30, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S30_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S30_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S30                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_Sx_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S30, R1                    \n"
    "   VMOV R2, S30                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_Sx_FAIL   \n"
    "   VMOV S30, R0                    \n"

    "   VMOV R0, S31                    \n"   /* S31 */
    "   LDR.W R1, =0x55555555           \n"
    "   VMOV S31, R1                    \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_S31_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_S31_ASM_BKPT:: \n"
#endif
    "   VMOV R2, S31                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_Sx_FAIL   \n"
    "   LDR R1, =0xAAAAAAAA             \n"
    "   VMOV S31, R1                    \n"
    "   VMOV R2, S31                    \n"
    "   CMP R2, R1                      \n"
    "   BNE IEC60730_CPU_REGS_FPU_Sx_FAIL   \n"
    "   VMOV S31, R0                    \n"

#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_Sx_PASS_ASM_BKPT:  \n"
#else
    "IEC60730_CPU_REGS_FPU_Sx_PASS_ASM_BKPT:: \n"
#endif
    "   MOVS %0, #0x1                   \n"     /* result equal SL_IEC60730_TEST_PASSED*/
    "   B IEC60730_CPU_REGS_FPU_Sx_DONE   \n"
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_Sx_FAIL:   \n"
#else
    "IEC60730_CPU_REGS_FPU_Sx_FAIL::  \n"
#endif
    "   MOVS %0, #0x0                   \n"     /* result equal SL_IEC60730_TEST_FAILED*/
#ifndef IAR_TESTING /* GCC */
    "IEC60730_CPU_REGS_FPU_Sx_DONE: \n"
#else
    "IEC60730_CPU_REGS_FPU_Sx_DONE::\n"
#endif
    "   LDR.W R1, =0xE000ED88           \n"
    "   STR R5, [R1, #0]                \n"     /* Restore CPACR */
    "   LDR.W R1, =0xE000EF34           \n"
    "   STR R6, [R1, #0]                \n"     /* Restore FPCCR */
    "   VMSR FPSCR, R4                  \n"     /* Restore FPSCR */
    "   LDMIA SP!, {R4, R5, R6}         \n"
    "   CPSIE i                         \n"     /*Enable IRQ Interrupts*/
    : "=r" (result)::"memory");
#endif
  return result;
}
#endif // ((defined (__FPU_PRESENT) && (__FPU_USED == 1U))

/** @} (end addtogroup IEC60730_CPUREG_Test) */
/** @} (end addtogroup efr32_iec60730) */
