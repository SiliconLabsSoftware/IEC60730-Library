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

from binascii import hexlify
import random
import os
import sys
from time import sleep
import glob

from iec60730_test_base import *
import jlink, jlink_constants
import unittest
import enums
import re
import logging

list_test_case = []
unity_begin_row = 0
unity_end_row = 0

def get_unity_begin_file(files,logger):
  with open(files, 'r') as fp:
  # read all lines using readline()
    lines = fp.readlines()
    for row in lines:
      # check if string present on a current line
      word = 'UnityBegin'
      if row.find(word) != -1:
        res = re.findall(r'\(.*?\)', row)
        file_test = str(res[0])
        file_test =file_test.replace('(','').replace(')','').replace('"','')
        # printing result
        logger.info("File test case: " + file_test)
        return file_test, lines.index(row)

def get_unity_end_file(files):
  with open(files, 'r') as fp:
  # read all lines using readline()
    lines = fp.readlines()
    for row in lines:
      # check if string present on a current line
      word = 'UnityEnd'
      if row.find(word) != -1:
        return lines.index(row)

def get_test_case(files,logger,start,end):
  with open(files, 'r') as fp:
  # read all lines using readline()
    lines = fp.readlines()

    for row in lines:
      # check if string present on a current line
      word = 'run_test'
      if (row.find(word) != -1) and (start < lines.index(row)) \
      and (end > lines.index(row)):
        res = re.findall(r'\(.*?\)', row)
        test_case = str(res[0])
        test_case = test_case.replace('(','').replace(')','').replace('"','')
        # printing result
        list_test_case.append(test_case)
    return list_test_case

def get_report_unit_test(result,file_test_case,log_report):
  test_fail = 0

  # Remove all handlers associated with the root logger object.
  for handler in logging.root.handlers[:]:
    logging.root.removeHandler(handler)
  logging.basicConfig(filename=log_report,
                    format='%(asctime)s %(message)s',
                    filemode='w')
  logger = logging.getLogger(log_report)
  logger.setLevel(logging.DEBUG)

  file_test, unity_begin_row = get_unity_begin_file(file_test_case,logger)
  unity_end_row = get_unity_end_file(file_test_case)
  list_test_case = get_test_case(file_test_case,logger,unity_begin_row,unity_end_row)

  if len(list_test_case) != len(result):
    raise ValueError("The list of test cases " + str(len(list_test_case)) + " in the file"
                    + file_test + " is not equal " + str(len(result)) + " test cases"
                    " results received from device")

  for test_case in list_test_case:
    index = list_test_case.index(test_case)
    test_case = test_case.split(',',3)
    if result[index] == 0:
      logger.info(file_test + ":" + test_case[2] + ":" + test_case[0] + ":PASS")
    else:
      test_fail +=1
      logger.info(file_test + ":" + test_case[2] + ":" + test_case[0] + ":FAIL")
  logger.info("\n----------------------------------------------------------------------")
  logger.info(str(len(list_test_case)) + " Tests " + str(test_fail) + " Failures")
  if test_fail != 0:
    logger.info("FAIL")
  else:
    logger.info("PASS")

def int_to_bytes(number: int) -> bytes:
    return number.to_bytes(length=(8 + (number + (number < 0)).bit_length()) // 8, byteorder='big', signed=True)


##  IEC60730 get unit test result
#
# @details This class runs IEC60730 get unit test results
#
# Hardware setup:
#   Connect DUT to PC. No additional hardware setup required.

class iec60730_unit_test_result(unittest.TestCase, iec60730TestBase):

  def get_unit_test_result(self):
    def get_result(self):
      self.adapter.halt()
      self.adapter.clear_all_breakpoints()

      self.adapter.set_breakpoint(address = asm_labels['IEC60730_UNIT_TEST_END'],
                                  typeflags=enums.JLinkBreakpoint.ANY)

      self.adapter.reset()
      pc = self.adapter.run_to_breakpoint(3)
      logging.info("Halted at label: " + self.get_label(pc))
      self.assertEqual(pc, asm_labels['IEC60730_UNIT_TEST_END'],\
                      "DUT failed to stop at IEC60730_UNIT_TEST_END" \
                      " , pc = " + hex(pc))
      #logging.info("Halted at label: " + self.get_label(pc))
      current_test_numth = variables['current_test_numth']
      self.assertNotEqual(0, current_test_numth,\
                        "Can not read current_test_numth address")
      reg = self.adapter.memory_read8(current_test_numth, 1, zone=None)

      current_test_numth = variables['current_test_numth']
      self.assertNotEqual(0, current_test_numth,\
                      "Can not read current_test_numth address")
      current_test_numth = self.adapter.memory_read8(current_test_numth, 1, zone=None)

      test_case_result = variables['test_case_result']
      self.assertNotEqual(0, test_case_result,\
                      "Can not read test_case_result address")
      result = self.adapter.memory_read(test_case_result,current_test_numth[0])
      return result
    def connect_device(self):
      self.pre_exec_test()
      full_path = os.path.realpath(__file__)
      dir_path = os.path.dirname(full_path)
      file_test_case = dir_path + "/../unit_test/src/" + file_name + ".c"
      logging.info("File name test: " + file_test_case)
      log_report = dir_path + "/../../log/" + file_name + ".log"
      logging.info("----------------- Start get unit test result")
      result = get_result(self)
      logging.info("Get unit test result: Done. See " + log_report)
      logging.info("----------------- End get unit test result")
      self.post_exec_test()
      get_report_unit_test(result,file_test_case,log_report)
    return connect_device(self)

if __name__ == "__main__":

  file_name= os.getenv('FILE_NAME')

  chip_name = os.getenv('CHIP')


  lst_file_path = os.getenv('LST_PATH')

  adapter_serial_no = os.getenv('ADAPTER_SN')

  lib_path = os.getenv('JLINK_PATH')

  while len(sys.argv) > 1:
    line = sys.argv.pop()
    if len(sys.argv) == 1:
      compiler = line

  print("Compiler: "+compiler)

  if lst_file_path.find(file_name) != -1:
    iec60730_unit_test_result = iec60730_unit_test_result()
    iec60730_unit_test_result.env_setup(adapter_serial_no,
                                        chip_name,
                                        lst_file_path,
                                        lib_path,
                                        compiler)
    iec60730_unit_test_result.get_unit_test_result()
  else:
    print("The FILE_NAME environment variable is not the same as the *.lst file prefix name"
    " in the path " + lst_file_path)
