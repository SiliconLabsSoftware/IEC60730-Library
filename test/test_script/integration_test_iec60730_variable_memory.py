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
# @defgroup IEC60730_VARIABLE_MEMORY_VERIFICATION Variable Memory Automated Verification Tests
# @{
# Python script for the IEC60730 Variable Memory plausibility verification tests.
#
# For details on the tests, see iec60730_variable_memory.iec60730_variable_memory.
#
#@section irq_test_results Test Results
#
#EFR32 Device| Test Results
#------------|-------------

from binascii import hexlify
from logging import debug, log
import random
import os
import sys
from time import sleep

from iec60730_test_base import *
import jlink, jlink_constants
import unittest
import enums

isEnableMarchXC = True

##  IEC60730 Variable Memory Plausibility Verification Tests
#
# @details This class runs IEC60730 verification tests for the variable memory plausibility test.
#
# Hardware setup:
#   Connect DUT to PC. No additional hardware setup required.

class iec60730_vmc(unittest.TestCase, iec60730TestBase):
  ## Text name of the test suite, used in XML output.
  TEST_SUITE_NAME = "VMC"

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

    logging.info("Run to bkp: " + label)
    pc = self.adapter.run_to_breakpoint(time)

    self.assertEqual(pc, asm_labels[label], \
                    "DUT failed to stop at " + label + \
                    ", pc = " + hex(pc) + \
                    ", setbkp = " + str(setbkp))
  ## Corruption changes the value at the breakpoint label
  def corruption(self,
                address = 0,
                value = 0,
                time = 1,
                label = '',
                label_pass = 'sl_iec60730_safe_state') :
    self.reach_to_breakpoint(label, True, time)
    # write data to address
    self.adapter.memory_write32(address, data = {value})
    logging.info("Write to location: " + hex(address) + ", bkp: " + label)
    temp = self.adapter.memory_read32(address, 1)
    logging.info("Read from location: " + hex(address) + ", val = " + hex(temp[0]))
    self.reach_to_breakpoint(label_pass, True, time)
    self.adapter.clear_all_breakpoints()
    self.adapter.reset()
    # self.reach_to_breakpoint(label, True, time)

  @iec60730_test_case('TC: Run step by step when testing Full Ram regions want to check')
  ## Verify the DUT run step by step when testing full ram at POST. This test
  # case (TC) checks under normal conditions with no exceptions, VMC module
  # DOES NOT raise issue.
  #
  # @details Order of execution
  # - Reset device
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_SAVE
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP1
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP2
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP3
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP4
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP5
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP6
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_RESTORE
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_MARCH_RET
  # - Verify code reaches breakpoint at IEC60730_VMC_POST_MARCHC_STEP_BKPT
  #
  # This process in loop with coditon \ref iec60730_rt_check < #RAMTEST_END. In case this
  # condition is not satisfied, clear all break point and verify
  #
  # - Verify code reaches breakpoint at IEC60730_VMC_POST_CHECK_BKBUF_BKPT
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF1
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF2
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF3
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF4
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF5
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF6
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_MARCH_RET
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_RET
  #
  # @return True
  #
  def test_correct_background_full_ram(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    self.adapter.reset()

    firstTime = True

    self.reach_to_breakpoint('IEC60730_VMC_POST_START_BKPT', True, 1)
    self.adapter.clear_all_breakpoints()

    iec60730_rt_check_location = variables['iec60730_rt_check']
    iec60730_rt_check = self.adapter.memory_read32(iec60730_rt_check_location, 1)

    phaseSteps = ['IEC60730_MARCHC_STEP_SAVE',
                  'IEC60730_MARCHC_STEP_STEP1',
                  'IEC60730_MARCHC_STEP_STEP2',
                  'IEC60730_MARCHC_STEP_STEP3',
                  'IEC60730_MARCHC_STEP_STEP4',
                  'IEC60730_MARCHC_STEP_STEP5',
                  'IEC60730_MARCHC_STEP_STEP6',
                  'IEC60730_MARCHC_STEP_RESTORE',
                  'IEC60730_MARCHC_STEP_MARCH_RET',
                  'IEC60730_VMC_POST_MARCHC_STEP_BKPT']

    num_ram_regions_check = self.adapter.memory_read32(variables['num_ram_regions_check'], 1)
    self.assertNotEqual(0, num_ram_regions_check[0],\
                    "Can not read num_ram_regions_check value")
    logging.info("Number regions test: " + str(num_ram_regions_check[0]))
    integration_test_vmc_region_test = variables['integration_test_vmc_region_test']
    self.assertNotEqual(0, integration_test_vmc_region_test,\
                    "Can not read integration_test_vmc_region_test address")
    #logging.info("Read integration_test_vmc_region_test value: " + hex(integration_test_vmc_region_test))
    region_test = self.adapter.memory_read32(integration_test_vmc_region_test, num_ram_regions_check[0]*2)
    self.assertNotEqual(0, region_test[0],\
                    "Can not read integration_test_vmc_region_test address")
    current_test_region = 0
    while current_test_region < num_ram_regions_check[0]:
      logging.info("Region " + str(current_test_region) + " -- start: " + hex(region_test[2*current_test_region])+
                  " end: " + hex(region_test[2*current_test_region+1]))
      current_test_region = current_test_region + 1

    # ram_test_end = self.adapter.memory_read32(variables['ram_test_end'], 1)
    # self.assertNotEqual(0, ram_test_end[0],\
    #                 "Can not read ram_test_end value")
    # logging.info("Read ram test end value: " + hex(ram_test_end[0]))

    current_test_region = 0
    while current_test_region < num_ram_regions_check[0]:
      iec60730_rt_check[0] = region_test[2*current_test_region]
      while iec60730_rt_check[0] < region_test[2*current_test_region+1]:
        logging.info(" iec60730_rt_check: " + hex(iec60730_rt_check[0]))
        for phase in phaseSteps :
          self.reach_to_breakpoint(phase, firstTime, 1)
        iec60730_rt_check = self.adapter.memory_read32(iec60730_rt_check_location, 1)
        firstTime = False
      self.adapter.clear_all_breakpoints()
      current_test_region = current_test_region + 1
      firstTime = True

    # self.adapter.clear_all_breakpoints()

    phaseBuffs = ['IEC60730_VMC_POST_CHECK_BKBUF_BKPT',
                  'IEC60730_MARCHC_STEP_BUFF1',
                  'IEC60730_MARCHC_STEP_BUFF2',
                  'IEC60730_MARCHC_STEP_BUFF3',
                  'IEC60730_MARCHC_STEP_BUFF4',
                  'IEC60730_MARCHC_STEP_BUFF5',
                  'IEC60730_MARCHC_STEP_BUFF6',
                  'IEC60730_MARCHC_STEP_MARCH_RET',
                  'IEC60730_MARCHC_STEP_RET']
    for phase in phaseBuffs :
      self.reach_to_breakpoint(phase, True, 1)

    return True

  @iec60730_test_case('TC: Detect corruption when testing Full Ram regions want to check')
  ## Verify the DUT jump to #sl_iec60730_safe_state when testing ram failed at
  # post. This TC simulates an abnormal situation that causes the \ref iec60730_rt_check,
  # and \ref iec60730_bk_buf variables to be changed. This leads to jump to break point at
  # #sl_iec60730_safe_state.
  #
  # In the VMC documentation there is a detailed description of the \ref iec60730_rt_check,
  # and \ref iec60730_bk_buf variables.
  #
  # @details Order of execution
  # - Reset device
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP2. We don't reach
  # IEC60730_MARCHC_STEP_STEP1 because this step only write (W1)
  #   - Edit value at address of \ref iec60730_rt_check
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP3
  #   - Edit value at address of \ref iec60730_rt_check
  #   + Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP4
  #   - Edit value at address of \ref iec60730_rt_check
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP5
  #   - Edit value at address of \ref iec60730_rt_check
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_STEP6
  #   - Edit value at address of \ref iec60730_rt_check
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  #
  # - Verify code reaches breakpoint at IEC60730_VMC_POST_CHECK_BKBUF_BKPT
  #
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF2. We don't reach
  # IEC60730_MARCHC_STEP_BUFF1 because this step only write (W1)
  #   - Edit value at address of \ref iec60730_bk_buf
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF3
  #   - Edit value at address of \ref iec60730_bk_buf
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF4
  #   - Edit value at address of \ref iec60730_bk_buf
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF5
  #   - Edit value at address of \ref iec60730_bk_buf
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHC_STEP_BUFF6
  #   - Edit value at address of \ref iec60730_bk_buf
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  #
  # @return True
  #
  def test_detect_corruption_full_ram(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    self.adapter.reset()

    self.reach_to_breakpoint('IEC60730_VMC_POST_START_BKPT', True, 1)
    self.adapter.clear_all_breakpoints()

    iec60730_rt_check_location = variables['iec60730_rt_check']

    phaseSteps = ['IEC60730_MARCHC_STEP_STEP2',
                  'IEC60730_MARCHC_STEP_STEP3',
                  'IEC60730_MARCHC_STEP_STEP4',
                  'IEC60730_MARCHC_STEP_STEP5',
                  'IEC60730_MARCHC_STEP_STEP6']

    for phase in phaseSteps :
      self.corruption(address = iec60730_rt_check_location,
                      value = 0xA5A5A5A5,
                      time = 1,
                      label = phase,
                      label_pass = 'sl_iec60730_safe_state')

    self.adapter.clear_all_breakpoints()
    self.adapter.reset()

    # TODO: Check test case again
    self.reach_to_breakpoint('IEC60730_VMC_POST_CHECK_BKBUF_BKPT', True, 1)
    self.adapter.clear_all_breakpoints()

    iec60730_bk_buf_location = 0
    iec60730_bk_buf_location = variables['iec60730_bk_buf']

    phaseBuffs = ['IEC60730_MARCHC_STEP_BUFF2',
                  'IEC60730_MARCHC_STEP_BUFF3',
                  'IEC60730_MARCHC_STEP_BUFF4',
                  'IEC60730_MARCHC_STEP_BUFF5',
                  'IEC60730_MARCHC_STEP_BUFF6']

    for phase in phaseBuffs :
      self.corruption(address = iec60730_bk_buf_location,
                      value = 0xA5A5A5A5,
                      time = 1,
                      label = phase,
                      label_pass = 'sl_iec60730_safe_state')

    return True

  @iec60730_test_case('TC: Run step by step when testing Step Ram first region')
  ## Verify the DUT run step by step when testing full ram at BIST. This TC
  # checks under normal conditions with no exceptions, vmc module DOES NOT raise
  # issue.
  #
  # @details Order of execution
  # - Reset device
  #
  # - Verify code reaches breakpoint at IEC60730_VMC_BIST_START_BKPT
  #
  # The loop with coditon \ref iec60730_rt_check < #RAMTEST_END
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_SAVE
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP1
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP2
  # - if not enable MatchX
  #   - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP3
  #   - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP4
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP5
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP6
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_RESTORE
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_MARCH_RET
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_RET
  # - Verify code reaches breakpoint at IEC60730_VMC_BIST_MARCHC_STEP_BKPT
  #
  # In case condition is not satisfied, clear all break point and verify
  #
  # - Verify code reaches breakpoint at IEC60730_IMC_BIST_CHECK_BKBUF_BKPT
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF1
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF2
  # - if not enable MatchX
  #   - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF3
  #   - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF4
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF5
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF6
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_MARCH_RET
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_RET
  #
  # @return True
  #
  def test_correct_background_step_ram(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    self.adapter.reset()

    firstTime = True

    self.reach_to_breakpoint('IEC60730_VMC_BIST_START_BKPT', True, 1)
    self.adapter.clear_all_breakpoints()

    iec60730_rt_check_location = variables['iec60730_rt_check']
    iec60730_rt_check = self.adapter.memory_read32(iec60730_rt_check_location, 1)

    phaseSteps = []
    if isEnableMarchXC :
      phaseSteps = ['IEC60730_MARCHXC_STEP_SAVE',
                    'IEC60730_MARCHXC_STEP_STEP1',
                    'IEC60730_MARCHXC_STEP_STEP2',
                    'IEC60730_MARCHXC_STEP_STEP5',
                    'IEC60730_MARCHXC_STEP_STEP6',
                    'IEC60730_VMC_BIST_MARCHC_STEP_BKPT']
    else :
      phaseSteps = ['IEC60730_MARCHXC_STEP_SAVE',
                    'IEC60730_MARCHXC_STEP_STEP1',
                    'IEC60730_MARCHXC_STEP_STEP2',
                    'IEC60730_MARCHXC_STEP_STEP3',
                    'IEC60730_MARCHXC_STEP_STEP4',
                    'IEC60730_MARCHXC_STEP_STEP5',
                    'IEC60730_MARCHXC_STEP_STEP6',
                    'IEC60730_VMC_BIST_MARCHC_STEP_BKPT']

    num_ram_regions_check = self.adapter.memory_read32(variables['num_ram_regions_check'], 1)
    self.assertNotEqual(0, num_ram_regions_check[0],\
                    "Can not read num_ram_regions_check value")
    logging.info("Number regions test: " + str(num_ram_regions_check[0]))
    integration_test_vmc_region_test = variables['integration_test_vmc_region_test']
    self.assertNotEqual(0, integration_test_vmc_region_test,\
                    "Can not read integration_test_vmc_region_test address")
    #logging.info("Read integration_test_vmc_region_test value: " + hex(integration_test_vmc_region_test))
    region_test = self.adapter.memory_read32(integration_test_vmc_region_test, num_ram_regions_check[0]*2)
    self.assertNotEqual(0, region_test[0],\
                    "Can not read integration_test_vmc_region_test address")
    current_test_region = 0
    while current_test_region < num_ram_regions_check[0]:
      logging.info("Region " + str(current_test_region) + " -- start: " + hex(region_test[2*current_test_region])+
                  " end: " + hex(region_test[2*current_test_region+1]))
      current_test_region = current_test_region + 1

    # ram_test_end = self.adapter.memory_read32(variables['ram_test_end'], 1)
    # self.assertNotEqual(0, ram_test_end[0],\
    #                 "Can not read ram_test_end value")
    # logging.info("Read ram test end value: " + hex(ram_test_end[0]))

    # logging.info("Read ram test end value: " + hex(ram_test_end[0]))
    # while iec60730_rt_check[0] < ram_test_end[0] :
    #   logging.info("TC: iec60730_rt_check: " + hex(iec60730_rt_check[0]))
    #   for phase in phaseSteps :
    #     self.reach_to_breakpoint(phase, firstTime, 1)
    #   iec60730_rt_check = self.adapter.memory_read32(iec60730_rt_check_location, 1)
    #   firstTime = False

    current_test_region = 0
    while current_test_region < num_ram_regions_check[0]:
      iec60730_rt_check[0] = region_test[2*current_test_region]
      while iec60730_rt_check[0] < region_test[2*current_test_region+1]:
        logging.info(" iec60730_rt_check: " + hex(iec60730_rt_check[0]))
        for phase in phaseSteps :
          self.reach_to_breakpoint(phase, firstTime, 1)
        #iec60730_rt_check[0] = iec60730_rt_check[0]+8
        iec60730_rt_check = self.adapter.memory_read32(iec60730_rt_check_location, 1)
        firstTime = False
      self.adapter.clear_all_breakpoints()
      current_test_region = current_test_region + 1
      firstTime = True

    self.adapter.clear_all_breakpoints()

    logging.info("Testing VMC the backup Buffer")
    phaseBuffs = []
    if isEnableMarchXC :
      phaseBuffs = ['IEC60730_IMC_BIST_CHECK_BKBUF_BKPT',
                    'IEC60730_MARCHXC_STEP_BUFF1',
                    'IEC60730_MARCHXC_STEP_BUFF2',
                    'IEC60730_MARCHXC_STEP_BUFF5',
                    'IEC60730_MARCHXC_STEP_BUFF6',
                    'IEC60730_MARCHXC_STEP_MARCH_RET',
                    'IEC60730_MARCHXC_STEP_RET']
    else :
      phaseBuffs = ['IEC60730_IMC_BIST_CHECK_BKBUF_BKPT',
                    'IEC60730_MARCHXC_STEP_BUFF1',
                    'IEC60730_MARCHXC_STEP_BUFF2',
                    'IEC60730_MARCHXC_STEP_BUFF3',
                    'IEC60730_MARCHXC_STEP_BUFF4',
                    'IEC60730_MARCHXC_STEP_BUFF5',
                    'IEC60730_MARCHXC_STEP_BUFF6',
                    'IEC60730_MARCHXC_STEP_MARCH_RET',
                    'IEC60730_MARCHXC_STEP_RET']
    for phase in phaseBuffs :
      self.reach_to_breakpoint(phase, True, 1)

    return True

  @iec60730_test_case('TC: Detect corruption when testing Step Ram first region')
  ## Verify the DUT jump to #sl_iec60730_safe_state when testing ram failed at
  # BIST. This TC simulates an abnormal situation that causes the variable iec60730_rt_check,
  # iec60730_bk_buf to be changed. This leads to jump to break point at #sl_iec60730_safe_state.
  #
  # In the VMC documentation there is a detailed description of the \ref iec60730_rt_check,
  # and \ref iec60730_bk_buf variables.
  #
  # @details Order of execution
  # - Reset device
  #
  # - Verify code reaches breakpoint at IEC60730_VMC_BIST_START_BKPT
  #
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP2. We don't reach
  # IEC60730_MARCHXC_STEP_STEP1 because this step only write (W1)
  #   - Edit value at address of \ref iec60730_rt_check
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - if not enable MatchX
  #   - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP3
  #     - Edit value at address of \ref iec60730_rt_check
  #     - Verify code reaches breakpoint at #sl_iec60730_safe_state
  #   - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP4
  #     - Edit value at address of \ref iec60730_rt_check
  #     - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP5
  #   - Edit value at address of \ref iec60730_rt_check
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_STEP6
  #   - Edit value at address of \ref iec60730_rt_check
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  #
  # - Verify code reaches breakpoint at IEC60730_IMC_BIST_CHECK_BKBUF_BKPT
  #
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF2. We don't reach
  # IEC60730_MARCHXC_STEP_BUFF1 because this step only write (W1)
  #   - Edit value at address of \ref iec60730_bk_buf
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - if not enable MatchX
  #   - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF3
  #     - Edit value at address of \ref iec60730_bk_buf
  #     - Verify code reaches breakpoint at #sl_iec60730_safe_state
  #   - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF4
  #     - Edit value at address of \ref iec60730_bk_buf
  #     - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF6
  #   - Edit value at address of \ref iec60730_bk_buf
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  # - Verify code reaches breakpoint at IEC60730_MARCHXC_STEP_BUFF6
  #   - Edit value at address of \ref iec60730_bk_buf
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  #
  # @return True
  #
  def test_detect_corruption_step_ram(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    self.adapter.reset()

    self.reach_to_breakpoint('IEC60730_VMC_BIST_START_BKPT', True, 1)
    self.adapter.clear_all_breakpoints()

    iec60730_rt_check_location = variables['iec60730_rt_check']

    phaseSteps = []
    if isEnableMarchXC :
      phaseSteps = ['IEC60730_MARCHXC_STEP_STEP2',
                    'IEC60730_MARCHXC_STEP_STEP5',
                    'IEC60730_MARCHXC_STEP_STEP6']
    else :
      phaseSteps = ['IEC60730_MARCHXC_STEP_STEP2',
                    'IEC60730_MARCHXC_STEP_STEP3',
                    'IEC60730_MARCHXC_STEP_STEP4',
                    'IEC60730_MARCHXC_STEP_STEP5',
                    'IEC60730_MARCHXC_STEP_STEP6']

    for phase in phaseSteps :
      self.corruption(address = iec60730_rt_check_location,
                      value = 0xA5A5A5A5,
                      time = 1,
                      label = phase,
                      label_pass = 'sl_iec60730_safe_state')

    self.adapter.clear_all_breakpoints()
    self.adapter.reset()

    self.reach_to_breakpoint('IEC60730_IMC_BIST_CHECK_BKBUF_BKPT', True, 1)
    self.adapter.clear_all_breakpoints()

    iec60730_bk_buf_location = 0
    iec60730_bk_buf_location = variables['iec60730_bk_buf']

    phaseBuffs = []
    if isEnableMarchXC :
      phaseBuffs = ['IEC60730_MARCHXC_STEP_BUFF2',
                    'IEC60730_MARCHXC_STEP_BUFF5',
                    'IEC60730_MARCHXC_STEP_BUFF6']
    else :
      phaseBuffs = ['IEC60730_MARCHXC_STEP_BUFF2',
                    'IEC60730_MARCHXC_STEP_BUFF3',
                    'IEC60730_MARCHXC_STEP_BUFF4',
                    'IEC60730_MARCHXC_STEP_BUFF5',
                    'IEC60730_MARCHXC_STEP_BUFF6']

    for phase in phaseBuffs :
      self.corruption(address = iec60730_bk_buf_location,
                      value = 0xA5A5A5A5,
                      time = 1,
                      label = phase,
                      label_pass = 'sl_iec60730_safe_state')

    return True

  @iec60730_test_case('TC: Stack Test work normally')
  ## Verify the DUT run to check RAM and not failed at BIST. This TC checks
  # under normal conditions with no exceptions, vmc module DOES NOT raise issue.
  #
  # @details Order of execution
  # - Reset device
  #
  # - Verify code reaches breakpoint at IEC60730_VMC_BIST_START_BKPT
  # - Verify code reaches breakpoint at IEC60730_VMC_BIST_STACK_TEST_OK_BKPT
  #
  # @return True
  #
  def test_work_normally_stack_test(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    self.adapter.reset()

    self.reach_to_breakpoint('IEC60730_VMC_BIST_START_BKPT', True, 1)
    self.adapter.clear_all_breakpoints()

    phaseBuffs = ['IEC60730_VMC_BIST_STACK_TEST_BKPT',
                  'IEC60730_VMC_BIST_STACK_TEST_OK_BKPT' ]

    for phase in phaseBuffs :
      self.reach_to_breakpoint(phase, True, 1)

    return True

  @iec60730_test_case('TC: Stack Test failed')
  ## Verify the DUT jump to #sl_iec60730_safe_state when stack test failed at
  # BIST. This TC simulates an abnormal situation that causes the variable
  # \ref iec60730_stack_test_over_flow to be changed. This leads to jump to break point at
  # #sl_iec60730_safe_state.
  #
  # In the VMC documentation there is a detailed description of the variable
  # \ref iec60730_stack_test_over_flow.
  #
  # @details Order of execution
  # - Reset device
  #
  # - Verify code reaches breakpoint at IEC60730_VMC_BIST_START_BKPT
  #   - Edit value at address of \ref iec60730_stack_test_over_flow
  #   - Verify code reaches breakpoint at #sl_iec60730_safe_state
  #
  # @return True
  #
  def test_detect_failed_stack_test(self):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()
    self.adapter.reset()

    self.reach_to_breakpoint('IEC60730_VMC_BIST_START_BKPT', True, 1)
    self.adapter.clear_all_breakpoints()

    # Edit variable iec60730_stack_test_over_flow
    iec60730_stack_test_over_flow_location = 0
    iec60730_stack_test_over_flow_location = variables['iec60730_stack_test_over_flow']

    for i in range(4):
      iec60730_stack_test_over_flow = self.adapter.memory_read32(iec60730_stack_test_over_flow_location + 4 * i, 1)
      logging.info("DUT iec60730_stack_test_over_flow[" + str(i) + "]=" + hex(iec60730_stack_test_over_flow[0]))
      self.corruption(address = iec60730_stack_test_over_flow_location + 4 * i,
                      value = 0x00000000,
                      time = 1,
                      label = 'IEC60730_VMC_BIST_STACK_TEST_BKPT',
                      label_pass = 'sl_iec60730_safe_state')

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

  ## Enable test with MARCH-X algorithm
  use_marchx_enable = os.getenv('INTEGRATION_TEST_USE_MARCHX_DISABLE')
  if not use_marchx_enable:
    use_marchx_enable = "enable"
  else:
    if use_marchx_enable == "disable":
      ## Disallowance  testing with MARCH-X algorithm
      isEnableMarchXC = False
  print("Using MarchXC algorithm: "+use_marchx_enable)

  while len(sys.argv) > 1:
    ## Number of arguments passed into the script file
    line = sys.argv.pop()
    if len(sys.argv) == 1:
      ## compiler creates the file *.lst
      compiler = line

  unittest.main()

## @}
# @}
