#!/usr/bin/env python
# License
# <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
# *******************************************************************************
#
# The licensor of this software is Silicon Laboratories Inc. Your use of this
# software is governed by the terms of Silicon Labs Master Software License
# Agreement (MSLA) available at
# www.silabs.com/about-us/legal/master-software-license-agreement. This
# software is distributed to you in Source Code format and is governed by the
# sections of the MSLA applicable to Source Code.
# *******************************************************************************

## @addtogroup IEC60730_INTEGRATION_TEST
# @{
# @defgroup IEC60730_INVARIABLE_MEMORY_VERIFICATION Invariable Memory Automated Verification Tests
# @{
# Python script for the IEC60730 Invariable Memory plausibility verification tests.
#
# For details on the tests, see iec60730_invariable_memory.iec60730_invariable_memory.
#
#@section irq_test_results Test Results
#
#EFR32 Device| Test Results
#------------|-------------

from binascii import hexlify
import random
import os
import sys
from time import sleep

from iec60730_test_base import *
import jlink, jlink_constants
import unittest
import enums

is_crc32 = False

##  IEC60730 Invariable Memory Plausibility Verification Tests
#
# @details This class runs IEC60730 verification tests for the invariable memory plausibility test.
#
# Hardware setup:
#   Connect DUT to PC. No additional hardware setup required.
class iec60730_imc(unittest.TestCase, iec60730TestBase):
  ## Text name of the test suite, used in XML output.
  TEST_SUITE_NAME = "IMC"

  ## Set up connect device.
  def setUp(self):
    self.env_setup(adapter_serial_no, chip_name, lst_file_path, lib_path, compiler)

  ## Set breakpoint.
  def set_breakpoint(self, label) :
    self.adapter.set_breakpoint(address = asm_labels[label],
                                typeflags=enums.JLinkBreakpoint.ANY)

  ## Run to breakpoint.
  def reach_to_breakpoint(self, label, setbkp, time) :
    if setbkp :
      self.set_breakpoint(label)

    pc = self.adapter.run_to_breakpoint(time)
    self.assertEqual(pc, asm_labels[label], \
                    "DUT failed to stop at " + label + \
                    ", pc = " + hex(pc) + \
                    ", setbkp = " + str(setbkp))

  @iec60730_test_case('TC: Calculate CRC for the entire flash memory')
  ## Verify the DUT calculation of CRC value at POST. This  test case (TC) checks
  # under normal conditions with no exceptions, firmware will calculate the CRC
  # of the entire flash, the calculated value SHOULD be equal to the calculated
  # value stored in the flash.
  #
  # @details Order of execution
  # - Reset device
  # - Verify code reaches breakpoint at IEC60730_IMC_POST_START_BKPT
  # - Verify code reaches breakpoint at IEC60730_IMC_POST_IEC60730_REF_CRC_BKPT
  # - Verify code reaches breakpoint at IEC60730_IMC_POST_END_BKPT
  #
  # @return True
  #
  def test_cal_crc_full_memory(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()

    # Reset device & run to break point
    self.adapter.reset()

    # reach to break point IEC60730_IMC_POST_START_BKPT
    self.reach_to_breakpoint('IEC60730_IMC_POST_START_BKPT', True, 1)

    # Verify initial value of iec60730_ref_crc
    iec60730_ref_crc_location = variables['iec60730_ref_crc']
    if is_crc32 :
      iec60730_ref_crc = self.adapter.memory_read32(iec60730_ref_crc_location, 1)
    else :
      iec60730_ref_crc = self.adapter.memory_read16(iec60730_ref_crc_location, 1)

    # Debug
    logging.info("DUT iec60730_ref_crc=" + hex(iec60730_ref_crc[0]) + " at " + hex(iec60730_ref_crc_location))

    if 0 != iec60730_ref_crc[0] :
      logging.info("DUT value of iec60730_ref_crc not equal 0")
      return False

    # reach to break point IEC60730_IMC_POST_IEC60730_REF_CRC_BKPT
    self.reach_to_breakpoint('IEC60730_IMC_POST_IEC60730_REF_CRC_BKPT', True, 1)

    # print value of iec60730_ref_crc
    if is_crc32 :
      iec60730_ref_crc = self.adapter.memory_read32(iec60730_ref_crc_location, 1)
    else :
      iec60730_ref_crc = self.adapter.memory_read16(iec60730_ref_crc_location, 1)

    # Debug
    logging.info("DUT iec60730_ref_crc=" + hex(iec60730_ref_crc[0]))

    # clear all break point
    self.adapter.clear_all_breakpoints()

    # reach to break point IEC60730_IMC_POST_END_BKPT
    self.reach_to_breakpoint('IEC60730_IMC_POST_END_BKPT', True, 1)

    return True

  @iec60730_test_case('TC: Corrupt CRC for the entire flash memory')
  ## Verify the DUT jump to #sl_iec60730_safe_state when flash faied at POST.
  # This TC simulates an abnormal situation that causes the variable \ref iec60730_ref_crc
  # to be changed, resulting in the calculated CRC value for the entire flash
  # being different from the value stored in the flash. This leads to jump to
  # break point at #sl_iec60730_safe_state.
  #
  # In the IMC documentation there is a detailed description of the variable
  # \ref iec60730_ref_crc.
  #
  # @details Order of execution
  # - Reset device
  # - Verify code reaches breakpoint at IEC60730_IMC_POST_START_BKPT
  # - Verify code reaches breakpoint at IEC60730_IMC_POST_IEC60730_REF_CRC_BKPT
  #   - Edit value of \ref iec60730_ref_crc variable
  #   - Verify execution breaks at #sl_iec60730_safe_state()
  #
  # @return True
  #
  def test_corrupt_crc_full_memory(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()

    # Reset device & jump to break point
    self.adapter.reset()

    # reach to break point IEC60730_IMC_POST_START_BKPT
    self.reach_to_breakpoint('IEC60730_IMC_POST_START_BKPT', True, 1)

    # reach to break point IEC60730_IMC_POST_START_BKPT
    self.reach_to_breakpoint('IEC60730_IMC_POST_IEC60730_REF_CRC_BKPT', True, 1)

    # Set iec60730_ref_crc to {0x0000}
    iec60730_ref_crc_location = variables['iec60730_ref_crc']
    if is_crc32 :
      self.adapter.memory_write32(iec60730_ref_crc_location, data = {0x00000000})
    else :
      self.adapter.memory_write16(iec60730_ref_crc_location, data = {0x0000})

    # Debug
    logging.info("DUT iec60730_ref_crc at " + hex(iec60730_ref_crc_location))

    # Set break point at sl_iec60730_safe_state
    self.adapter.clear_all_breakpoints()

    # reach to break point sl_iec60730_safe_state
    self.reach_to_breakpoint('sl_iec60730_safe_state', True, 1)

    return True

  @iec60730_test_case('TC: Calculate CRC for the Block flash memory')
  ## Verify the DUT calculation of CRC value at BIST. This TC checks under normal
  # conditions with no exceptions, firmware will calculate the CRC values for each
  # step of BIST and accumulate these values. Finally, the calculated CRC value
  # SHOULD be equal to the value stored in the flash.
  #
  # @details Order of execution
  # - Reset device
  # - Verify code reaches breakpoint at IEC60730_IMC_BIST_CRCBLOCK_BKPT
  # - Loop with IEC60730_IMC_BIST_CALCRC_BKPT
  # - Verify code reaches breakpoint at IEC60730_IMC_BIST_END_BKPT (success)
  #
  # @return True
  #
  def test_cal_crc_block_memory(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    self.adapter.reset()

    firstTime = True

    # Reset device & jump to break point
    self.reach_to_breakpoint('IEC60730_IMC_BIST_START_BKPT', True, 1)
    self.adapter.clear_all_breakpoints()

    iec60730_run_crc_location = variables['iec60730_run_crc']
    iec60730_run_crc = self.adapter.memory_read32(iec60730_run_crc_location, 1)

    rom_end_location = variables['check_sum']

    # Debug
    logging.info("DUT iec60730_run_crc at " + hex(iec60730_run_crc_location) + " check_sum at " + hex(rom_end_location))

    while iec60730_run_crc[0] < rom_end_location :
      # logging.info("TC: iec60730_run_crc: " + hex(iec60730_run_crc[0]) + " < ROM_END: " + hex(rom_end_location))
      self.reach_to_breakpoint('IEC60730_IMC_BIST_CRCBLOCK_BKPT', firstTime, 1)
      iec60730_run_crc = self.adapter.memory_read32(iec60730_run_crc_location, 1)
      firstTime = False

    self.adapter.clear_all_breakpoints()

    self.reach_to_breakpoint('IEC60730_IMC_BIST_CALCRC_BKPT', True, 1)

    # Verify initial value of crcCrc
    iec60730_cur_crc_location = variables['iec60730_cur_crc']
    if is_crc32 :
      iec60730_cur_crc = self.adapter.memory_read32(iec60730_cur_crc_location, 1)
    else :
      iec60730_cur_crc = self.adapter.memory_read16(iec60730_cur_crc_location, 1)

    # Debug
    logging.info("DUT iec60730_cur_crc=" + hex(iec60730_cur_crc[0]) + " at " + hex(iec60730_cur_crc_location))

    if 0 == iec60730_cur_crc[0] :
      logging.info("DUT value of iec60730_cur_crc equal 0")
      return False

    self.adapter.clear_all_breakpoints()
    self.reach_to_breakpoint('IEC60730_IMC_BIST_END_BKPT', True, 1)

    return True

  @iec60730_test_case('TC: Corrupt CRC at step calculate CRC for the Block flash memory')
  ## Verify the DUT jump to #sl_iec60730_safe_state when flash faied at BIST.
  # This TC simulates an abnormal situation that causes the variable \ref iec60730_cur_crc
  # to be changed, resulting in the calculated CRC value for the entire flash
  # being different from the value stored in the flash. This leads to jump to
  # break point at #sl_iec60730_safe_state.
  #
  # In the IMC documentation there is a detailed description of the variable
  # \ref iec60730_cur_crc.
  #
  # @details Order of execution
  # - Reset device
  # - Verify code reaches breakpoint at IEC60730_IMC_BIST_CALCRC_BKPT
  #   - Edit value of \ref iec60730_cur_crc
  #   - Verify execution breaks at #sl_iec60730_safe_state()
  #
  # @return True
  #
  def test_corrupt_crc_cal_crc_block_memory(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()

    # Reset device & jump to break point
    self.adapter.reset()

    # reach to break point IEC60730_IMC_BIST_CALCRC_BKPT
    self.reach_to_breakpoint('IEC60730_IMC_BIST_CALCRC_BKPT', True, 1)

    # Verify initial value of crcCrc
    iec60730_cur_crc_location = variables['iec60730_cur_crc']
    if is_crc32 :
      iec60730_cur_crc = self.adapter.memory_read32(iec60730_cur_crc_location, 1)
    else :
      iec60730_cur_crc = self.adapter.memory_read16(iec60730_cur_crc_location, 1)
    logging.info("DUT iec60730_cur_crc=" + hex(iec60730_cur_crc[0]) + " at " + hex(iec60730_cur_crc_location))

    # Set iec60730_ref_crc to {0x0000}
    iec60730_ref_crc_location = variables['iec60730_ref_crc']
    logging.info("DUT iec60730_ref_crc at " + hex(iec60730_ref_crc_location))

    if is_crc32 :
      self.adapter.memory_write16(iec60730_ref_crc_location, data = {0x00000000})
    else :
      self.adapter.memory_write16(iec60730_ref_crc_location, data = {0x0000})

    # Set break point at sl_iec60730_safe_state
    self.adapter.clear_all_breakpoints()

    # reach to break point sl_iec60730_safe_state
    self.reach_to_breakpoint('sl_iec60730_safe_state', True, 1)

    return True

  @iec60730_test_case('TC: Corrupt integrity of iec60730_run_crc variable for the Block flash memory')
  ## Verify integrity of \ref iec60730_run_crc variable. This TC simulates an abnormal
  # situation that causes the variable iec60730_run_crc to be changed, resulting in the
  # calculated CRC value for the entire flash being different from the value stored
  # in the flash. This leads to jump to break point at #sl_iec60730_safe_state.
  #
  # In the IMC documentation there is a detailed description of the variable
  # \ref iec60730_run_crc.
  #
  # @details Order of execution
  # - Reset device
  # - Verify code reaches breakpoint at IEC60730_IMC_BIST_CRCBLOCK_BKPT
  #   - Edit value of \ref iec60730_run_crc variable
  #   - Verify execution breaks at #sl_iec60730_safe_state()
  #
  # @return True
  #
  def test_corrupt_integrity_iec60730_run_crc_block_memory(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()

    # Reset device & jump to break point
    self.adapter.reset()

    # reach to break point IEC60730_IMC_BIST_CRCBLOCK_BKPT
    self.reach_to_breakpoint('IEC60730_IMC_BIST_CRCBLOCK_BKPT', True, 1)

    iec60730_run_crc_location = variables['iec60730_run_crc']
    logging.info("DUT iec60730_run_crc at " + hex(iec60730_run_crc_location))

    if is_crc32 :
      self.adapter.memory_write16(iec60730_run_crc_location, data = {0x00000000})
    else :
      self.adapter.memory_write16(iec60730_run_crc_location, data = {0x0000})

    # reach to break point sl_iec60730_safe_state
    self.reach_to_breakpoint('sl_iec60730_safe_state', True, 1)

    return True


if __name__ == "__main__":

  ## Chip name run test
  chip_name = os.getenv('CHIP')

  ## Path to file *.lst
  lst_file_path = os.getenv('LST_PATH')

  ## serialno of device
  adapter_serial_no = os.getenv('ADAPTER_SN')

  ## Path to jlink library
  lib_path = os.getenv('JLINK_PATH')

  ## Enable test calculation with CRC32
  cal_crc_32 = os.getenv('INTEGRATION_TEST_ENABLE_CAL_CRC_32')
  if not cal_crc_32:
    cal_crc_32 = "disable"
  else:
    if cal_crc_32 == "enable":
      ## Allows testing with CRC32
      is_crc32 = True
  print("Enable calculate crc32: "+cal_crc_32)

  while len(sys.argv) > 1:
    ## Number of arguments passed into the script file
    line = sys.argv.pop()
    if len(sys.argv) == 1:
      ## compiler creates the file *.lst
      compiler = line

  unittest.main()

## @}
# @}
