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
# @defgroup IEC60730_CPU_REGISTERS_VERIFICATION CPU Register Check Automated Verification Tests
# @{
# Python script for the IEC60730 CPU Register Check Verification tests.
#
# For details on the tests, see iec60730_cpu_registers.iec60730_cpu_regs.
#
#@section cpu_registers_test_results Test Results
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


##  IEC60730 CPU Register Check Tests
#
# @details This class runs IEC60730 CPU Register tests.
#
# Hardware setup:
#   Connect DUT to PC. No additional hardware setup required.

class iec60730_cpu_regs(unittest.TestCase, iec60730TestBase):
  ## Text name of the test suite, used in XML output.
  TEST_SUITE_NAME = "CPU_REGS"

  ## Set up connect device.
  def setUp(self):
    self.env_setup(adapter_serial_no, chip_name, lst_file_path, lib_path, compiler)

  ## Supports setting breakpoints and changing test register values
  def core_register_common_test(self,
                                bkp_label_inject,
                                bkp_testcase_pass,
                                bkp_fail,
                                reg_test,
                                reg_mask,
                                reg_new_value = [0]):
    self.adapter.halt()
    self.adapter.clear_all_breakpoints()

    # if compiler == "IAR" and ("IEC60730" in bkp_label_inject) :
      # bkp_label_inject = "??" + bkp_label_inject

    # if compiler == "IAR" and ("IEC60730" in bkp_fail) :
      # bkp_fail = "??" + bkp_fail

    # if compiler == "IAR" and ("IEC60730" in bkp_testcase_pass) :
      # bkp_testcase_pass = "??" + bkp_testcase_pass

    self.adapter.set_breakpoint(address = asm_labels[bkp_label_inject],
                                typeflags=enums.JLinkBreakpoint.ANY)

    self.adapter.set_breakpoint(address = asm_labels[bkp_fail],
                                typeflags=enums.JLinkBreakpoint.ANY)

    self.adapter.set_breakpoint(address = asm_labels[bkp_testcase_pass],
                                typeflags=enums.JLinkBreakpoint.ANY)

    self.adapter.reset()
    pc = self.adapter.run_to_breakpoint(0.5)
    logging.info("Halted at label: " + self.get_label(pc))
    self.assertEqual(pc, asm_labels[bkp_label_inject],\
                    "DUT failed to stop at " + bkp_label_inject + ", " + hex(asm_labels[bkp_label_inject]) +\
                    " , pc = " + hex(pc))

    reg_value = self.adapter.register_read(reg_test)
    logging.info("Old value of " + reg_test + ": " + str(int_to_bytes(reg_value)))

    if reg_mask != 0:
      self.adapter.register_write(reg_test, reg_mask & reg_value)
    else:
      for i in reg_new_value:
        self.adapter.register_write(reg_test, i)

    reg_value = self.adapter.register_read(reg_test)
    logging.info("New value of " + reg_test + ": " + str(int_to_bytes(reg_value)))

    pc = self.adapter.run_to_breakpoint(0.1)
    logging.info("Halted at label: " + self.get_label(pc) + " " + hex(pc))

    self.assertEqual(pc, asm_labels[bkp_testcase_pass], \
                      "DUT failed to stop at " + bkp_testcase_pass + \
                      ", pc = " + hex(pc))

    logging.info("DUT successfully detected register " + reg_test + " stuck-at")

    return True



  @iec60730_test_case('TC: Verify Core Registers APSR, R0-R2, LR')
  ## Verify the error detected correctly on the Core Registers APSR, R0-R2, LR.
  # @details Order of execution:
  #   - Verify error detecting on APSR
  #   - Verify error detecting on R0-R2
  #   - Verify error detecting on R2-R13
  #   - Verify error detecting on LR
  #   - Test passes, return True
  #
  # @return True
  #
  def test_01_core_register_apsr(self):
    self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_CORE_Z_FLAG_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_CORE_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT',
                                          reg_test = 'APSR',
                                          reg_mask = 0xBFFFFFFF)


    self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_CORE_N_FLAG_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_CORE_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT',
                                          reg_test = 'APSR',
                                          reg_mask = 0x7FFFFFFF)

    self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_CORE_C_FLAG_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_CORE_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT',
                                          reg_test = 'APSR',
                                          reg_mask = 0xDFFFFFFF)

    self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_CORE_V_FLAG_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_CORE_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT',
                                          reg_test = 'APSR',
                                          reg_mask = 0xEFFFFFFF)


    write_value = [0xAAAAAAAB]

    for i in range(2):
      logging.info("Testing register R" + str(i))
      fp_reg = 'R' + str(i)
      bkp__inject = 'IEC60730_CPU_REGS_CORE_R' + str(i) + '_ASM_BKPT'

      self.core_register_common_test(bkp_label_inject = bkp__inject,
                                            bkp_testcase_pass = 'IEC60730_CPU_REGS_CORE_FAIL',
                                            bkp_fail = 'IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT',
                                            reg_test = fp_reg,
                                            reg_mask = 0,
                                            reg_new_value = write_value)

    for i in range(2,13):
      logging.info("Testing register R" + str(i))
      fp_reg = 'R' + str(i)
      bkp__inject = 'IEC60730_CPU_REGS_CORE_R' + str(i) + '_ASM_BKPT'

      result = self.core_register_common_test(bkp_label_inject = bkp__inject,
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_CORE_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT',
                                          reg_test = fp_reg,
                                          reg_mask = 0,
                                          reg_new_value = write_value)
      if result != True:
        return result

    self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_CORE_LR_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_CORE_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_CORE_PASS_ASM_BKPT',
                                          reg_test = 'R14',
                                          reg_mask = 0,
                                          reg_new_value = write_value)

    return True

  @iec60730_test_case('TC: Verify MSP_NS')
  ## Verify the error detected correctly on the Core Registers MSP_NS.
  # @details Order of execution:
  #   - Verify error detecting on MSP_NS
  #   - Test passes, return True
  #
  # @return True
  #
  def test_02_msp_ns_register(self):
    if -1 != chip_name.find("G2"):

        write_value = [0xAAAAAAA9, 0xAAAAAAAA, 0xAAAAAAAB, 0xAAAAAAAC, 0xAAAAAAAD]
        return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_MSP_NS_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_FAIL_ENDLESS_LOOP_MSP_NS',
                                              bkp_fail = 'IEC60730_CPU_REGS_MSP_NS_PASS_ASM_BKPT',
                                              reg_test = 'MSP_NS',
                                              reg_mask = 0,
                                              reg_new_value = write_value)
    else:
        logging.info("Device does not support secure extension. Test case is passed by default.")
        return True


  @iec60730_test_case('TC: Verify MSP_S')
  ## Verify the error detected correctly on the Register MSP_S.
  # @details Order of execution:
  #   - Verify error detecting on MSP_S
  #   - Test passes, return True
  #
  # @return True
  #
  def test_03_msp_s_register(self):

    write_value = [0xAAAAAAA9, 0xAAAAAAAA, 0xAAAAAAAB, 0xAAAAAAAC, 0xAAAAAAAD]
    return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_MSP_S_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_FAIL_ENDLESS_LOOP_MSP_S',
                                          bkp_fail = 'IEC60730_CPU_REGS_MSP_S_PASS_ASM_BKPT',
                                          reg_test = 'MSP',
                                          reg_mask = 0,
                                          reg_new_value = write_value)


  @iec60730_test_case('TC: Verify PSP_NS')
  ## Verify the error detected correctly on the Register PSP_NS.
  # @details Order of execution:
  #   - Verify error detecting on PSP_NS
  #   - Test passes, return True
  #
  # @return True
  #
  def test_04_psp_ns_register(self):
    if -1 != chip_name.find("G2"):

        write_value = [0xAAAAAAA9, 0xAAAAAAAA, 0xAAAAAAAB, 0xAAAAAAAC, 0xAAAAAAAD]
        return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_PSP_NS_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_FAIL_ENDLESS_LOOP_PSP_NS',
                                              bkp_fail = 'IEC60730_CPU_REGS_PSP_NS_PASS_ASM_BKPT',
                                              reg_test = 'PSP_NS',
                                              reg_mask = 0,
                                              reg_new_value = write_value)
    else:
        logging.info("Device does not support secure extension. Test case is passed by default.")
        return True

  @iec60730_test_case('TC: Verify PSP_S')
  ## Verify the error detected correctly on the Register PSP_S.
  # @details Order of execution:
  #   - Verify error detecting on PSP_S
  #   - Test passes, return True
  #
  # @return True
  #
  def test_05_psp_s_register(self):

    write_value = [0xAAAAAAA9, 0xAAAAAAAA, 0xAAAAAAAB, 0xAAAAAAAC, 0xAAAAAAAD]
    return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_PSP_S_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_FAIL_ENDLESS_LOOP_PSP_S',
                                          bkp_fail = 'IEC60730_CPU_REGS_PSP_S_PASS_ASM_BKPT',
                                          reg_test = 'PSP',
                                          reg_mask = 0,
                                          reg_new_value = write_value)


  @iec60730_test_case('TC: Verify MSPLIM_NS')
  ## Verify the error detected correctly on the Register MSPLIM_NS.
  # @details Order of execution:
  #   - Verify error detecting on MSPLIM_NS
  #   - Test passes, return True
  #
  # @return True
  #
  def test_06_msplim_ns_register(self):
    if -1 != chip_name.find("G2"):

        write_value = [0xAAAAAAA9, 0xAAAAAAAA, 0xAAAAAAAB, 0xAAAAAAAC, 0xAAAAAAAD, \
                        0xAAAAAAAE, 0xAAAAAAAF, 0xAAAAAAB0, 0xAAAAAAB1, 0xAAAAAAB2]
        return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_MSPLIM_NS_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_NS',
                                              bkp_fail = 'IEC60730_CPU_REGS_MSPLIM_NS_PASS_ASM_BKPT',
                                              reg_test = 'MSPLIM_NS',
                                              reg_mask = 0,
                                              reg_new_value = write_value)
    else:
        logging.info("Device does not support secure extension. Test case is passed by default.")
        return True

  @iec60730_test_case('TC: Verify MSPLIM_S')
  ## Verify the error detected correctly on the Register MSPLIM_S.
  # @details Order of execution:
  #   - Verify error detecting on MSPLIM_S
  #   - Test passes, return True
  #
  # @return True
  #
  def test_07_msplim_s_register(self):
    if -1 != chip_name.find("G2"):

        write_value = [0xAAAAAAA9, 0xAAAAAAAA, 0xAAAAAAAB, 0xAAAAAAAC, 0xAAAAAAAD, \
                        0xAAAAAAAE, 0xAAAAAAAF, 0xAAAAAAB0, 0xAAAAAAB1, 0xAAAAAAB2]
        return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_MSPLIM_S_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_FAIL_ENDLESS_LOOP_MSPLIM_S',
                                              bkp_fail = 'IEC60730_CPU_REGS_MSPLIM_S_PASS_ASM_BKPT',
                                              reg_test = 'MSPLIM',
                                              reg_mask = 0,
                                              reg_new_value = write_value)
    else:
        logging.info("Device does not support MSPLIM. Test case is passed by default.")
        return True

  @iec60730_test_case('TC: Verify PSPLIM_NS')
  ## Verify the error detected correctly on the Register PSPLIM_NS.
  # @details Order of execution:
  #   - Verify error detecting on PSPLIM_NS
  #   - Test passes, return True
  #
  # @return True
  #
  def test_08_psplim_ns_register(self):
    if -1 != chip_name.find("G2"):

        write_value = [0xAAAAAAA9, 0xAAAAAAAA, 0xAAAAAAAB, 0xAAAAAAAC, 0xAAAAAAAD, \
                        0xAAAAAAAE, 0xAAAAAAAF, 0xAAAAAAB0, 0xAAAAAAB1, 0xAAAAAAB2]
        return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_PSPLIM_NS_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_NS',
                                              bkp_fail = 'IEC60730_CPU_REGS_PSPLIM_NS_PASS_ASM_BKPT',
                                              reg_test = 'PSPLIM_NS',
                                              reg_mask = 0,
                                              reg_new_value = write_value)
    else:
        logging.info("Device does not support secure extension. Test case is passed by default.")
        return True

  @iec60730_test_case('TC: Verify PSPLIM_S')
  ## Verify the error detected correctly on the Register PSPLIM_S.
  # @details Order of execution:
  #   - Verify error detecting on PSPLIM_S
  #   - Test passes, return True
  #
  # @return True
  #
  def test_09_psplim_s_register(self):
    if -1 != chip_name.find("G2"):

        write_value = [0xAAAAAAA9, 0xAAAAAAAA, 0xAAAAAAAB, 0xAAAAAAAC, 0xAAAAAAAD, \
                        0xAAAAAAAE, 0xAAAAAAAF, 0xAAAAAAB0, 0xAAAAAAB1, 0xAAAAAAB2]
        return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_PSPLIM_S_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_FAIL_ENDLESS_LOOP_PSPLIM_S',
                                              bkp_fail = 'IEC60730_CPU_REGS_PSPLIM_S_PASS_ASM_BKPT',
                                              reg_test = 'PSPLIM',
                                              reg_mask = 0,
                                              reg_new_value = write_value)
    else:
        logging.info("Device does not support PSPLIM. Test case is passed by default.")
        return True

  @iec60730_test_case('TC: Verify CONTROL_NS')
  ## Verify the error detected correctly on the Register CONTROL_NS.
  # @details Order of execution:
  #   - Verify error detecting on CONTROL_NS
  #   - Test passes, return True
  #
  # @return True
  #
  def test_10_control_ns_register(self):
    if -1 != chip_name.find("G2"):

        self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_CONTROL_NS_SPSEL_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_CPU_REGS_CONTROL_NS_FAIL',
                                              bkp_fail = 'IEC60730_CPU_REGS_CONTROL_NS_PASS_ASM_BKPT',
                                              reg_test = 'CONTROL_NS',
                                              reg_mask = 0)

        self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_CONTROL_NS_FPCA_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_CPU_REGS_CONTROL_NS_FAIL',
                                              bkp_fail = 'IEC60730_CPU_REGS_CONTROL_NS_PASS_ASM_BKPT',
                                              reg_test = 'CONTROL_NS',
                                              reg_mask = 0)
    else:
        logging.info("Device does not support secure extension. Test case is passed by default.")
    return True


  @iec60730_test_case('TC: Verify CONTROL_S')
  ## Verify the error detected correctly on the Register CONTROL_S.
  # @details Order of execution:
  #   - Verify error detecting on CONTROL_S
  #   - Test passes, return True
  #
  # @return True
  #
  def test_11_control_s_register(self):

    self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_CONTROL_S_SPSEL_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_CONTROL_S_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_CONTROL_S_PASS_ASM_BKPT',
                                          reg_test = 'CONTROL',
                                          reg_mask = 0)

    self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_CONTROL_S_FPCA_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_CONTROL_S_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_CONTROL_S_PASS_ASM_BKPT',
                                          reg_test = 'CONTROL',
                                          reg_mask = 0)

    return True


  @iec60730_test_case('TC: Verify PRIMASK_NS')
  ## Verify the error detected correctly on the Register PRIMASK_NS.
  # @details Order of execution:
  #   - Verify error detecting on PRIMASK_NS
  #   - Test passes, return True
  #
  # @return True
  #
  def test_12_primask_ns_register(self):
    if -1 != chip_name.find("G2"):

        return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_PRIMASK_NS_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_CPU_REGS_PRIMASK_NS_FAIL',
                                              bkp_fail = 'IEC60730_CPU_REGS_PRIMASK_NS_PASS_ASM_BKPT',
                                              reg_test = 'PRIMASK_NS',
                                              reg_mask = 0)
    else:
        logging.info("Device does not support secure extension. Test case is passed by default.")
        return True

  @iec60730_test_case('TC: Verify PRIMASK_S')
  ## Verify the error detected correctly on the Register PRIMASK_S.
  # @details Order of execution:
  #   - Verify error detecting on PRIMASK_S
  #   - Test passes, return True
  #
  # @return True
  #
  def test_13_primask_s_register(self):

    return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_PRIMASK_S_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_PRIMASK_S_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_PRIMASK_S_PASS_ASM_BKPT',
                                          reg_test = 'PRIMASK',
                                          reg_mask = 0)


  @iec60730_test_case('TC: Verify BASEPRI_NS')
  ## Verify the error detected correctly on the Register BASEPRI_NS.
  # @details Order of execution:
  #   - Verify error detecting on BASEPRI_NS
  #   - Test passes, return True
  #
  # @return True
  #
  def test_14_basepri_ns_register(self):
    if -1 != chip_name.find("G2"):

        write_value = [0x9000]
        return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_BASEPRI_NS_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_CPU_REGS_BASEPRI_NS_FAIL',
                                              bkp_fail = 'IEC60730_CPU_REGS_BASEPRI_NS_PASS_ASM_BKPT',
                                              reg_test = 'BASEPRI_NS',
                                              reg_mask = 0,
                                              reg_new_value = write_value)
    else:
        logging.info("Device does not support secure extension. Test case is passed by default.")
        return True

  @iec60730_test_case('TC: Verify BASEPRI_S')
  ## Verify the error detected correctly on the Register BASEPRI_S.
  # @details Order of execution:
  #   - Verify error detecting on BASEPRI_S
  #   - Test passes, return True
  #
  # @return True
  #
  def test_15_basepri_s_register(self):

    write_value = [0x9000]
    return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_BASEPRI_S_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_BASEPRI_S_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_BASEPRI_S_PASS_ASM_BKPT',
                                          reg_test = 'BASEPRI',
                                          reg_mask = 0,
                                          reg_new_value = write_value)


  @iec60730_test_case('TC: Verify FAULTMASK_NS')
  ## Verify the error detected correctly on the Register FAULTMASK_NS.
  # @details Order of execution:
  #   - Verify error detecting on FAULTMASK_NS
  #   - Test passes, return True
  #
  # @return True
  #
  def test_16_faultmask_ns_register(self):
    if -1 != chip_name.find("G2"):

        return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_FAULTMASK_NS_ASM_BKPT',
                                              bkp_testcase_pass = 'IEC60730_CPU_REGS_FAULTMASK_NS_FAIL',
                                              bkp_fail = 'IEC60730_CPU_REGS_FAULTMASK_NS_PASS_ASM_BKPT',
                                              reg_test = 'FAULTMASK_NS',
                                              reg_mask = 0)
    else:
        logging.info("Device does not support secure extension. Test case is passed by default.")
        return True

  @iec60730_test_case('TC: Verify FAULTMASK_S')
  ## Verify the error detected correctly on the Register FAULTMASK_S.
  # @details Order of execution:
  #   - Verify error detecting on FAULTMASK_S
  #   - Test passes, return True
  #
  # @return True
  #
  def test_17_faultmask_s_register(self):

    return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_FAULTMASK_S_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_FAULTMASK_S_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_FAULTMASK_S_PASS_ASM_BKPT',
                                          reg_test = 'FAULTMASK',
                                          reg_mask = 0)


  @iec60730_test_case('TC: Verify FPU FPSCR')
  ## Verify the error detected correctly on the Register FPSCR.
  # @details Order of execution:
  #   - Verify error detecting on FPSCR
  #   - Test passes, return True
  #
  # @return True
  #
  def test_18_fpr_fpscr_register(self):

    write_value = [0x0000]
    return self.core_register_common_test(bkp_label_inject = 'IEC60730_CPU_REGS_FPU_FPSCR_ASM_BKPT',
                                          bkp_testcase_pass = 'IEC60730_CPU_REGS_FPU_FPSCR_FAIL',
                                          bkp_fail = 'IEC60730_CPU_REGS_FPU_FPSCR_PASS_ASM_BKPT',
                                          reg_test = 'FPSCR',
                                          reg_mask = 0,
                                          reg_new_value = write_value)


  @iec60730_test_case('TC: Verify FPU S0 - S31')
  ## Verify the error detected correctly on the FPU Registers S0 - S31.
  # @details Order of execution:
  #   - Verify error detecting on S0 - S31
  #   - Test passes, return True
  #
  # @return True
  #
  def test_19_fpu_sx_register(self):

    write_value = [0x55555556]

    for i in range(32):
      fp_reg = 'FPS' + str(i)
      bkp__inject = 'IEC60730_CPU_REGS_FPU_S' + str(i) + '_ASM_BKPT'
      result = self.core_register_common_test(bkp_label_inject = bkp__inject,
                                            bkp_testcase_pass = 'IEC60730_CPU_REGS_FPU_Sx_FAIL',
                                            bkp_fail = 'IEC60730_CPU_REGS_FPU_Sx_PASS_ASM_BKPT',
                                            reg_test = fp_reg,
                                            reg_mask = 0,
                                            reg_new_value = write_value)

      if result != True:
        return result
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
