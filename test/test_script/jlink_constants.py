"""
jlink_constants.py:  Constants for JLinkARM.dll Wrapper - Python Edition

This module provides constants used by the JLinkARM.dll wrapper jlink.py.
"""
import ctypes as ct

class JLinkArmEmuConnectInfo_CStruct(ct.Structure):
    """ JLINKARM_EMU_CONNECT_INFO struct """
    _pack_ = 1
    _fields_ = [("SerialNumber", ct.c_uint32),
                ("Connection", ct.c_uint),
                ("USBAddr", ct.c_uint32),
                ("aIPAddr", ct.c_uint8 * 16),
                ("Time", ct.c_int),
                ("Time_us", ct.c_uint64),
                ("HWVersion", ct.c_uint32),
                ("abMACAddr", ct.c_uint8 * 6),
                ("acProduct", ct.c_char * 32),
                ("acNickName", ct.c_char * 32),
                ("acFWString", ct.c_char * 112),
                ("IsDHCPAssignedIP", ct.c_char),
                ("IsDHCPAssignedIPIsValid", ct.c_char),
                ("NumIPConnections", ct.c_char),
                ("NumIPConnectionsIsValid", ct.c_char),
                ("aPadding", ct.c_uint8 * 34)]


class JLinkJtagIdData_CStruct(ct.Structure):
    """ JLINKARM_EMU_CONNECT_INFO struct """
    _pack_ = 1
    _fields_ = [("NumDevices", ct.c_int),
                ("ScanLen", ct.c_uint16),
                ("aId", ct.c_uint32 * 3),
                ("aScanLen", ct.c_uint8 * 3),
                ("aIrRead", ct.c_uint8 * 3),
                ("aScanRead", ct.c_uint8 * 3)]


class JlinkArm_Speed_Info_CStruct(ct.Structure):
    """ JLINKARM_SPEED_INFO struct """
    _pack_ = 1
    _fields_ = [("SizeOfStruct", ct.c_uint32),
                ("BaseFreq", ct.c_uint32),
                ("MinDiv", ct.c_uint16),
                ("SupportAdaptive", ct.c_uint16)]
