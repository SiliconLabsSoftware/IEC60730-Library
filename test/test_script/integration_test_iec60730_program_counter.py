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
# @defgroup IEC60730_PROGRAMME_COUNTER_VERIFICATION Program Counter Verification Tests
# @{
# Python script for the IEC60730 Program counter Verification tests.
#
# For details on the tests, see iec60730_programme_counter.iec60730_programme_counter.
#
#@section programme_counter_test_results Test Results
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


##  IEC60730 CPU Program counter Tests
#
# @details This class runs IEC60730 Program counter tests.
#
# Hardware setup:
#   Connect DUT to PC. No additional hardware setup required.

class iec60730_programme_counter(unittest.TestCase, iec60730TestBase):
  ## Text name of the test suite, used in XML output.
  TEST_SUITE_NAME = "PROGRAM COUNTER"

  ## Set up connect device.
  def setUp(self):
    self.env_setup(adapter_serial_no, chip_name, lst_file_path, lib_path, compiler)


  @iec60730_test_case('TC: Verify Program counter execution')
  ## Verify the DUT repeatedly Program counter test, indicating Program counter correctly running.
  # @details Order of execution:
  #   - Verify code reaches breakpoint at IEC60730_PC_BKPT
  #   - Verify code reaches breakpoint at IEC60730_PC_BKPT
  #     - Test passes, return True
  #
  # @return True
  #
  def test_program_counter_execution(self):
    # Set breakpoints in system clock test-related ISRs.
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()

    self.adapter.set_breakpoint(address = asm_labels['IEC60730_PC_BKPT'],
                                 typeflags=enums.JLinkBreakpoint.ANY)

    self.adapter.set_breakpoint(address = asm_labels['sl_iec60730_safe_state'],
                                 typeflags=enums.JLinkBreakpoint.ANY)

    self.adapter.reset()
    pc = self.adapter.run_to_breakpoint(2)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_PC_BKPT'],\
                    "DUT failed to stop at IEC60730_PC_BKPT" \
                    " , pc = " + hex(pc))

    pc = self.adapter.run_to_breakpoint(2)
    self.assertEqual(pc, asm_labels['IEC60730_PC_BKPT'], \
                      "DUT failed to stop at IEC60730_PC_BKPT a second time" + \
                      ", pc = " + hex(pc))

    logging.info("DUT correctly running Program counter test")

    return True


  @iec60730_test_case('TC: Enter SafeState when Program counter tests are not executing within period')
  ## Verify the DUT repeatedly executes Program counter test, indicating Program counter enters safe state.
  # @details Order of execution:
  #   - Verify code reaches breakpoint at IEC60730_PC_BKPT
  #   - Inject fault condition into Program counter flags
  #   - Verify code reaches breakpoint at iec60730_SafeState
  #     - Test passes, return True
  #
  # @return True
  #
  def test_program_counter_too_slow(self):
    programmeCounterCheckLocation = variables['sl_iec60730_program_counter_check']

    self.assertNotEqual(0, programmeCounterCheckLocation,\
                    "Can not read sl_iec60730_program_counter_check address")

    # Set breakpoints in system clock test-related ISRs.
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()

    self.adapter.set_breakpoint(address = asm_labels['IEC60730_PC_BKPT'],
                                 typeflags=enums.JLinkBreakpoint.ANY)



    self.adapter.reset()
    pc = self.adapter.run_to_breakpoint(2)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_PC_BKPT'],\
                    "DUT failed to stop at IEC60730_PC_BKPT" \
                    " , pc = " + hex(pc))

    self.adapter.clear_all_breakpoints()

    self.adapter.set_breakpoint(address = asm_labels['sl_iec60730_safe_state'],
                                 typeflags=enums.JLinkBreakpoint.ANY)

    # Clear bist execution completion flag bits
    self.adapter.memory_write16(addr = programmeCounterCheckLocation, data = {0x0000})

    read_value = self.adapter.memory_read16(addr = programmeCounterCheckLocation,num_halfwords=1)

    # Verify written data
    self.assertEqual(0, read_value[0], \
                      "Cannot clear BIST execution flag" + \
                      ", read value = " + str(read_value[0]))

    pc = self.adapter.run_to_breakpoint(2)
    self.assertEqual(pc, asm_labels['sl_iec60730_safe_state'], \
                      "DUT did not enter safe state after bist frequency fail" + \
                      ", pc = " + hex(pc))

    logging.info("DUT correctly entered safe state after Program counter check failure")

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
