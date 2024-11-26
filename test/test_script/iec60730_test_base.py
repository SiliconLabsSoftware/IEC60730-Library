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

import sys
import os
import jlink
import logging
import re

from os import listdir

variables = {}
asm_labels = {}
chip_name = ""
adapter_serial_no = ""
lst_file_path = ""
lib_path = ""
compiler = ""

class iec60730_logger:
    @staticmethod
    def init_logger():
        logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s - %(levelname)s: %(message)s')
        logger = logging.getLogger()
        # Create file handler which logs even debug messages
        full_path = os.path.realpath(__file__)
        dir_path = os.path.dirname(full_path)
        log_report = dir_path + "/../../log/" + "temp.log"
        file_handler = logging.FileHandler(filename=log_report)
        file_handler.setLevel(logging.INFO)
        formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
        file_handler.setFormatter(formatter)
        logger.addHandler(file_handler)
        logger.propagate = False
        return logger
    def remove_handler():
        # Remove all handlers associated with the root logger object.
        for handler in logging.root.handlers[:]:
            handler.close()
            logging.root.removeHandler(handler)


class iec60730TestBase():

    def env_setup(self,
                adapter_serial_no,
                chip_name,
                lst_file_path,
                library_path,
                comp = "GCC"):
        self._chip_name = chip_name
        self._adapter_serial_no = adapter_serial_no
        self._output_path = lst_file_path
        self._library_path = library_path
        self._compiler = comp
        iec60730_logger.remove_handler()
        self.logger = iec60730_logger.init_logger()

    def _initialize(self):
        self._adapter_list = self._jlink._adapter_list
        return None

    def init_connect_group(self):
        self._is_connected = False
        self._adapter_serial_number = "0"

    def connect_mcu(self, chip_name):
        if self._is_connected:
            self.init_connect_group()
            self._jlink.close()
            msg = "Disconnected"
        else:
            result = self._jlink.connect(int(self._adapter_serial_number), chip_name)
            if not result:
                msg = "Connect failed."
            else:
                msg = "Connected to " + self._jlink._part_number
                self._is_connected = True
        logging.info(msg)
        return False

    def re_connect_mcu(self):
        logging.info("Re-connecting to MCU...")
        if self._is_connected:
            self._is_connected = False
            #logging.info("Adapter is being used. Closing it...")
            #self._jlink.close()
            #logging.info("Closed")
        result = self._jlink.connect(int(self._adapter_serial_number), self._chip_name)
        if not result:
            msg = "Connect failed."
        else:
            msg = "Connected to " + self._jlink._part_number
            self._is_connected = True
        logging.info(msg)
        return False

    def adapter_close(self):
        self._is_connected = False
        self._jlink.close()
        logging.info("Adapter Closed.")

    def flash_mcu(self):
        if self._is_connected:
            file_name = self.pathLineEdit.text()
            if os.path.isfile(file_name):
                offset = int("0x" + self.startAddrLineEdit.text(), 16)
                result = self._jlink.erase_chip()
                if not result:
                    self.init_connect_group()
                    self._jlink.close()
                    msg = "Erase failed."
                    self.statusBar().showMessage(msg)
                    return
                self._jlink.download(file_name, offset)
                if self.resetMCUCheckBox.checkState():
                    self._jlink.reset(False)
                msg = "Program done."
            else:
                msg = "Please select a file to download."
        else:
            msg = "Please connect to device."
        logging.debug(msg)
        return

    def erase_mcu(self):
        if self._is_connected:
            result = self._jlink.erase_chip()
            if not result:
                self.init_connect_group()
                self._jlink.close()
                msg = "Erase failed."
            else:
                msg = "Erase done."
        else:
            msg = "Please connect to device."
        logging.debug(msg)
        return

    def scan_adapter(self, serial_number, chip_name):
        num_adapters, adapter_list = self._jlink.get_usb_adapter_list()

        if (num_adapters == 0):
            return

        logging.info("Adapter list: ")
        for i in range(num_adapters):
            adapter = adapter_list[i]
            logging.info(str(i) + ". " + str(adapter.SerialNumber))
            if serial_number == str(adapter.SerialNumber):
                index = i

        if index < 0 or index >= num_adapters:
            logging.info("No adapter with serial number %s " % serial_number)
            return

        logging.info("Select adapter " + str(serial_number))

        adapter = adapter_list[index]
        self._adapter_serial_number = str(serial_number)
        return self.connect_mcu(chip_name)

    def pre_exec_test(self):
        files = os.listdir(self._output_path)

        lst_path = ''

        for file in files:
            if (".lst" in file):
                lst_path = self._output_path + '/' + file
                break

        symbol_table = False

        # Check Compiler GCC/IAR
        if self._compiler == "GCC" :
            with open(lst_path, encoding = 'utf-8') as lst_file:
                for line in lst_file :
                    if "SYMBOL TABLE:" in line :
                        symbol_table = True

                    if (symbol_table == True) and line.isspace() :
                        break

                    if symbol_table == True :
                        if ((".IEC60730" in line) or ("IEC60730" in line) or ("iec60730" in line)) \
                            and (".text" in line) :
                            address_string = re.search("^\S*", line).group(0)
                            label = re.search("\S*$", line).group(0)
                            asm_labels[label] = int(address_string, 16)
                            # print("Text[{}] = 0x{}".format(label, address_string))

                        if (".bss" in line) or (".data" in line) or (".user_classb" in line) \
                            or (".crc" in line) or (".ram_no_clear" in line) or ("ram_ret_no_clear" in line) \
                            or (".stack_check" in line):
                            address_string = re.search("^\S*", line).group(0)
                            label = re.search("\S*$", line).group(0)
                            variables[label] = int(address_string, 16)
                            # print("Data[{}] = 0x{}".format(label, address_string))
        elif self._compiler == "IAR":
            count_space = 0
            with open(lst_path, encoding = 'utf-8') as lst_file:
                for line in lst_file :
                    if ".symtab:" in line :
                        symbol_table = True

                    if (symbol_table == True) and line.isspace() :
                        count_space = count_space + 1
                        if count_space == 3:
                            break

                    if symbol_table == True :
                        str_line = re.split(r'\s+', line)
                        # print(str_line)
                        if len(str_line) <= 2:
                            continue
                        label = str_line[2]
                        line_addr = ""
                        if len(str_line) <= 4:
                            line_addr = next(lst_file)
                            str_line_addr = re.split(r'\s+', line_addr)
                            address_string = str_line_addr[1].replace("'", "")
                            # print(str_line_addr)
                        else:
                            address_string = str_line[3].replace("'", "")

                        if ("Code" in line) or ("Code" in line_addr):
                            if ("??IEC60730" in line) or ("IEC60730" in line) or ("iec60730" in line):
                                asm_labels[label] = int(address_string, 16) - 1
                                #print("Code[{}] = {}".format(label, address_string))
                        elif ("Data" in line) or ("Data" in line_addr):
                            variables[label] = int(address_string, 16)
                            # print("Data[{}] = {}".format(label, address_string))

        self._jlink = jlink.JLinkDll(self._library_path)
        self.adapter = self._jlink
        self._is_connected = False
        self._adapter_serial_number = "0"
        self._initialize()
        self.scan_adapter(self._adapter_serial_no, self._chip_name)
        #self.logger = LogGen.loggen()

    def post_exec_test(self):
        self.init_connect_group()
        self._jlink.close()
        logging.info("Disconnected")

    def get_variable_address(self, relative_path, var_name):
        files = os.listdir(self._output_path + relative_path)

        map_path = ''
        for file in files:
          if (".map" in file):
            map_path = self._output_path + relative_path + file
            break

        is_search_addr = False
        address_string = '0'
        with open(map_path, encoding = 'utf-8') as map_file:
          if self._compiler == "GCC" :
            for line in map_file:
              if (('.'+var_name) in line):
                is_search_addr = True
                continue

              #print(line)

              if is_search_addr == True \
                and var_name in line:
                address_string = re.search("0x(.*?) ", line).group(1)
                break
          elif self._compiler == "IAR":
            for line in map_file:
              if (var_name in line):
                address_string = re.search("0x(.*?) ", line.replace("'", "")).group(1)
                break

        return int(address_string, 16)

    def get_label(self, address):
        if address in asm_labels.values():
            return list(asm_labels.keys())[list(asm_labels.values()).index(address)]
        return hex(address)

    def set_chip_name(self, chip_name):
        self._chip_name = chip_name

def iec60730_test_case(testcase):
    def _iec60730_test_case(f):
        def wrapper(self,*args):
            self.pre_exec_test()
            logging.info("----------------- Start testcase: " + testcase)
            print("{}: ".format(testcase))
            result = f(self, *args)
            # try:
            #     result = f(self, *args)
            # except Exception as e:
            #     print ("Exception msg: {}".format(e))
            #     result = False
            # print result of test case 0: pass 1:fail
            print (result)
            logging.info("Result testcase {}: {}".format(testcase, result))
            logging.info("----------------- End testcase: ")
            self.post_exec_test()
            return result
        return wrapper
    return _iec60730_test_case

# Main Function
if __name__ == '__main__':
    print("iec60730TestBase")
