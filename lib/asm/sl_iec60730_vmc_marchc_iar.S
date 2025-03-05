;***************************************************************************//**
; @file
; @brief Variable memory March-X test
;*******************************************************************************
; # License
; <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
;*******************************************************************************
;
; The licensor of this software is Silicon Laboratories Inc. Your use of this
; software is governed by the terms of Silicon Labs Master Software License
; Agreement (MSLA) available at
; www.silabs.com/about-us/legal/master-software-license-agreement. This
; software is distributed to you in Source Code format and is governed by the
; sections of the MSLA applicable to Source Code.
;
;******************************************************************************/

	SECTION text:CODE(2)

  EXPORT sl_iec60730_vmc_marchc_step

;****************************************************************************/**
; public IEC60730 March C test for start-up
;
; @param   r0 - Start address of RAM to check
;          r1 - End address of RAM to check
;          r2 - Background pattern
;          r3 - Backup area
; @return  1 if passed, 0 if failed
;******************************************************************************/

sl_iec60730_vmc_marchc_step:
  PUSH  {r4-r7, lr}

  ; Setup inverted background pattern (r7)
  MOVS  r7, r2
  RSBS  r7, r7, #0
  SUBS  r7, r7, #1

  ; Backup buffer to be tested?
  MOVS  r5, r0
  CMP   r5, r3
  BEQ   IEC60730_MARCHC_STEP_BUFF1

  ; SAVE
IEC60730_MARCHC_STEP_SAVE:
  MOVS  r4, #0
IEC60730_MARCHC_STEP_SAVE_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_STEP1
  LDR   r6, [r5], #+4
  STR   r6, [r3, r4]
  ADDS  r4, r4, #+4
  B     IEC60730_MARCHC_STEP_SAVE_LOOP

  ; STEP 1
IEC60730_MARCHC_STEP_STEP1:
  MOVS  r5, r0
IEC60730_MARCHC_STEP_STEP1_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_STEP2
  STR   r2, [r5], #+4
  B     IEC60730_MARCHC_STEP_STEP1_LOOP

  ; STEP 2
IEC60730_MARCHC_STEP_STEP2:
  MOVS  r5, r0
IEC60730_MARCHC_STEP_STEP2_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_STEP3
  LDR   r6, [r5]
  CMP   r6, r2
  BNE   IEC60730_MARCHC_STEP_ERR
  STR   r7, [r5], #4
  B     IEC60730_MARCHC_STEP_STEP2_LOOP

  ; STEP 3
IEC60730_MARCHC_STEP_STEP3:
  MOVS  r5, r0
IEC60730_MARCHC_STEP_STEP3_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_STEP4
  LDR   r6, [r5]
  CMP   r6, r7
  BNE   IEC60730_MARCHC_STEP_ERR
  STR   r2, [r5], #4
  B     IEC60730_MARCHC_STEP_STEP3_LOOP

  ; STEP 4
IEC60730_MARCHC_STEP_STEP4:
  MOVS  r5, r1
  SUB   r5, #+3
IEC60730_MARCHC_STEP_STEP4_LOOP:
  CMP   r5, r0
  BLO   IEC60730_MARCHC_STEP_STEP5
  LDR   r6, [r5]
  CMP   r6, r2
  BNE   IEC60730_MARCHC_STEP_ERR
  STR   r7, [r5], #-4
  B     IEC60730_MARCHC_STEP_STEP4_LOOP

  ; STEP 5
IEC60730_MARCHC_STEP_STEP5:
  MOVS  r5, r1
  SUB   r5, #+3
IEC60730_MARCHC_STEP_STEP5_LOOP:
  CMP   r5, r0
  BLO   IEC60730_MARCHC_STEP_STEP6
  LDR   r6, [r5]
  CMP   r6, r7
  BNE   IEC60730_MARCHC_STEP_ERR
  STR   r2, [r5], #-4
  B     IEC60730_MARCHC_STEP_STEP5_LOOP

  ; STEP 6
IEC60730_MARCHC_STEP_STEP6:
  MOVS  r5, r0
IEC60730_MARCHC_STEP_STEP6_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_RESTORE
  LDR   r6, [r5], #4
  CMP   r6, r2
  BNE   IEC60730_MARCHC_STEP_ERR
  B     IEC60730_MARCHC_STEP_STEP6_LOOP

  ; RESTORE
IEC60730_MARCHC_STEP_RESTORE:
  MOVS  r5, r0
  MOVS  r4, #0
IEC60730_MARCHC_STEP_RESTORE_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_MARCH_RET
  LDR   r6, [r3, r4]
  STR   r6, [r5]
  ADDS  r5, r5, #+4
  ADDS  r4, r4, #+4
  B     IEC60730_MARCHC_STEP_RESTORE_LOOP

IEC60730_MARCHC_STEP_BUFF1:
  MOVS  r5, r0
IEC60730_MARCHC_STEP_BUFF1_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_BUFF2
  STR   r2, [r5], #+4
  B     IEC60730_MARCHC_STEP_BUFF1_LOOP

  ; STEP 2
IEC60730_MARCHC_STEP_BUFF2:
  MOVS  r5, r0
IEC60730_MARCHC_STEP_BUFF2_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_BUFF3
  LDR   r6, [r5]
  CMP   r6, r2
  BNE   IEC60730_MARCHC_STEP_ERR
  STR   r7, [r5], #4
  B     IEC60730_MARCHC_STEP_BUFF2_LOOP

  ; STEP 3
IEC60730_MARCHC_STEP_BUFF3:
  MOVS  r5, r0
IEC60730_MARCHC_STEP_BUFF3_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_BUFF4
  LDR   r6, [r5]
  CMP   r6, r7
  BNE   IEC60730_MARCHC_STEP_ERR
  STR   r2, [r5], #4
  B     IEC60730_MARCHC_STEP_BUFF3_LOOP

  ; STEP 4
IEC60730_MARCHC_STEP_BUFF4:
  MOVS  r5, r1
  SUB   r5, #+3
IEC60730_MARCHC_STEP_BUFF4_LOOP:
  CMP   r5, r0
  BLO   IEC60730_MARCHC_STEP_BUFF5
  LDR   r6, [r5]
  CMP   r6, r2
  BNE   IEC60730_MARCHC_STEP_ERR
  STR   r7, [r5], #-4
  B     IEC60730_MARCHC_STEP_BUFF4_LOOP

  ; STEP 5
IEC60730_MARCHC_STEP_BUFF5:
  MOVS  r5, r1
  SUB   r5, #+3
IEC60730_MARCHC_STEP_BUFF5_LOOP:
  CMP   r5, r0
  BLO   IEC60730_MARCHC_STEP_BUFF6
  LDR   r6, [r5]
  CMP   r6, r7
  BNE   IEC60730_MARCHC_STEP_ERR
  STR   r2, [r5], #-4
  B     IEC60730_MARCHC_STEP_BUFF5_LOOP

  ; STEP 6
IEC60730_MARCHC_STEP_BUFF6:
  MOVS  r5, r0
IEC60730_MARCHC_STEP_BUFF6_LOOP:
  CMP   r5, r1
  BHI   IEC60730_MARCHC_STEP_MARCH_RET
  LDR   r6, [r5], #4
  CMP   r6, r2
  BNE   IEC60730_MARCHC_STEP_ERR
  B     IEC60730_MARCHC_STEP_BUFF6_LOOP

IEC60730_MARCHC_STEP_MARCH_RET:
  MOVS  r0, #1
  B     IEC60730_MARCHC_STEP_RET

IEC60730_MARCHC_STEP_ERR:
  MOVS  r0, #0

IEC60730_MARCHC_STEP_RET:
  POP   {r4-r7, pc}

  END
