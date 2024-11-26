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
# @defgroup IEC60730_SYSTEM_CLOCK_VERIFICATION System Clock Automated Verification Tests
# @{
# Python script for the IEC60730 system clock Verification tests.
#
# For details on the tests, see iec60730_system_clock.iec60730_system_clock.
#
#@section system_clock_test_results Test Results
#
#EFR32 Device | Test Results
#-------------|-------------


from binascii import hexlify
import random
import os
import sys
from time import sleep

from iec60730_test_base import *
import jlink, jlink_constants
import unittest
import enums

def int_to_bytes(number: int) -> bytes:
    return number.to_bytes(length=(8 + (number + (number < 0)).bit_length()) // 8, byteorder='big', signed=True)

##  IEC60730 System Clock Verification Tests
#
# @details This class runs IEC60730 verification tests for the system clock test.
#
# Hardware setup:
#   Connect DUT to PC. No additional hardware setup required.

class iec60730_system_clock(unittest.TestCase, iec60730TestBase):
  ## Text name of the test suite, used in XML output.
  TEST_SUITE_NAME = "System Clock"

  ## Set up connect device.
  def setUp(self):
    self.env_setup(adapter_serial_no, chip_name, lst_file_path, lib_path, compiler)


  @iec60730_test_case('TC: System Clock execution test')
  ## Verify the DUT repeatedly executes the system clock test, indicating no erroneous entry into safe state.
  # @details Order of execution:
  #   - Verify code reaches breakpoint at IEC60730_SYSTEM_CLOCK_TICK_BKPT (to verify Clock counter is working).
  #   - Verify code reaches breakpoint at IEC60730_TEST_CLOCK_TICK_BKPT (To verify Clock counter is checked. Test finishes without stuck in safe state.).
  #     - Test passes, return True
  #
  # @return True
  #
  def test_system_clock_execution(self):
    # Set breakpoints in system clock test-related ISRs.
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    bp1 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_SYSTEM_CLOCK_TICK_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    # Reset device
    self.adapter.reset()
    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_SYSTEM_CLOCK_TICK_BKPT'],\
                    "DUT failed to enter system clock tick" \
                    " , pc = " + hex(pc))

    self.adapter.clear_all_breakpoints()
    bp2 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_TEST_CLOCK_TICK_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)

    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_TEST_CLOCK_TICK_BKPT'],\
                    "DUT did not run system clock test" \
                    " , pc = " + hex(pc))

    self.adapter.clear_all_breakpoints()
    bp1 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_SYSTEM_CLOCK_TICK_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)

    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_SYSTEM_CLOCK_TICK_BKPT'],\
                    "DUT did not run system clock test a second time, indicating safe state entry" \
                    " , pc = " + hex(pc))

    logging.info("DUT correctly running system clock test")
    return True

  @iec60730_test_case('TC: Enter SafeState when system clock out of bound')
  ## Verifies that the BIST system clock test enters safe state when out-of-bounds system clock is enabled
  # @details The system clock test is validated by forcing a system clock frequency that runs low
  # bound checked by the test.  Testing validates that the system enters safe state
  # after the system clock is forced low bound.
  #
  # Order of execution:
  #   - Verify code reaches breakpoint at IEC60730_SYSTEM_CLOCK_FREQ_ADJUSTMENT_BKPT
  #   - Force value of sl_iec60730_sys_clock_count to be out of bound
  #   - Verify code reaches breakpoint at IEC60730_SAFE_STATE_BKPT
  #     - Code correctly tested entered sl_iec60730_safe_state() due to out of bounds clock
  #     - Test passes, return True
  #
  # @return True

  def test_system_clock_out_of_bound(self):
    sys_clock_count_location = variables['sl_iec60730_sys_clock_count']
    self.assertNotEqual(0, sys_clock_count_location,\
                    "Can not read sl_iec60730_sys_clock_count address")
    logging.info("Set sl_iec60730_sys_clock_count flag at address: " + hex(sys_clock_count_location))

    # Set breakpoint to force system clocks to to appear as though they've not executed to frequently/infrequently, and breakpoint for safe state entry
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    self.adapter.set_breakpoint(address = asm_labels['IEC60730_TEST_CLOCK_TICK_BKPT'],
                                typeflags=enums.JLinkBreakpoint.ANY)
    self.adapter.set_breakpoint(address = asm_labels['IEC60730_SAFE_STATE_BKPT'],
                                typeflags=enums.JLinkBreakpoint.ANY)

    # Reset device
    self.adapter.reset()
    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_TEST_CLOCK_TICK_BKPT'],\
                    "DUT failed to reach system clock plausibility test" \
                    " , pc = " + hex(pc))

    # Set count of system clock to be 0, when minimum in code is 9
    self.adapter.memory_write8(addr = sys_clock_count_location, data =  {0x01}, zone=None)

    pc = self.adapter.run_to_breakpoint(2)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_SAFE_STATE_BKPT'],\
                    "DUT did not enter safe state after system clock plausibility fail" \
                    " , pc = " + hex(pc))

    self.adapter.reset()
    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_TEST_CLOCK_TICK_BKPT'],\
                    "DUT failed to reach system clock plausibility test" \
                    " , pc = " + hex(pc))

    # Set count of system clock to be 13, when maximum in code is 10
    self.adapter.memory_write8(addr = sys_clock_count_location, data =  {0x0D}, zone=None)

    pc = self.adapter.run_to_breakpoint(2)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_SAFE_STATE_BKPT'],\
                    "DUT did not enter safe state after system clock plausibility fail" \
                    " , pc = " + hex(pc))
    logging.info("DUT correctly entered safe state after system clock plausibility check failure")
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

  while len(sys.argv) > 1:
    ## Number of arguments passed into the script file
    line = sys.argv.pop()
    if len(sys.argv) == 1:
        ## compiler creates the file *.lst
        compiler = line

  print("Compiler: "+compiler)

  unittest.main()

## @}
# @}
