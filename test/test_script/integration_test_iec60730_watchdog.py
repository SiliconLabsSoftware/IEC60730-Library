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
# @defgroup IEC60730_WATCHDOG_VERIFICATION Watchdog Automated Verification Tests
# @{
# Python script for the IEC60730 Watchdog timer Verification tests.
#
# For details on the tests, see iec60730_watchdog.iec60730_watchdog.
#
#@section watchdog_test_results Test Results
#
#EFR32 Device | Test Results
#-------------|-------------


from binascii import hexlify
import random
import os
import sys
from time import sleep
from util import device_target_power_on_off

from iec60730_test_base import *
import jlink, jlink_constants
import unittest
import enums

def int_to_bytes(number: int) -> bytes:
    return number.to_bytes(length=(8 + (number + (number < 0)).bit_length()) // 8, byteorder='big', signed=True)

## IP address of device
hostIP = os.getenv('HOST_IP')

##  IEC60730 Watchdog Verification Tests
#
# @details This class runs IEC60730 verification tests for the watchdog.
#
# Hardware setup:
#   Connect DUT to PC. No additional hardware setup required.

class iec60730_watchdog(unittest.TestCase, iec60730TestBase):
  ## Text name of the test suite, used in XML output.
  TEST_SUITE_NAME = "Watchdog"

  ## Set up connect device.
  def setUp(self):
    self.env_setup(adapter_serial_no, chip_name, lst_file_path, lib_path, compiler)

  ## Clear reset cause register.
  def clear_rst_causes(self):
    logging.info("Clear RSTCAUSES register.")
    rstcauses_register = self.adapter.memory_read32(variables['rstcauses_register'], 1)
    self.assertNotEqual(0, rstcauses_register[0],\
                    "Can not read rstcauses_register value")
    logging.info("Read RSTCAUSES Register at address: " + hex(rstcauses_register[0]))
    rstcauses_clear = self.adapter.memory_read32(variables['rstcauses_clear'], 1)
    self.assertNotEqual(0, rstcauses_clear[0],\
                    "Can not read rstcauses_clear value")
    logging.info("Read bit mask value uses to clear reset cause: " + hex(rstcauses_clear[0]))
    reg = self.adapter.memory_write32(rstcauses_register[0], data = [rstcauses_clear[0]], zone=None)

  ## Allows to start running test watchdog.
  def wdog_test_running_enable(self):
    pc = self.adapter.run_to_breakpoint(20)
    logging.info("Halted at label: " + self.get_label(pc))

    watchdogStateLocation = variables['wdog_por_wait']
    self.assertNotEqual(0, watchdogStateLocation,\
                    "Can not read wdog_por_wait address")
    logging.info("Set wdog_por_wait flag at address: " + hex(watchdogStateLocation))
    reg = self.adapter.memory_write32(watchdogStateLocation, data = [1], zone=None)

  ## Reset watchdog by power on by sending command to hostIP.
  def wdog_power_cycle(self):
    self.adapter_close()

    device_target_power_on_off(hostIP, "off")
    print("Target power off.")
    print("Waiting...")
    sleep(2)
    device_target_power_on_off(hostIP, "on")
    print("Target power on.")
    sleep(1)

    self.re_connect_mcu()

  @iec60730_test_case('TC: Watchdog Power On Self Test')
  ## Verify the DUT follows the watchdog timer POST test logic correctly.
  # @details Order of execution:
  #   - Verify code reaches breakpoint at IEC60730_WATCHDOG_POST_POR_RESET_BKPT
  #   - Verify code reaches breakpoint at IEC60730_WATCHDOG_POST_TIMEOUT_BKPT
  #   - Verify code reaches breakpoint at IEC60730_WATCHDOG_POST_TESTING_BKPT
  #   - Verify code reaches breakpoint at IEC60730_WATCHDOG_POST_END_BKPT
  #     - Verify the Watchdog status is SL_IEC60730_WATCHDOG_VALID
  #     - Verify the Watchdog status saved to BURAM is SL_IEC60730_WATCHDOG_VALID (On EFR32 series 2 only)
  #     - Code correctly tested watchdog timer and watchdog reset trigger,
  #       enters main loop
  #     - Test passes, return True
  #
  # @return True
  #
  def test_tc1_watchdog_por_execution(self):
    self.wdog_power_cycle()
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    bp1 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_WATCHDOG_POST_POR_RESET_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp2 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_WATCHDOG_POST_TIMEOUT_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp3 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_WATCHDOG_POST_TESTING_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp4 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_WATCHDOG_POST_END_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp5 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_TEST_WDOG_POR_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)

    # Reset device
    self.adapter.reset()
    self.wdog_test_running_enable()

    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_WATCHDOG_POST_POR_RESET_BKPT'],\
                    "DUT failed to enter testing state" \
                    " , pc = " + hex(pc))

    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_WATCHDOG_POST_TIMEOUT_BKPT'],\
                    "DUT failed to trigger watchdog 0 timeout" \
                    " , pc = " + hex(pc))

    self.wdog_test_running_enable()
    pc = self.adapter.run_to_breakpoint(3)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_WATCHDOG_POST_TESTING_BKPT'],\
                    "DUT failed to enter pass result status" \
                    " , pc = " + hex(pc))

    # Second Watchdog
    if wdog1_present == "enable":
        pc = self.adapter.run_to_breakpoint(3)
        logging.info("Halted at label: " + self.get_label(pc))
        self.assertEqual(pc, asm_labels['IEC60730_WATCHDOG_POST_TIMEOUT_BKPT'],\
                        "DUT failed to trigger watchdog 1 timeout" \
                        " , pc = " + hex(pc))
        self.wdog_test_running_enable()
        pc = self.adapter.run_to_breakpoint(3)
        logging.info("Halted at label: " + self.get_label(pc))
        self.assertEqual(pc, asm_labels['IEC60730_WATCHDOG_POST_TESTING_BKPT'],\
                        "DUT failed to enter pass result status" \
                        " , pc = " + hex(pc))

    pc = self.adapter.run_to_breakpoint(7)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_WATCHDOG_POST_END_BKPT'],\
                    "DUT failed to finish POR test" \
                    " , pc = " + hex(pc))

    watchdogStateLocation = variables['iec60730_watchdog_state']
    self.assertNotEqual(0, watchdogStateLocation,\
                    "Can not read iec60730_watchdog_state address")
    logging.info("Read mem at address: " + hex(watchdogStateLocation))
    reg = self.adapter.memory_read8(watchdogStateLocation, 1, zone=None)

    # State must be SL_IEC60730_WATCHDOG_VALID (2)
    self.assertEqual(reg[0], 2, "DUT failed in POR completed state")

    if -1 != chip_name.find("G2"):
      # Check BURAM
      buram_addr = self.adapter.memory_read32(variables['buram_addr'], 1)
      logging.info("Read BURAM Register at address: " + hex(buram_addr[0]))
      reg = self.adapter.read_ram_arm_32(buram_addr[0], num_words = 1)
      # State must be SL_IEC60730_WATCHDOG_VALID (2)
      self.assertEqual(reg[0], 0x2, "DUT failed to save state")

    logging.info("DUT correctly running the watchdog POR test.")
    return True

  @iec60730_test_case('TC: Watchdog internal counter failed')
  ## Verify the DUT follows the watchdog timer POST test logic correctly.
  # @details Order of execution:
  #   - Verify code reaches breakpoint at IEC60730_WATCHDOG_POST_TESTING_BKPT
  #   - Verify code reaches breakpoint at IEC60730_SAFE_STATE_BKPT
  #     - Code correctly enters safe state
  #     - Test passes, return True
  #
  # @return True
  #
  def test_tc2_watchdog_counter_execution(self):
    self.wdog_power_cycle()
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    bp1 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_WATCHDOG_POST_TESTING_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp2 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_SAFE_STATE_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp3 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_TEST_WDOG_POR_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)

    # Reset device
    self.adapter.reset()
    self.wdog_test_running_enable()

    # Enable again after first wdog reset
    self.wdog_test_running_enable()

    pc = self.adapter.run_to_breakpoint(4)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_WATCHDOG_POST_TESTING_BKPT'],\
                    "DUT failed to enter pass result status" \
                    " , pc = " + hex(pc))

    WatchdogCount_loc = variables['iec60730_watchdog_count']
    self.assertNotEqual(0, WatchdogCount_loc,\
                    "Can not read iec60730_watchdog_count address")
    logging.info("Read mem at address: " + hex(WatchdogCount_loc))
    reg = self.adapter.memory_read8(WatchdogCount_loc, 1, zone=None)
    reg[0] += 5
    reg = self.adapter.memory_write8(WatchdogCount_loc, data =  (reg), zone=None)

    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_SAFE_STATE_BKPT'],\
                    "DUT failed to safe state" \
                    " , pc = " + hex(pc))

    logging.info("DUT correctly enter safe state when internal counter failed.")
    return True

  @iec60730_test_case('TC: Watchdog wrong reset cause')
  ## Verify the DUT follows the watchdog timer POST test logic correctly.
  # @details Order of execution:
  #   - Verify code reaches breakpoint at IEC60730_WATCHDOG_POST_TESTING_BKPT
  #   - Verify code reaches breakpoint at IEC60730_SAFE_STATE_BKPT
  #     - Code correctly enters safe state
  #     - Test passes, return True
  #
  # @return True
  #
  def test_tc3_watchdog_wrong_rst_execution(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    bp1 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_WATCHDOG_POST_TESTING_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp2 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_SAFE_STATE_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp5 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_TEST_WDOG_POR_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)

    # Reset device
    self.adapter.reset()
    self.wdog_test_running_enable()

    pc = self.adapter.run_to_breakpoint(4)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_WATCHDOG_POST_TESTING_BKPT'],\
                    "DUT failed to enter pass result status" \
                    " , pc = " + hex(pc))

    # Clear Watchdog reset flag
    self.clear_rst_causes()

    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_SAFE_STATE_BKPT'],\
                    "DUT failed to safe state" \
                    " , pc = " + hex(pc))


    logging.info("DUT correctly enter safe state when wrong reset cause detected.")
    return True

  @iec60730_test_case('TC: Watchdog wrong testing state')
  ## Verify the DUT follows the watchdog timer POST test logic correctly.
  # @details Order of execution:
  #   - Verify code reaches breakpoint at IEC60730_WATCHDOG_POST_WORKING_BKPT
  #   - Verify code reaches breakpoint at IEC60730_SAFE_STATE_BKPT
  #     - Code correctly enters safe state
  #     - Test passes, return True
  #
  # @return True
  #
  def test_tc4_watchdog_wrong_testing_execution(self):
    self.wdog_power_cycle()
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    bp1 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_WATCHDOG_POST_WORKING_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp2 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_SAFE_STATE_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp3 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_TEST_WDOG_POR_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)

    # Reset device
    self.adapter.reset()
    self.wdog_test_running_enable()

    # Enable again after first wdog reset
    self.wdog_test_running_enable()

    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_WATCHDOG_POST_WORKING_BKPT'],\
                    "DUT failed to enter pass result status" \
                    " , pc = " + hex(pc))

    watchdogStateLocation = variables['iec60730_watchdog_state']
    self.assertNotEqual(0, watchdogStateLocation,\
                    "Can not read iec60730_watchdog_state address")
    logging.info("Read mem at address: " + hex(watchdogStateLocation))
    reg = self.adapter.memory_read8(watchdogStateLocation, 1, zone=None)
    # Reset state value
    reg[0] = 0
    reg = self.adapter.memory_write8(watchdogStateLocation, data =  (reg), zone=None)

    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_SAFE_STATE_BKPT'],\
                    "DUT failed to safe state" \
                    " , pc = " + hex(pc))


    logging.info("DUT correctly enter safe state when wrong testing state detected.")
    return True

  @iec60730_test_case('TC: Watchdog reset when not restarted')
  ## Verify the DUT follows the watchdog timer POST test logic correctly.
  # @details Order of execution:
  #   - Verify code reaches breakpoint at IEC60730_TEST_RESTART_WDOG_BKPT
  #   - Verify code reaches breakpoint at IEC60730_SAFE_STATE_BKPT
  #     - Code correctly enters safe state
  #     - Test passes, return True
  #
  # @return True
  #
  def test_tc5_restart_watchdog_execution(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    bp1 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_TEST_RESTART_WDOG_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp2 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_SAFE_STATE_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)
    bp3 = self.adapter.set_breakpoint(address = asm_labels['IEC60730_TEST_WDOG_POR_BKPT'],
                                      typeflags=enums.JLinkBreakpoint.ANY)

    # Reset device
    self.adapter.reset()
    # Clear Watchdog reset flags
    self.clear_rst_causes()

    self.wdog_test_running_enable()

    pc = self.adapter.run_to_breakpoint(4)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_TEST_RESTART_WDOG_BKPT'],\
                    "DUT failed to enter watchdog restart" \
                    " , pc = " + hex(pc))

    wdog_restart_loc = variables['wdog_restart_enable']
    self.assertNotEqual(0, wdog_restart_loc,\
                    "Can not read wdog_restart_enable address")
    logging.info("Read mem at address: " + hex(wdog_restart_loc))
    # Disable watchdog restart
    reg = self.adapter.memory_write8(wdog_restart_loc, data = [0], zone=None)

    self.wdog_test_running_enable()
    pc = self.adapter.run_to_breakpoint(1)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels['IEC60730_SAFE_STATE_BKPT'],\
                    "DUT failed to safe state" \
                    " , pc = " + hex(pc))

    # Clear Watchdog reset flags
    self.clear_rst_causes()

    logging.info("DUT correctly enter safe state after watchdog reset when not restarted.")
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

  ## Enable test watchdog 1
  wdog1_present = os.getenv('INTEGRATION_TEST_WDOG1_ENABLE')
  if not wdog1_present:
    wdog1_present = "disable"
  print("Watchdog 1 testing: "+wdog1_present)

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
