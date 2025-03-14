/***************************************************************************//**
 * @file
 * @brief common unit test api
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: LicenseRef-MSLA
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of the Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement
 * By installing, copying or otherwise using this software, you agree to the
 * terms of the MSLA.
 *
 ******************************************************************************/

#ifndef _TEST_COMMON_H
#define _TEST_COMMON_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "unity.h"
#include "sl_iec60730_internal.h"

/*=======Result test case=====*/
#define MAX_NUMBER_OF_TEST_CASE    20

extern uint8_t test_case_result[MAX_NUMBER_OF_TEST_CASE];
extern uint8_t current_test_numth;

/*=======Test Runner Used To Run Each Test=====*/
extern void run_test(UnityTestFunction func, const char* name, UNITY_LINE_TYPE line_num);
/*=======Test Reset Options=====*/
extern void setUp(void);
extern void tearDown(void);
extern void resetTest(void);
/*=======Mock Management=====*/

/**************************************************************************/ /**
 * Public   Function set value simulate value function mock return.
 *
 * @returns None.
 *
 *****************************************************************************/
extern void cmock_set_value(sl_iec60730_test_result_t sl_iec60730_test_result_t);

/**************************************************************************/ /**
 * Public   Function get value simulate value function mock return.
 *
 * @returns None.
 *
 *****************************************************************************/
extern sl_iec60730_test_result_t cmock_return_value(void);

/**************************************************************************/ /**
 * Public   Function runs all unit test cases.
 *
 * @returns None.
 *
 *****************************************************************************/
extern void unit_test_run_all_test_cases(void);

#endif // _TEST_COMMON_H
