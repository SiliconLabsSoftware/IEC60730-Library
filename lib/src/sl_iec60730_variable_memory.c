/***************************************************************************/ /**
 * @file
 * @brief Variable memory check
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

#include <stddef.h>

#include "sl_iec60730_internal.h"

#ifdef UNIT_TEST_IEC60730_VARIABLE_MEMORY_ENABLE
#include "unit_test_iec60730_variable_memory.h"
#endif // UNIT_TEST_IEC60730_VARIABLE_MEMORY_ENABLE

#define ALIGN_ADDRESS_32  0xFFFFFFFCUL

/* RAM location for temporary storage for run-time test */
static uint32_t iec60730_bk_buf[RT_BLOCKSIZE + 2] __RT_BUF;
/* RAM pointer for run-time test */
sl_static_dec_classb_vars(uint32_t *, iec60730_rt_check);
static uint32_t iec60730_stack_test_over_flow[4] __STACK_BOTTOM;
static __no_init sl_iec60730_vmc_test_multiple_regions_t iec60730_vmc_test_config __CLASSB_RAM;
static bool iec60730_vmc_init_flag = false;

extern bool sl_iec60730_vmc_marchc_step(uint32_t *start,
                                        uint32_t *end,
                                        uint32_t background,
                                        uint32_t *backup);
extern bool sl_iec60730_vmc_marchxc_step(uint32_t *start,
                                         uint32_t *end,
                                         uint32_t background,
                                         uint32_t *backup);

__WEAK bool sl_iec60730_vmc_pre_run_marchxc_step(uint32_t *addr, uint32_t size)
{
  UNUSED_VAR(addr);
  UNUSED_VAR(size);
  return true;
}

__WEAK void sl_iec60730_vmc_post_run_marchxc_step(uint32_t *addr, uint32_t size)
{
  UNUSED_VAR(addr);
  UNUSED_VAR(size);
}

void sl_iec60730_vmc_init(sl_iec60730_vmc_test_multiple_regions_t *test_config)
{
  if ((NULL == test_config) || (test_config->region == NULL)
      || (test_config->number_of_test_regions == 0)) {
    return;
  }

  iec60730_vmc_test_config.region = test_config->region;
  iec60730_vmc_test_config.number_of_test_regions = test_config->number_of_test_regions;
  iec60730_rt_check = (uint32_t *)((uint32_t)(test_config->region[0].start - (RT_BLOCK_OVERLAP << 1)) & ALIGN_ADDRESS_32);
  INV_CLASSB_PVAR(uint32_t, iec60730_rt_check);

  iec60730_stack_test_over_flow[0] = SL_STACK_OVERFLOW_CONST_GUARD_VALUE_0;
  iec60730_stack_test_over_flow[1] = SL_STACK_OVERFLOW_CONST_GUARD_VALUE_1;
  iec60730_stack_test_over_flow[2] = SL_STACK_OVERFLOW_CONST_GUARD_VALUE_2;
  iec60730_stack_test_over_flow[3] = SL_STACK_OVERFLOW_CONST_GUARD_VALUE_3;
  iec60730_vmc_init_flag = true;
}

sl_iec60730_test_result_t sl_iec60730_vmc_post(void)
{
  sl_iec60730_test_result_t result = SL_IEC60730_TEST_FAILED;
  uint8_t current_test_region = 0;

  SL_IEC60730_VMC_POST_ENTER_CRITICAL();

  if (!iec60730_vmc_init_flag) {
    goto VMC_POST_DONE;
  }

  while (current_test_region < iec60730_vmc_test_config.number_of_test_regions) {
    iec60730_rt_check = (uint32_t *)((uint32_t)(iec60730_vmc_test_config.region[current_test_region].start - (RT_BLOCK_OVERLAP << 1)) & ALIGN_ADDRESS_32);
    INV_CLASSB_PVAR(uint32_t, iec60730_rt_check);

    LABEL_DEF(IEC60730_VMC_POST_START_BKPT);

    // Check RAM region from begin to end
    while (iec60730_rt_check < (uint32_t *)iec60730_vmc_test_config.region[current_test_region].end) {
      if (!CHECK_INTEGRITY(uint32_t, iec60730_rt_check)) {
        goto VMC_POST_DONE;
      }

      // Only test iec60730_bk_buf in the last step
      if (iec60730_rt_check == iec60730_bk_buf) {
        iec60730_rt_check += sizeof(iec60730_bk_buf) / sizeof(iec60730_bk_buf[0]);
      }
      if (!sl_iec60730_vmc_marchc_step(iec60730_rt_check,
                                       (uint32_t *)((uint8_t *)(iec60730_rt_check + RT_BLOCKSIZE) - 1),
                                       BACKGROUND,
                                       iec60730_bk_buf)) {
        goto VMC_POST_DONE;
      }
      iec60730_rt_check += BLOCKSIZE;
      LABEL_DEF(IEC60730_VMC_POST_MARCHC_STEP_BKPT);
      INV_CLASSB_PVAR(uint32_t, iec60730_rt_check);
    }
    // Change the test region if the previous test region was successful
    current_test_region++;
  }

  LABEL_DEF(IEC60730_VMC_POST_CHECK_BKBUF_BKPT);

  // Check buffer iec60730_bk_buf
  if (CHECK_INTEGRITY(uint32_t, iec60730_rt_check)) {
    if (sl_iec60730_vmc_marchc_step(iec60730_bk_buf,
                                    (uint32_t *)((uint8_t *)(iec60730_bk_buf + sizeof(iec60730_bk_buf) / sizeof(iec60730_bk_buf[0])) - 1),
                                    BACKGROUND,
                                    iec60730_bk_buf)) {
      result = SL_IEC60730_TEST_PASSED;
    }
  }

  VMC_POST_DONE:
  SL_IEC60730_VMC_POST_EXIT_CRITICAL();

  return result;
}

sl_iec60730_test_result_t sl_iec60730_vmc_bist(void)
{
  LABEL_DEF(IEC60730_VMC_BIST_START_BKPT);

  sl_iec60730_test_result_t result = SL_IEC60730_TEST_IN_PROGRESS;
  uint8_t current_test_region = 0;

  SL_IEC60730_VMC_BIST_ENTER_CRITICAL();

  iec60730_rt_check = (uint32_t *)((uint32_t)(iec60730_vmc_test_config.region[current_test_region].start - (RT_BLOCK_OVERLAP << 1)) & ALIGN_ADDRESS_32);
  INV_CLASSB_PVAR(uint32_t, iec60730_rt_check);

  while (current_test_region < iec60730_vmc_test_config.number_of_test_regions) {
    for (uint16_t i = 0; i < SL_IEC60730_VAR_BLOCKS_PER_BIST; i++) {
      if (!CHECK_INTEGRITY(uint32_t, iec60730_rt_check)) {
        result = SL_IEC60730_TEST_FAILED;
        goto VMC_BIST_DONE;
      }

      if (iec60730_rt_check > (uint32_t *)iec60730_vmc_test_config.region[current_test_region].end) {
        LABEL_DEF(IEC60730_IMC_BIST_CHECK_BKBUF_BKPT);

        // Check backup buffer iec60730_bk_buf
        if (sl_iec60730_vmc_marchxc_step(iec60730_bk_buf,
                                         (uint32_t *)((uint8_t *)(iec60730_bk_buf + sizeof(iec60730_bk_buf) / sizeof(iec60730_bk_buf[0])) - 1),
                                         BACKGROUND,
                                         iec60730_bk_buf)) {
          result = SL_IEC60730_TEST_PASSED;
          current_test_region++;
          if (current_test_region >= iec60730_vmc_test_config.number_of_test_regions) {
            goto VMC_BIST_DONE;
          }
        } else {
          result = SL_IEC60730_TEST_FAILED;
          goto VMC_BIST_DONE;
        }

        iec60730_rt_check = (uint32_t *)((uint32_t)(iec60730_vmc_test_config.region[current_test_region].start - (RT_BLOCK_OVERLAP << 1)) & ALIGN_ADDRESS_32);
        INV_CLASSB_PVAR(uint32_t, iec60730_rt_check);
      } else {
        // Checking the allowance run testing
        if (!sl_iec60730_vmc_pre_run_marchxc_step(iec60730_rt_check, BLOCKSIZE)) {
          sl_iec60730_vmc_post_run_marchxc_step(iec60730_rt_check, BLOCKSIZE);
          iec60730_rt_check += BLOCKSIZE;
          // If rtCheck equal bkBuf then jump to next block
          if (iec60730_rt_check == iec60730_bk_buf) {
            iec60730_rt_check += sizeof(iec60730_bk_buf) / sizeof(iec60730_bk_buf[0]);
          }
          INV_CLASSB_PVAR(uint32_t, iec60730_rt_check);
          continue;
        }

        // If iec60730_rt_check equal iec60730_bk_buf then jump to next block
        if (iec60730_rt_check == iec60730_bk_buf) {
          iec60730_rt_check += sizeof(iec60730_bk_buf) / sizeof(iec60730_bk_buf[0]);
        }
        // Call MarchXC
        if (sl_iec60730_vmc_marchxc_step(iec60730_rt_check,
                                         (uint32_t *)((uint8_t *)(iec60730_rt_check + RT_BLOCKSIZE) - 1),
                                         BACKGROUND,
                                         iec60730_bk_buf)) {
          // Implement after invoking MarchXC
          sl_iec60730_vmc_post_run_marchxc_step(iec60730_rt_check, BLOCKSIZE);
          // Point to next block
          iec60730_rt_check += BLOCKSIZE;
          LABEL_DEF(IEC60730_VMC_BIST_MARCHC_STEP_BKPT);
          INV_CLASSB_PVAR(uint32_t, iec60730_rt_check);
        } else {
          // Implement after invoking MarchXC
          sl_iec60730_vmc_post_run_marchxc_step(iec60730_rt_check, BLOCKSIZE);
          // Set result failed
          result = SL_IEC60730_TEST_FAILED;
          goto VMC_BIST_DONE;
        }
      }
    }
  }

  VMC_BIST_DONE:
  SL_IEC60730_VMC_BIST_EXIT_CRITICAL();

  if ((result == SL_IEC60730_TEST_IN_PROGRESS) || (result == SL_IEC60730_TEST_PASSED)) {
    LABEL_DEF(IEC60730_VMC_BIST_STACK_TEST_BKPT);
    if ((iec60730_stack_test_over_flow[0] != SL_STACK_OVERFLOW_CONST_GUARD_VALUE_0)
        || (iec60730_stack_test_over_flow[1] != SL_STACK_OVERFLOW_CONST_GUARD_VALUE_1)
        || (iec60730_stack_test_over_flow[2] != SL_STACK_OVERFLOW_CONST_GUARD_VALUE_2)
        || (iec60730_stack_test_over_flow[3] != SL_STACK_OVERFLOW_CONST_GUARD_VALUE_3)) {
      result = SL_IEC60730_TEST_FAILED;
    } else {
      LABEL_DEF(IEC60730_VMC_BIST_STACK_TEST_OK_BKPT);
    }

    if (result == SL_IEC60730_TEST_PASSED) {
      sl_iec60730_program_counter_check |= (IEC60730_VMC_COMPLETE);
    }
  }

  return result;
}
