# -*- coding: utf-8 -*-
# Copyright 2017 Square, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import enums

import platform
import sys
import os
from datetime import *
import logging
import traceback
import telnetlib

from junit_xml import TestSuite as JunitTestSuite
from junit_xml import TestCase as JunitTestCase


def is_integer(val):
    """Returns whether the given value is an integer.
    Args:
      val (object): value to check
    Returns:
      ``True`` if the given value is an integer, otherwise ``False``.
    """
    try:
        val += 1
    except TypeError:
        return False
    return True


def is_natural(val):
    """Returns whether the given value is a natural number.
    Args:
      val (object): value to check
    Returns:
      ``True`` if the given value is a natural number, otherwise ``False``.
    """
    return is_integer(val) and (val >= 0)


def is_os_64bit():
    """Returns whether the current running platform is 64bit.
    Returns:
      ``True`` if the platform is 64bit, otherwise ``False``.
    """
    return platform.machine().endswith('64')


def noop(*args, **kwargs):
    """No-op.  Does nothing.
    Args:
      args: list of arguments
      kwargs: keyword arguments dictionary
    Returns:
      ``None``
    """
    pass


def unsecure_hook_dialog(title, msg, flags):
    """No-op that ignores the dialog.
    Args:
      title (str): title of the insecure dialog
      msg (str): text of the insecure dialog
      flags (int): flags specifying which values can be returned
    Returns:
      ``enums.JLinkFlags.DLG_BUTTON_NO``
    """
    return enums.JLinkFlags.DLG_BUTTON_NO


def progress_bar(iteration,
                 total,
                 prefix=None,
                 suffix=None,
                 decs=1,
                 length=100):
    """Creates a console progress bar.
    This should be called in a loop to create a progress bar.
    See `StackOverflow <http://stackoverflow.com/questions/3173320/>`__.
    Args:
      iteration (int): current iteration
      total (int): total iterations
      prefix (str): prefix string
      suffix (str): suffix string
      decs (int): positive number of decimals in percent complete
      length (int): character length of the bar
    Returns:
      ``None``
    Note:
      This function assumes that nothing else is printed to the console in the
      interim.
    """
    if prefix is None:
        prefix = ''

    if suffix is None:
        suffix = ''

    format_str = '{0:.' + str(decs) + 'f}'
    percents = format_str.format(100 * (iteration / float(total)))
    filled_length = int(round(length * iteration / float(total)))
    bar = '█' * filled_length + '-' * (length - filled_length)

    prefix, suffix = prefix.strip(), suffix.strip()

    sys.stdout.write('\r%s |%s| %s%s %s' % (prefix, bar, percents, '%', suffix))
    sys.stdout.flush()

    if iteration == total:
        sys.stdout.write('\n')
        sys.stdout.flush()

    return None


def flash_progress_callback(action, progress_string, percentage):
    """Callback that can be used with ``JLink.flash()``.
    This callback generates a progress bar in the console to show the progress
    of each of the steps of the flash.
    Args:
      action (str): the current action being invoked
      progress_string (str): the current step in the progress
      percentage (int): the percent to which the current step has been done
    Returns:
      ``None``
    Note:
      This function ignores the compare action.
    """
    if action.lower() != 'compare':
        return progress_bar(min(100, percentage), 100, prefix=action)

    return None


def calculate_parity(n):
    """Calculates and returns the parity of a number.
    The parity of a number is ``1`` if the number has an odd number of ones
    in its binary representation, otherwise ``0``.
    Args:
      n (int): the number whose parity to calculate
    Returns:
      ``1`` if the number has an odd number of ones, otherwise ``0``.
    Raises:
      ValueError: if ``n`` is less than ``0``.
    """
    if not is_natural(n):
        raise ValueError('Expected n to be a positive integer.')

    y = 0
    n = abs(n)
    while n:
        y += n & 1
        n = n >> 1
    return y & 1


def device_target_power_on_off(hostIp, cmd):
    telnetNodeAdmin = TelnetDeviceUtil()
    if not telnetNodeAdmin.open_device_connection(hostName=hostIp,
                                                      hostPort=4902,
                                                      hide=True):
        return False
    telnetNodeAdmin.device_send('target power {}'.format(cmd), 0.5)
    telnetNodeAdmin.close_device_connection(hide=True)
    return True


class TimeStampUtil(object):
    def __init__(self):
        self._startUpTime = None

    def update_startUp_time(self):
        self._startUpTime = datetime.now().time()

    # @brief: return the current time by datetime.
    def get_absolute_time(self):
        return datetime.now().time()

    def get_total_running_time(self):
        return datetime.now().time() - self._startUpTime

    # @brief: convert the *timeIn* into a human-readable string.
    def convert_time_to_readable(self, timeIn):
        return datetime.now().isoformat()


"""
" @brief
"   PyTestSuites class serves as the key role to assert a statement and detects
"   whether a Junit test case should be logged.
"""


class PyTestSuites(object):
    def __init__(self, xmlDir=None):
        self._timeStampSetter = TimeStampUtil()
        self._testElements = []
        self._curTestElement = None
        # Local flags or constants.
        self._LOG_OUT = 1  # Enable/disable store the log in a file.
        if xmlDir:
            self._xmlDir = xmlDir
        else:
            self._xmlDir = os.path.dirname(os.path.realpath(__file__)) + \
                           '/../../result/report_' + datetime.now().strftime("%Y-%m-%d-%H-%M-%S") + '.xml'

    def store_log(self, msg):
        if self._LOG_OUT: logging.info(msg)

    def log_test(self, msg):
        t = self._timeStampSetter.get_absolute_time()
        logMsg = "IEC60730(" + \
                 self._timeStampSetter.convert_time_to_readable(t) + \
                 "): " + msg
        print(logMsg)
        self.store_log(logMsg)

    """
    " The methods, close_test_suite, complete_test_case and handle_test_suite_abortion
    " are called within this class.
    """

    # @brief
    #   This function is called when all the cases is done.
    def close_test_suite(self):
        xmlTestCases = []
        for testElement in self._testElements:
            tc = testElement['test_case']
            xmlTestCases.append(tc)

        if not len(xmlTestCases):
            self.log_test('No test cases were found.')
            return
        ts = JunitTestSuite("IEC Testsuite", xmlTestCases)
        with open(self._xmlDir, 'w') as f:
            ts.to_file(f, [ts], prettyprint=True, encoding="utf-8")

    # @brief
    #   This function is called when each test script ends without abortion.
    def complete_test_case(self):
        failure_info = self._curTestElement['failure_info']
        if len(failure_info):
            tc = self._curTestElement['test_case']
            tc.add_failure_info(message='Detected assumption failure',
                                output=failure_info)

    """
    " Structure of _testElements:1
    "
    " [
    "   { 'test_case':xxx, 'failure_info':xxx, 'file':xxx },
    "   { 'test_case':xxx, 'failure_info':xxx, 'file':xxx }
    " ]
    """

    def insert_test_case(self, testName, testClass, testResult=None, file=None):
        testElement = {}
        tc = JunitTestCase(name=testName, classname=testClass)
        testElement['test_case'] = tc
        testElement['result'] = testResult
        testElement['failure_info'] = ''
        if file:
            testElement['file'] = file
        else:
            testElement['file'] = ''
        self._testElements.append(testElement)
        self._curTestElement = testElement

    # @brief
    #   This function is called when each test script aborts.
    def abort_test_suite(self):
        raise AbortTestSuite

    # @brief
    #   A test case may contain multiple assumption. Once an assumption fails,
    #   the user needs to decide whether to complete the test case or to abort
    #   the whole test suite.
    def assume(self, statement, level=1):
        try:
            assert statement
            self.log_test(
                '\033[1;32m  [*] Passed assumption - '
                '{} at line {} \033[0m'.format(
                    str(traceback.extract_stack()[level][0]),
                    str(traceback.extract_stack()[level][1])
                )
            )
            return True
        except AssertionError:
            self._curTestElement['failure_info'] += str(traceback.extract_stack())
            self.log_test(
                '\033[1;31m  [*] Failed assumption - '
                '{} at line {} \033[0m'.format(
                    str(traceback.extract_stack()[level][0]),
                    str(traceback.extract_stack()[level][1])
                )
            )
            return False


class TelnetHostUtil(telnetlib.Telnet):
    def __init__(self, logService=None):
        telnetlib.Telnet.__init__(self)
        # Disable telnetlib debug printing.
        self.set_debuglevel(0)
        self._logService = logService
        self._timeStampUtil = TimeStampUtil()
        self._hostName = None
        self._hostPort = None
        self._hostPasswd = None
        self._hostLoginName = None

    # @brief: open a telnet connection to a host processor.
    def open_host_connection(self, hostName=None, hostPort=None, timeout=None, hide=None):
        if hostName and hostPort:
            self._hostName = hostName
            self._hostPort = hostPort
        self.open(self._hostName, self._hostPort, timeout)
        if not self.sock:
            raise ValueError('Host connection open failed!')
        else:
            self.log('Host connection open succeeded!', hide)

    # @brief: send commands to the host application with expected results.
    #         Returns 'success' if match is found, otherwise return original
    #         printouts.
    def host_send_expect(self, cmd, expectStr, timeout=None, hide=None):
        try:
            self.log("Sending cmd [" + cmd + "]...", hide)
            self.write(cmd + "\r\n")
            buf = self.read_until(expectStr, timeout)
            if expectStr in buf:
                return 'success'
            else:
                msg = "Expected string [" + \
                      expectStr + \
                      "] not found. Original prints:"
                # self.log(buf)
                return msg + "\n" + buf
        except EOFError:
            self.log('Socket disconnected.')
            return 'conn_failure'

    # @brief: send commands to the host application and return results. No
    #         check on the expected pattern.
    def host_send(self, cmd, timeout=None, hide=None):
        try:
            self.log("Sending cmd [" + cmd + "]...", hide)
            cmd = cmd.encode("utf-8")
            self.write(cmd + "\r\n".encode("utf-8"))
            buf = self.read_until('Nonsense'.encode("utf-8"), timeout)
            return buf
        except EOFError:
            self.log('Socket disconnected.')
            return ''

    # @brief: logs in this context.
    def log(self, msg, hide=None):
        logMsg = "Telnet2Host(" + str(self.host) + \
                 ":" + str(self.port) + ")-(" + \
                 self._timeStampUtil.convert_time_to_readable(
                     self._timeStampUtil.get_absolute_time()) + \
                 "): " + msg
        if hide is not True:
            print(logMsg)
        return logMsg

    def write_log(self, msg, hide=None, logAsResult=None):
        if hide and hide == True:
            pass
        else:
            if self._logService:
                self._logService.write_log(msg, hide, logAsResult)
            else:
                print(msg)

    ## Helper methods.
    def close_host_connection(self, hide=None):
        self.log('Host connection closing...', hide)
        self.close()

    def is_host_connected(self):
        if self.sock:
            return True
        else:
            return False


class TelnetDeviceUtil(TelnetHostUtil):
    def __init__(self, logService=None):
        self._logService = logService
        TelnetHostUtil.__init__(self, self._logService)

    # @brief: logs in this context.
    def log(self, msg, hide=None):
        logMsg = "Telnet2Device(" + str(self.host) + \
                 ":" + str(self.port) + ")-(" + \
                 self._timeStampUtil.convert_time_to_readable(
                     self._timeStampUtil.get_absolute_time()) + \
                 "): " + msg
        if hide and hide == True:
            pass
        else:
            print(logMsg)
        return logMsg

    def open_device_connection(self, hostName, hostPort=None, timeout=30, hide=None):
        ret = True
        try:
            self.open_host_connection(hostName, hostPort, timeout, hide)
        except ValueError:
            ret = False
        except IOError:
            ret = False
        return ret

    # Ensure the connection is closed for next telnet connection.
    def close_device_connection(self, hide=None):
        self.close_host_connection(hide)

    def leave_nwk_and_reset_device(self, hide=None):
        ret = self.host_send('reset', 0.5, hide) \
              + self.host_send_expect('network leave', "leave", 0.5, hide)
        return ret

    def reset_device(self, hide=None):
        ret = self.host_send('reset', 0.5, hide)
        return ret

    def clear_binding_table(self, hide=None):
        ret = self.host_send('option binding-table clear', 0.5, hide)
        return ret

    def device_read_until(self, expectStr, timeout):
        try:
            buf = self.read_until(expectStr, timeout)
            if expectStr in buf:
                return 'success'
            else:
                msg = "Expected string [" + \
                      expectStr + \
                      "] not found. Original prints:"
                return msg + "\n" + buf
        except EOFError:
            self.log('Socket disconnected.')
            return 'conn_failure'

    def device_send(self, cmd, timeout=None, hide=None):
        return self.host_send(cmd, timeout, hide)

    def device_send_expect(self, cmd, expectStr, timeout=None, hide=None):
        return self.host_send_expect(cmd, expectStr, timeout, hide)

    def device_send_no_socket_ret(self, cmd, timeout=None, hide=None):
        return self.host_send_no_socket_ret(cmd, timeout, hide)
