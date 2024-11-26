/***************************************************************************//**
 * @file
 * @brief common unit test api
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "unit_test_common.h"

/*=======Result test case=====*/

uint8_t test_case_result[MAX_NUMBER_OF_TEST_CASE];
uint8_t current_test_numth;
static sl_iec60730_test_result_t cmock_value_return;

/*=======Mock Management=====*/
static void cmock_init(void)
{
}
static void cmock_verify(void)
{
}
static void cmock_destroy(void)
{
}

void cmock_set_value(sl_iec60730_test_result_t value)
{
  cmock_value_return = value;
}

sl_iec60730_test_result_t cmock_return_value(void)
{
  return cmock_value_return;
}
/*=======Test Reset Options=====*/
void setUp(void)
{
}

void tearDown(void)
{
}

void resetTest(void)
{
  tearDown();
  cmock_verify();
  cmock_destroy();
  cmock_init();
  setUp();
}
void verifyTest(void);
void verifyTest(void)
{
  cmock_verify();
}
/*=======Test Runner Used To Run Each Test=====*/
void run_test(UnityTestFunction func, const char* name, UNITY_LINE_TYPE line_num)
{
  Unity.CurrentTestName = name;
  Unity.CurrentTestLineNumber = line_num;
#ifdef UNITY_USE_COMMAND_LINE_ARGS
  if (!UnityTestMatches()) {
    return;
  }
#endif
  Unity.NumberOfTests++;
  UNITY_CLR_DETAILS();
  UNITY_EXEC_TIME_START();
  cmock_init();
  if (TEST_PROTECT()) {
    setUp();
    func();
  }
  if (TEST_PROTECT()) {
    tearDown();
    cmock_verify();
  }
  cmock_destroy();
  UNITY_EXEC_TIME_STOP();

  test_case_result[current_test_numth++] = Unity.CurrentTestFailed;

  UnityConcludeTest();
}
