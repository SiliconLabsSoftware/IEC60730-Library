"""
jlink.py:  JLinkARM.dll Wrapper - Python Edition

This module provides access to all functions of JLinkARM.dll.

"""
import ctypes
import ctypes as ct
import os
import sys
import logging
import six
import time
from enum import Enum, unique
import enums
import errors
from jlink_constants import *

# Adapter Interfaces
JLINKARM_HOSTIF_USB = 0x01
JLINKARM_HOSTIF_IP  = 0x02


class JLinkDll:
    """
    Object for accessing and controlling a JLink adapter
    """
    MAX_BUF_SIZE = 336

    # Maximum number of adapters this wrapper can find.
    MAX_NUM_ADAPTERS = 32

    # Maximum number of CPU registers.
    MAX_NUM_CPU_REGISTERS = 256

    #logger = None

    def __init__(self, lib_path):
        try:
            # Load JLinkARM.dll
            self._dll = ct.cdll.LoadLibrary(lib_path)
            #self._dll = ct.cdll.LoadLibrary("/opt/SEGGER/ozone/3.22.1/Lib/libjlinkarm.so")
        except:
            print("Unable to load JLinkDll class")

        # Keep a list of all adapters
        self._num_adapters, self._adapter_list = self.get_usb_adapter_list()

        # Initialize all properties of the connected MCU
        self._initialize_mcu_properties()

        #self._init_logger()

        # Suppress dialog from the DLL
        self._suppress_usb_dialog()

    # def _init_logger(self):
    #     logging.basicConfig(level=logging.INFO,
    #                 format='%(asctime)s - %(levelname)s: %(message)s')
    #     self.logger = logging.getLogger()

    def abort(self, errMsg):
        self.logger.error(errMsg)
        self._dll.JLINKARM_Close()
        self._initialize_mcu_properties()

    def execute_command(self, command_string):
        """
        Execute a JLink command string.

        Keyword arguments:
        command_string -- the string to execute with JLINKARM_ExecCommand()

        Returns the return value of the executed command.
        """
        logging.debug(" >>>>>>>> Execute command: [" + command_string + "]")
        err_buf = (ctypes.c_char * self.MAX_BUF_SIZE)()
        buffer_size = ct.c_int(self.MAX_BUF_SIZE)
        res = self._dll.JLINKARM_ExecCommand(command_string.encode(), err_buf, buffer_size)
        err_buf = ctypes.string_at(err_buf).decode()

        if len(err_buf) > 0:
            raise errors.JLinkException(err_buf.strip())

        return res

    def _initialize_mcu_properties(self):
        """
        Resest all internal variables tracking the properties of the
        attached MCU.
        """
        self._mcu_interface_type = None
        self._mcu_core = None
        self._id_code = None
        self._part_family = None
        self._part_number = None
        self._part_number_id = None
        self._flash_size = None

    def _suppress_usb_dialog(self):
        """
        Suppresses dialog message from JLinkARM.dll.
        """
        result = self.execute_command("SuppressEmuUSBDialog")
        if result != 0:
            raise errors.JLinkException('_suppress_usb_dialog')

    def _locked_part_callback(self, sTitle, sMessage, Flags):
        """
        Callback function called if attached part is locked.
        This prevents pop-up dialog from JLINK DLL.
        """
        JLINK_DLG_BUTTON_NO = (1 << 1)

        return JLINK_DLG_BUTTON_NO

    def get_usb_adapter_list(self):
        """
        Returns a list of all adapters connected to the host via USB.
        """
        host_ifs = ct.c_int(JLINKARM_HOSTIF_USB)
        connect_info = \
            (JLinkArmEmuConnectInfo_CStruct * self.MAX_NUM_ADAPTERS)()
        max_info = ct.c_int(self.MAX_NUM_ADAPTERS)

        num_adapters = self._dll.JLINKARM_EMU_GetList(host_ifs,
                                                      ct.byref(connect_info),
                                                      max_info)
        # Create a list of information for each adapter
        adapter_list = []
        for i in range(num_adapters):
            adapter_list.append(connect_info[i])
        self._num_adapters = num_adapters
        self._adapter_list = adapter_list
        return num_adapters, adapter_list

    def get_tcp_ip_adapter_list(self):
        """
        Returns a list of all adapters connected to the host via TCP/IP.
        """
        host_ifs = ct.c_int(JLINKARM_HOSTIF_IP)
        connect_info = \
            (JLinkArmEmuConnectInfo_CStruct * self.MAX_NUM_ADAPTERS)()
        max_info = ct.c_int(self.MAX_NUM_ADAPTERS)

        num_adapters = self._dll.JLINKARM_EMU_GetList(host_ifs,
                                                      ct.byref(connect_info),
                                                      max_info)
        # Create a list of information for each adapter
        adapter_list = []
        for i in range(num_adapters):
            adapter_list.append(connect_info[i])

        return num_adapters, adapter_list

    def get_adapter_list(self):
        """
        Returns a list of all adapters connected to the host via TCP/IP or USB.
        """
        host_ifs = ct.c_int(JLINKARM_HOSTIF_USB | JLINKARM_HOSTIF_IP)
        connect_info = \
            (JLinkArmEmuConnectInfo_CStruct * self.MAX_NUM_ADAPTERS)()
        max_info = ct.c_int(self.MAX_NUM_ADAPTERS)

        num_adapters = self._dll.JLINKARM_EMU_GetList(host_ifs,
                                                      ct.byref(connect_info),
                                                      max_info)
        # Create a list of information for each adapter
        adapter_list = []
        for i in range(num_adapters):
            adapter_list.append(connect_info[i])

        return num_adapters, adapter_list

    def connect(self,
                serial_number,
                chip_name):
        # First, look for the adapter in the adapter list
        # num_adapters, adapter_list = self.get_usb_adapter_list()
        found_adapter = False
        for adapter in self._adapter_list:
            if adapter.SerialNumber == serial_number:
                found_adapter = True
                break

        if not found_adapter:
            raise errors.JLinkException('No emulator with serial number %s found.' % serial_number)

        self._suppress_usb_dialog()
        # serial_number = ct.c_uint32(serial_number)

        # USB Adapter
        if adapter.Connection == JLINKARM_HOSTIF_USB:
            error = self._dll.JLINKARM_EMU_SelectByUSBSN(serial_number)
            if error < 0:
                raise errors.JLinkException('No emulator with serial number %s found.' % serial_number)
        else:
            print("TCP/IP support currently not implemented in jlink.py.")
            return False

        self._initialize_mcu_properties()

        self._dll.JLINKARM_Open.restype = ctypes.POINTER(ctypes.c_char)
        result = self._dll.JLINKARM_Open()
        result = ctypes.cast(result, ctypes.c_char_p).value
        if result is not None:
            raise errors.JLinkException(result.decode())

        error = self._dll.JLINKARM_HasError()
        if error != 0:
            raise errors.JLinkException("JLINKARM_HasError = True")

        self._dll.JLINKARM_TIF_Select(enums.JLinkInterfaces.SWD)
        #self.set_device("EFR32MG21AXXXF1024")

        # Set device
        self.set_device(chip_name)

        # Set to maximum speed
        speed_info = self.get_speed_info()
        maxSpeed = int(speed_info["BaseFreq"] / speed_info["MinDiv"] / 1000)
        speedSet = int(maxSpeed / 2)
        logging.info("JLINKARM_SetSpeed: " + str(speedSet))
        self._dll.JLINKARM_SetSpeed(speedSet)

        # Connect, halt, and get the Part ID
        result = self._dll.JLINKARM_Connect()

        if result < 0:
            raise errors.JLinkException(result)

        self._dll.JLINKARM_Halt()

        id_data = self.get_id_data()

        self._part_number = chip_name
        self._part_family = self._dll.JLINKARM_GetDeviceFamily()

        return True

    def close(self):
        """
        Closes the connection to the JLink adapter.
        """
        self._dll.JLINKARM_Close()
        self._initialize_mcu_properties()

    def get_speed(self):
        """
        Returns the current JTAG connection speed.
        """
        speed = self._dll.JLINKARM_GetSpeed()
        return speed

    def set_max_speed(self):
        """
        Sets the JTAG connection speed to its maximum value.
        """
        self._dll.JLINKARM_SetMaxSpeed()

    def set_speed(self, speed=4000):
        """
        Sets the JTAG connection speed.

        Keyword arguments:
        speed -- speed of JTAG connection in kHz.
        """
        self._dll.JLINKARM_SetSpeed(ct.c_uint32(speed))

    def get_speed_info(self):
        """
        Gets the target interface speed information.

        Returns a dictionary containing the speed information.
        """
        speed_info = JlinkArm_Speed_Info_CStruct()
        speed_info.SizeOfStruct = ct.c_uint32(ct.sizeof(speed_info))
        self._dll.JLINKARM_GetSpeedInfo(ct.pointer(speed_info))

        return {"BaseFreq": speed_info.BaseFreq,
                "MinDiv": speed_info.MinDiv,
                "SupportAdaptive": bool(speed_info.SupportAdaptive)}

    def get_id(self):
        """
        Retrieves ID of the core.
        """
        id = self._dll.JLINKARM_GetId()
        return id

    def get_id_data(self):
        """
        Retrieves detailed info of the device on the JTAG bus.
        """
        id_data = JLinkJtagIdData_CStruct()
        self._dll.JLINKARM_GetIdData(ct.pointer(id_data))
        return id_data

    def halted(self):
        """Returns whether the CPU core was halted.

        Args:
          self (JLink): the ``JLink`` instance

        Returns:
          ``True`` if the CPU core is halted, otherwise ``False``.

        Raises:
          JLinkException: on device errors.
        """
        result = int(self._dll.JLINKARM_IsHalted())
        if result < 0:
            raise errors.JLinkException(result)

        return (result > 0)

    def run_to_breakpoint(self, timeout):

        self._dll.JLINKARM_Step()
        self._dll.JLINKARM_Go()
        isHalted = False
        startTime = time.perf_counter()
        while isHalted == False and \
                (time.perf_counter() - startTime) < timeout:
            isHalted = self.halted()

        pc_halt = self.register_read("R15 (PC)")

        logging.debug("Halted at: " + hex(pc_halt))

        return pc_halt

    def go_ex(self):
        """
        Runs the currently connected device, skipping over any breakpoint at the current instruction.
        """
        self._dll.JLINKARM_GoEx(ct.c_uint32(JLINKARM_GO_MAX_EMUL_INSTS_DEFAULT),
                                ct.c_uint32(JLINKARM_GO_FLAG_OVERSTEP_BP))

    def step(self):
        self._dll.JLINKARM_Step()

    def halt(self):
        self._dll.JLINKARM_Halt()

    def reset(self, halt=True):
        """
        Resets the currently connected device.

        Keyword arguments:
        halt -- if true, the part will be halted before reset
                if false, the part will not be halted before reset
        """
        if halt:
            self._dll.JLINKARM_Reset()
        else:
            self._dll.JLINKARM_ResetNoHalt()

    def get_device_family(self):
        return self._dll.JLINKARM_GetDeviceFamily()

    def set_device(self, device):
        result = self.execute_command("device = %s" % device)
        return result


    def read_mem(self, address, num_bytes):
        data = ct.create_string_buffer(num_bytes)
        status = ct.create_string_buffer(256)
        result = self._dll.JLINKARM_ReadMem(ct.c_uint32(address),
                                            ct.c_uint32(num_bytes),
                                            ct.pointer(data),
                                            ct.pointer(status))
        if result:
            errMsg = "ReadMem: Failure result {} reading {} bytes at '{:#010x}'".format(result, num_bytes, address)
            self.abort(errMsg)
        # Convert to a list of integers
        ret_buffer = []
        for byte in data.raw:
            ret_buffer.append(ord(byte))

        return ret_buffer

    def read_ram_arm_8(self, address, num_bytes):
        data = ct.create_string_buffer(num_bytes)
        status = ct.create_string_buffer(256)
        self._dll.JLINKARM_ReadMemU8(ct.c_uint32(address),
                                     ct.c_uint32(num_bytes),
                                     ct.pointer(data),
                                     ct.pointer(status))
        # Convert to a list of 8-bit integers
        ret_buffer = []
        for i in range(num_bytes):
            ret_buffer.append(ord(data[i]))

        return ret_buffer

    def read_ram_arm_16(self, address, num_words):
        data = ct.create_string_buffer(num_words * 2)
        status = ct.create_string_buffer(256)
        self._dll.JLINKARM_ReadMemU16(ct.c_uint32(address),
                                      ct.c_uint32(num_words),
                                      ct.pointer(data),
                                      ct.pointer(status))
        # Convert to a list of 16-bit integers
        ret_buffer = []
        for i in range(num_words):
            ret_buffer.append(
                (ord(data[(i * 2) + 1]) * 256) + ord(data[i * 2]))

        return ret_buffer

    def read_ram_arm_32(self, address, num_words):
        data = ct.create_string_buffer(num_words * 4)
        status = ct.create_string_buffer(256)
        self._dll.JLINKARM_ReadMemU32(ct.c_uint32(address),
                                      ct.c_uint32(num_words),
                                      ct.pointer(data),
                                      ct.pointer(status))
        # Convert to a list of 32-bit integers
        ret_buffer = []
        for i in range(num_words):
            ret_buffer.append((ord(data[(i * 2) + 3]) * 0x1000000) +
                              (ord(data[(i * 2) + 2]) * 0x10000) +
                              (ord(data[(i * 2) + 1]) * 0x100) +
                              ord(data[i * 2]))

        return ret_buffer

    def read_ram_arm_64(self, address, num_words):
        """
        Reads a block of RAM in 64-bit words.

        Keyword arguments:
        address -- starting address to read
        num_words -- number of 64-bit words to read
        """
        data = ct.create_string_buffer(num_words * 8)
        status = ct.create_string_buffer(256)
        self._dll.JLINKARM_ReadMemU64(ct.c_uint32(address),
                                      ct.c_uint32(num_words),
                                      ct.pointer(data),
                                      ct.pointer(status))
        # Convert to a list of 64-bit integers
        ret_buffer = []
        for i in range(num_words):
            ret_buffer.append((ord(data[(i * 2) + 7]) * 0x100000000000000) +
                              (ord(data[(i * 2) + 6]) * 0x1000000000000) +
                              (ord(data[(i * 2) + 5]) * 0x10000000000) +
                              (ord(data[(i * 2) + 4]) * 0x100000000) +
                              (ord(data[(i * 2) + 3]) * 0x1000000) +
                              (ord(data[(i * 2) + 2]) * 0x10000) +
                              (ord(data[(i * 2) + 1]) * 0x100) +
                              ord(data[i * 2]))

        return ret_buffer

    def clear_breakpoint(self, bp_to_clear):
        '''
        Clears breakpoint on the connected device.

        Keyword arguments:
        bp_to_clear - Handle of the breakpoint to clear.
            Pass JLINKARM_BP_HANDLE_ALL to clear all breakpoints.
        '''
        result = self._dll.JLINKARM_ClrBPEx(ct.c_int32(bp_to_clear))
        if result:
            raise errors.JLinkException("JLINKARM_ClrBPEx error")

    def clear_all_breakpoints(self):
        result = self._dll.JLINKARM_ClrBPEx(0xFFFFFFFF)
        if result:
            raise errors.JLinkException("JLINKARM_ClrBPEx clear all breakpoints error")

    def register_name(self, register_index):
        """Retrieves and returns the name of an ARM CPU register.

        Args:
          self (JLink): the ``JLink`` instance
          register_index (int): index of the register whose name to retrieve

        Returns:
          Name of the register.
        """
        self._dll.JLINKARM_GetRegisterName.restype = ctypes.POINTER(ctypes.c_char)

        result = self._dll.JLINKARM_GetRegisterName(register_index)
        return ctypes.cast(result, ctypes.c_char_p).value.decode()

    def register_list(self):
        """Returns a list of the indices for the CPU registers.

        The returned indices can be used to read the register content or grab
        the register name.

        Args:
          self (JLink): the ``JLink`` instance

        Returns:
          List of registers.
        """
        num_items = self.MAX_NUM_CPU_REGISTERS
        buf = (ctypes.c_uint32 * num_items)()
        num_regs = self._dll.JLINKARM_GetRegisterList(buf, num_items)
        return buf[:num_regs]

    def _get_register_index_from_name(self, register):
        """
        Converts a register name to a register index

        Args:
            self (JLink): the ``JLink`` instance
            register (str): the register name

        Returns:
          ``int``
        """
        index = 0xFF
        for idx in self.register_list():
            if register == self.register_name(idx):
                index = idx

        if index == 0xFF:
            error_message = "No register found matching name: {}. (available registers: {})"
            regs = list(self.register_name(idx) for idx in self.register_list())
            raise errors.JLinkException(error_message.format(register, ', '.join(regs)))

        return index

    def register_read(self, register_index):
        """Reads the value from the given register.

        Args:
          self (JLink): the ``JLink`` instance
          register_index (int/str): the register to read

        Returns:
          The value stored in the given register.
        """
        # TODO: rename 'register_index' to 'register'
        if isinstance(register_index, six.string_types):
            register_index = self._get_register_index_from_name(register_index)
        return self._dll.JLINKARM_ReadReg(register_index)

    def register_write(self, reg_index, value):
        """Writes into an ARM register.

        Note:
          The data is not immediately written, but is cached before being
          transferred to the CPU on CPU start.

        Args:
          self (JLink): the ``JLink`` instance
          reg_index (int/str): the ARM register to write to
          value (int): the value to write to the register

        Returns:
          The value written to the ARM register.

        Raises:
          JLinkException: on write error.
        """
        # TODO: rename 'reg_index' to 'register'
        if isinstance(reg_index, six.string_types):
            reg_index = self._get_register_index_from_name(reg_index)
        res = self._dll.JLINKARM_WriteReg(reg_index, value)
        if res != 0:
            raise errors.JLinkException('Error writing' + hex(value) + ' to register %d' % reg_index)
        return value


    def num_active_breakpoints(self):
        """Returns the number of currently active breakpoints.

        Args:
          self (JLink): the ``JLink`` instance

        Returns:
          The number of breakpoints that are currently set.
        """
        return self._dll.JLINKARM_GetNumBPs()

    def set_breakpoint(self, address, typeflags):
        '''
        Set a breakpoint within the CPU.

        address - Address for the breakpoint.
        typeflags - Flags for the breakpoint.  Ignored for EFM8 devices.
        '''
        bp_addr = hex(address)
        logging.debug("Set BP: " + str(bp_addr))
        flags = enums.JLinkBreakpoint.ANY
        handle = self._dll.JLINKARM_SetBPEx(int(address), flags)
        if handle <= 0:
            raise errors.JLinkException('Breakpoint could not be set.')

        return handle

    def memory_read(self, addr, num_units, zone=None, nbits=None):
        """Reads memory from a target system or specific memory zone.

        The optional ``zone`` specifies a memory zone to access to read from,
        e.g. ``IDATA``, ``DDATA``, or ``CODE``.

        The given number of bits, if provided, must be either ``8``, ``16``, or
        ``32``.  If not provided, always reads ``num_units`` bytes.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to read from
          num_units (int): number of units to read
          zone (str): optional memory zone name to access
          nbits (int): number of bits to use for each unit

        Returns:
          List of units read from the target system.

        Raises:
          JLinkException: if memory could not be read.
          ValueError: if ``nbits`` is not ``None``, and not in ``8``, ``16``,
            or ``32``.
        """
        buf_size = num_units
        buf = None
        access = 0

        if nbits is None:
            buf = (ctypes.c_uint8 * buf_size)()
            access = 0
        elif nbits == 8:
            buf = (ctypes.c_uint8 * buf_size)()
            access = 1
        elif nbits == 16:
            buf = (ctypes.c_uint16 * buf_size)()
            access = 2
            buf_size = buf_size * access
        elif nbits == 32:
            buf = (ctypes.c_uint32 * buf_size)()
            access = 4
            buf_size = buf_size * access
        else:
            raise ValueError('Given bit size is invalid: %s' % nbits)

        args = [addr, buf_size, buf, access]

        method = self._dll.JLINKARM_ReadMemEx
        if zone is not None:
            method = self._dll.JLINKARM_ReadMemZonedEx
            args.append(zone.encode())

        units_read = method(*args)
        if units_read < 0:
            raise errors.JLinkReadException(units_read)

        return buf[:units_read]

    def memory_read8(self, addr, num_bytes, zone=None):
        """Reads memory from the target system in units of bytes.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to read from
          num_bytes (int): number of bytes to read
          zone (str): memory zone to read from

        Returns:
          List of bytes read from the target system.

        Raises:
          JLinkException: if memory could not be read.
        """
        return self.memory_read(addr, num_bytes, zone=zone, nbits=8)

    def memory_read16(self, addr, num_halfwords, zone=None):
        """Reads memory from the target system in units of 16-bits.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to read from
          num_halfwords (int): number of half words to read
          zone (str): memory zone to read from

        Returns:
          List of halfwords read from the target system.

        Raises:
          JLinkException: if memory could not be read
        """
        return self.memory_read(addr, num_halfwords, zone=zone, nbits=16)

    def memory_read32(self, addr, num_words, zone=None):
        """Reads memory from the target system in units of 32-bits.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to read from
          num_words (int): number of words to read
          zone (str): memory zone to read from

        Returns:
          List of words read from the target system.

        Raises:
          JLinkException: if memory could not be read
        """
        return self.memory_read(addr, num_words, zone=zone, nbits=32)

    def memory_read64(self, addr, num_long_words):
        """Reads memory from the target system in units of 64-bits.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to read from
          num_long_words (int): number of long words to read

        Returns:
          List of long words read from the target system.

        Raises:
          JLinkException: if memory could not be read
        """
        buf_size = num_long_words
        buf = (ctypes.c_ulonglong * buf_size)()
        units_read = self._dll.JLINKARM_ReadMemU64(addr, buf_size, buf, 0)
        if units_read < 0:
            raise errors.JLinkException(units_read)

        return buf[:units_read]

    def memory_write(self, addr, data, zone=None, nbits=None):
        """Writes memory to a target system or specific memory zone.

        The optional ``zone`` specifies a memory zone to access to write to,
        e.g. ``IDATA``, ``DDATA``, or ``CODE``.

        The given number of bits, if provided, must be either ``8``, ``16``, or
        ``32``.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to write to
          data (list): list of data units to write
          zone (str): optional memory zone name to access
          nbits (int): number of bits to use for each unit

        Returns:
          Number of units written.

        Raises:
          JLinkException: on write hardware failure.
          ValueError: if ``nbits`` is not ``None``, and not in ``8``, ``16`` or
            ``32``.
        """
        buf_size = len(data)
        buf = None
        access = 0

        if nbits is None:
            # Pack the given data into an array of 8-bit unsigned integers in
            # order to write it successfully
            packed_data = map(lambda d: reversed(binpacker.pack(d)), data)
            packed_data = list(itertools.chain(*packed_data))

            buf_size = len(packed_data)
            buf = (ctypes.c_uint8 * buf_size)(*packed_data)

            # Allow the access width to be chosen for us.
            access = 0
        elif nbits == 8:
            buf = (ctypes.c_uint8 * buf_size)(*data)
            access = 1
        elif nbits == 16:
            buf = (ctypes.c_uint16 * buf_size)(*data)
            access = 2
            buf_size = buf_size * access
        elif nbits == 32:
            buf = (ctypes.c_uint32 * buf_size)(*data)
            access = 4
            buf_size = buf_size * access
        else:
            raise ValueError('Given bit size is invalid: %s' % nbits)

        args = [addr, buf_size, buf, access]

        method = self._dll.JLINKARM_WriteMemEx
        if zone is not None:
            method = self._dll.JLINKARM_WriteMemZonedEx
            args.append(zone.encode())

        units_written = method(*args)
        if units_written < 0:
            raise errors.JLinkWriteException(units_written)

        return units_written

    def memory_write8(self, addr, data, zone=None):
        """Writes bytes to memory of a target system.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to write to
          data (list): list of bytes to write
          zone (str): optional memory zone to access

        Returns:
          Number of bytes written to target.

        Raises:
          JLinkException: on memory access error.
        """
        return self.memory_write(addr, data, zone, 8)

    def memory_write16(self, addr, data, zone=None):
        """Writes half-words to memory of a target system.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to write to
          data (list): list of half-words to write
          zone (str): optional memory zone to access

        Returns:
          Number of half-words written to target.

        Raises:
          JLinkException: on memory access error.
        """
        return self.memory_write(addr, data, zone, 16)

    def memory_write32(self, addr, data, zone=None):
        """Writes words to memory of a target system.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to write to
          data (list): list of words to write
          zone (str): optional memory zone to access

        Returns:
          Number of words written to target.

        Raises:
          JLinkException: on memory access error.
        """
        return self.memory_write(addr, data, zone, 32)

    def memory_write64(self, addr, data, zone=None):
        """Writes long words to memory of a target system.

        Note:
          This is little-endian.

        Args:
          self (JLink): the ``JLink`` instance
          addr (int): start address to write to
          data (list): list of long words to write
          zone (str): optional memory zone to access

        Returns:
          Number of long words written to target.

        Raises:
          JLinkException: on memory access error.
        """
        words = []
        bitmask = 0xFFFFFFFF
        for long_word in data:
            words.append(long_word & bitmask)          # Last 32-bits
            words.append((long_word >> 32) & bitmask)  # First 32-bits
        return self.memory_write32(addr, words, zone=zone)

    def erase_chip(self):
        """
        Erases all user flash on the connected device.
        """
        result = self._windll.JLINK_EraseChip()
        if result < 0:
            errMsg = "Erase Chip Failed with Error code {}".format(result)
            self.abort(errMsg)
            return False
        return True

    def download(self, image, offset=0):
        if not os.path.isfile(image):
            print('Could not find image file %s' % image)

        file_name = ct.c_char_p(image)
        retval = self._windll.JLINK_DownloadFile(
            file_name, ct.c_uint32(offset))
        self.reset()

    @classmethod
    def ErrorOutHandler(cls, errorMsg):
        """
        Log error from the DLL.

        :param errorMsg: The DLL error message.
        """
        if cls.logger:
            cls.logger.error(errorMsg.data)

        cls.library_error_list.append(errorMsg.data)

    @classmethod
    def WarnOutHandler(cls, warnMsg):
        """
        Log warning from the DLL.

        :param warnMsg: The DLL error message.
        """
        if cls.logger:
            cls.logger.warn(warnMsg.data)
