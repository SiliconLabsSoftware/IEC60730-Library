/***************************************************************************/ /**
 * @file
 * @brief Library definitions
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

#ifndef SL_IEC60730_H
#define SL_IEC60730_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "sl_iec60730_config.h"
#include "sl_iec60730_toolchain.h"
#include "em_device.h"
#include "em_core.h"
#include "em_gpcrc.h"
#include SL_IEC60730_BOARD_HEADER

#define IE60730_LIBRARY_VERSION { 2, 0, 0 }

#ifdef DOXYGEN
/// User SHOULD define #SL_IEC60730_BOARD_HEADER definition for using IMC and VMC modules.
/// This definition SHOULD be a header file. In this header file SHOULD be
/// definitions listed in IMC and VMC modules. In our example we defined
/// the #SL_IEC60730_BOARD_HEADER definition is sl_iec60730_board.h. Our definitions
/// in #SL_IEC60730_BOARD_HEADER is an example that you can reference. Of course, user CAN
/// redefine the #SL_IEC60730_BOARD_HEADER definition to any header file.
#define SL_IEC60730_BOARD_HEADER

#else
#define IEC60370_CM4  0
#define IEC60370_CM33 1
#if defined(__CM4_REV)
#define IEC60370_CPU IEC60370_CM4
#elif defined(__CM33_REV)
#define IEC60370_CPU IEC60370_CM33
#else
#error "Unknown Core type!"
#endif

#endif /* DOXYGEN */

/**************************************************************************/ /**
 * @addtogroup efr32_iec60730
 * @{
 * @addtogroup sl_iec60730_post
 * @{
 *****************************************************************************/
typedef struct {
  uint8_t major;    ///< Major version
  uint8_t minor;    ///< Minor version
  uint8_t revision; ///< Revision
} sl_iec60730_library_version_t;

/// Global variable which holds the IEC60370 library version.
extern const sl_iec60730_library_version_t SL_IEC60730_LIBRARY_VERSION;

/// The result of a test.
typedef enum {
  SL_IEC60730_TEST_FAILED      = 0, ///< test result failed
  SL_IEC60730_TEST_PASSED      = 1, ///< test result passed
  SL_IEC60730_TEST_IN_PROGRESS = 2, ///< test is still in progress
} sl_iec60730_test_result_t;

/// Cause of failure passed to Safe State function.
typedef enum {
  SL_IEC60730_NO_FAILURE           = 0, ///< No failure detected
  SL_IEC60730_TEST_VERIFY          = 1, ///< Special case used for automated verification testing
  SL_IEC60730_WATCHDOG_POST_FAIL   = 2, ///< Watchdog POST test failed
  SL_IEC60730_VMC_FAIL             = 3, ///< Variable Memory Check detected a failure
  SL_IEC60730_IMC_FAIL             = 4, ///< Invariable Memory Check detected a failure
  SL_IEC60730_CPU_REGISTERS_FAIL   = 5, ///< CPU Registers Check detected a failure
  SL_IEC60730_CLOCK_FAIL           = 6, ///< System Clock Check detected a failure
  SL_IEC60730_INTERRUPT_FAIL       = 7, ///< Interrupt Plausibility Check detected a failure
  SL_IEC60730_PROGRAM_COUNTER_FAIL = 8, ///< Program Counter Check detected a failure
  SL_IEC60730_GPIO_FAIL            = 9, ///< GPIO Plausibility Check detected a failure
  SL_IEC60730_ANALOG_FAIL          = 10, ///< Analog Plausibility Check detected a failure
  SL_IEC60730_CS0_FAIL             = 11, ///< CSEN0 detected a failureLESENSE
  SL_IEC60730_LESENSE0_FAIL        = 12, ///< LESENSE0 detected a failure
  SL_IEC60730_RADIO_FAIL           = 13, ///< Radio comms detected a failure
  SL_IEC60730_I2C0_FAIL            = 14, ///< I2C0 comms channel detected a failure
  SL_IEC60730_I2C1_FAIL            = 15, ///< I2C1 comms channel detected a failure
  SL_IEC60730_USART0_FAIL          = 16, ///< USART0 comms channel detected a failure
  SL_IEC60730_USART1_FAIL          = 17, ///< USART1 comms channel detected a failure
  SL_IEC60730_USART2_FAIL          = 18, ///< USART2 comms channel detected a failure
  SL_IEC60730_USART3_FAIL          = 19, ///< USART3 comms channel detected a failure
  SL_IEC60730_LEUART0_FAIL         = 20, ///< LEUART comms channel detected a failure
  SL_IEC60730_EUART0_FAIL          = 21, ///< EUART comms channel detected a failure
  SL_IEC60730_PDM0_FAIL            = 22, ///< PDM comms channel detected a failure
  SL_IEC60730_OEM_FAIL_1           = 23, ///< OEM defined failure
  SL_IEC60730_OEM_FAIL_2           = 24, ///< OEM defined failure
  SL_IEC60730_OEM_FAIL_3           = 25, ///< OEM defined failure
  SL_IEC60730_OEM_FAIL_4           = 26, ///< OEM defined failure
  SL_IEC60730_SAFETY_CHECK_FAIL    = 0XFF, ///< iec60730_safety_check detected some failure
} sl_iec60730_test_failure_t;

/// Useful macro for avoiding compiler warnings related to unused function
/// arguments or unused variables.
#ifndef UNUSED_VAR
#define UNUSED_VAR(x) (void) (x)
#endif /* UNUSED_VAR */

/// Macro merge separate words to be joined into a single word
#ifndef __CONCAT
#define __CONCAT(x, y) x##y
#endif

#define CONCAT(x, y) __CONCAT(x, y)

/// Macro convert to string
#define STR(x)  #x
#define XSTR(x) STR(x)

/// The #LABEL_DEF macro is used to generate label for break points. Using this
/// macro avoids label removal by the compiler's optimize levels.
#ifndef LABEL_DEF
#define LABEL_DEF(NAME) LB_ASM volatile(LB_XSTR(NAME))
#endif /* LABEL_DEF */

#ifdef DOXYGEN
/// The #__no_init macro is used for compatibility between
/// GCC and IAR compiler.
#define __no_init
#else // !DOXYGEN
#ifndef __no_init
#define __no_init
#endif /* __no_init */
#endif // DOXYGEN

/// The #DEC_CLASSB_VARS definition is used for declaring pair of classsb_ram
/// variables.
#define DEC_CLASSB_VARS(vartype, name) \
  __no_init vartype name __CLASSB_RAM; \
  __no_init vartype CONCAT(name, _inv) __CLASSB_RAM

/// The #EXTERN_DEC_CLASSB_VARS definition is extern of pair of
/// #DEC_CLASSB_VARS
#define EXTERN_DEC_CLASSB_VARS(vartype, name) \
  extern __no_init vartype name __CLASSB_RAM; \
  extern __no_init vartype CONCAT(name, _inv) __CLASSB_RAM

/// The #sl_static_dec_classb_vars definition used for declaring static pair
/// #DEC_CLASSB_VARS
#define sl_static_dec_classb_vars(vartype, name) \
  static __no_init vartype name __CLASSB_RAM;    \
  static __no_init vartype CONCAT(name, _inv) __CLASSB_RAM

/// The #INV_CLASSB_VAR macro is used to inverse a Inv value of pair
/// #DEC_CLASSB_VARS
#define INV_CLASSB_VAR(vartype, name) \
  CONCAT(name, _inv) = (vartype) (~(vartype) name)

/// The #INV_CLASSB_PVAR macro is used to inverse a Inv value of pointer pair
/// #DEC_CLASSB_VARS
#define INV_CLASSB_PVAR(vartype, name) \
  CONCAT(name, _inv) = (vartype *) (~(vartype) name)

#if ((defined UNIT_TEST_IEC60730_VARIABLE_MEMORY_ENABLE) || defined (UNIT_TEST_IEC60730_INVARIABLE_MEMORY_ENABLE))
#define CHECK_INTEGRITY(vartype, x)  unit_test_mock_check_integrity()
#else
/// The #CHECK_INTEGRITY definition used for checking integrity of pair of
/// variables stored in *.classb_ram* section.
#define CHECK_INTEGRITY(vartype, x) \
  ((vartype) - 1 == ((vartype) x ^ (vartype) CONCAT(x, _inv)))
#endif // (UNIT_TEST_IEC60730_VARIABLE_MEMORY_ENABLE || UNIT_TEST_IEC60730_INVARIABLE_MEMORY_ENABLE)

/** @} (end addtogroup sl_iec60730_post) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_INVARIABLE_MEMORY_TEST
 * @{
 *****************************************************************************/

#ifdef DOXYGEN
/// Support for cleaner code.
///   * If user use the #SL_IEC60730_USE_CRC_32_ENABLE == 1, #sl_iec60730_crc_t is uint32_t.
///   * Otherwise, #sl_iec60730_crc_t is uint16_t.
#define sl_iec60730_crc_t

/// The #SL_IEC60730_INVAR_BLOCKS_PER_BIST desribles maximum number of loops that perform
/// the CRC calculation each time the #sl_iec60730_imc_bist is invoked. We provide
/// default value for this definition in sl_iec60730.h file.
#define SL_IEC60730_INVAR_BLOCKS_PER_BIST

#else // !DOXYGEN

#if (SL_IEC60730_USE_CRC_32_ENABLE == 1)
/// CRC calculation data type
typedef uint32_t sl_iec60730_crc_t;
#else /* !SL_IEC60730_USE_CRC_32_ENABLE */
/// CRC calculation data type
typedef uint16_t sl_iec60730_crc_t;
#endif /* SL_IEC60730_USE_CRC_32_ENABLE */

#endif // DOXYGEN

/// The end address of Flash is specified by the address of the #check_sum
/// variable. This variable is provided by linker in GCC or generated by
/// configurations in IAR.
extern sl_iec60730_crc_t check_sum;

/// The #SL_IEC60730_ROM_SIZE definition describes the size of Flash calculated CRC value.
/// This calculation based on #SL_IEC60730_ROM_END, and #SL_IEC60730_ROM_START definitions.
#define SL_IEC60730_ROM_SIZE          ((uint32_t) SL_IEC60730_ROM_END - (uint32_t) SL_IEC60730_ROM_START)

/// The #STEPS_NUMBER definitionn WILL be defined base on #SL_IEC60730_ROM_SIZE and
/// #SL_IEC60730_FLASH_BLOCK definition. User SHOULD not change it.
#define STEPS_NUMBER                  ((uint32_t) SL_IEC60730_ROM_SIZE / SL_IEC60730_FLASH_BLOCK)

/// The #SL_IEC60730_ROM_SIZE_INWORDS definitionn WILL be defined base on #SL_IEC60730_ROM_SIZE definition.
/// User SHOULD not change it.
#define SL_IEC60730_ROM_SIZE_INWORDS  ((uint32_t) SL_IEC60730_ROM_SIZE / 4U)

/// The #SL_IEC60730_FLASH_BLOCK_WORDS definitionn WILL be defined base on #SL_IEC60730_ROM_SIZE_INWORDS
/// and #STEPS_NUMBER  definitions. User SHOULD not change it.
#define SL_IEC60730_FLASH_BLOCK_WORDS ((uint32_t) (SL_IEC60730_ROM_SIZE_INWORDS / STEPS_NUMBER))

/// The #SL_IEC60730_ROM_SIZE_TEST definition describes the size of region Flash calculated CRC value.
/// This calculation based on #end address, and #start address region is transmitted.
/// definitions.
#define SL_IEC60730_ROM_SIZE_TEST(start, end) \
  ((uint32_t) end - (uint32_t) start)

/// The #STEPS_NUMBER_TEST definitionn WILL be defined base on #SL_IEC60730_ROM_SIZE_TEST and
/// #SL_IEC60730_FLASH_BLOCK definition. User SHOULD not change it.
#define STEPS_NUMBER_TEST(start, end) \
  ((uint32_t) SL_IEC60730_ROM_SIZE_TEST(start, end) / SL_IEC60730_FLASH_BLOCK)

/// The #SL_IEC60730_ROM_SIZE_INWORDS_TEST definitionn WILL be defined base on #SL_IEC60730_ROM_SIZE_TEST definition.
/// User SHOULD not change it.
#define SL_IEC60730_ROM_SIZE_INWORDS_TEST(start, end)  ((uint32_t) SL_IEC60730_ROM_SIZE_TEST(start, end) / 4U)

/// The #SL_IEC60730_FLASH_BLOCK_WORDS_TEST definitionn WILL be defined base on #SL_IEC60730_ROM_SIZE_INWORDS_TEST
/// and #STEPS_NUMBER_TEST  definitions. User SHOULD not change it.
#define SL_IEC60730_FLASH_BLOCK_WORDS_TEST(start, end) ((uint32_t) (SL_IEC60730_ROM_SIZE_INWORDS_TEST(start, end) / STEPS_NUMBER_TEST(start, end)))

/// The #SL_IEC60730_REF_CRC definition describes variable that address of this variable
/// store CRC value that is calculated by \ref sl_iec60730_call_crcXY in Post Build. We use
/// #check_sum so in our example #SL_IEC60730_REF_CRC definition is #check_sum.
#ifndef SL_IEC60730_REF_CRC
#define SL_IEC60730_REF_CRC (check_sum)
#endif

/** @} (end addtogroup IEC60730_INVARIABLE_MEMORY_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_VARIABLE_MEMORY_TEST
 * @{
 *****************************************************************************/

/// The #BLOCKSIZE definition MUST be 4 as our example for the algorithm to
/// work properly.
#define BLOCKSIZE         (4)

/// The #RT_BLOCK_OVERLAP definition is used to present the size of
/// #iec60730_ram_test_overlap buffer. The variable #iec60730_ram_test_overlap has a size of
/// (2 * #RT_BLOCK_OVERLAP). For the algorithm (March-X/March-C) to work
/// properly this definition MUST take the value 1 as in our example.
#define RT_BLOCK_OVERLAP  (1)

/// The definition of #RT_BLOCKSIZE is taken from the definition of #BLOCKSIZE
/// and #RT_BLOCK_OVERLAP.
#define RT_BLOCKSIZE      (BLOCKSIZE + (RT_BLOCK_OVERLAP << 1))

/// The #BACKGROUND definition is background used to testing. In our sample,
/// value 0x00000000uL is used value for background. Of course, we can define
/// another value. We recommend using definition like ours (0x00000000uL) because
/// it accurately describes the steps of the March-X/March-C algorithm
/// described above.
#ifndef BACKGROUND
#define BACKGROUND        ((uint32_t) 0x00000000uL)
#endif

/** @} (end addtogroup IEC60730_VARIABLE_MEMORY_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_SAFE_STATE
 * @{
 * The Safe State function must be written by the OEM to configure the
 * system to prevent any potential negative effects from occurring. GPIOs and
 * communications channels must be configured appropriately.
 *
 * Once any configuration is complete, the function loops to restart the watchdog.
 * This prevents any resets until the end-user power-cycles the device.
 *
 *****************************************************************************/

/**************************************************************************/ /**
 * public IEC60730 Safe State
 *
 * @param failure Enum with the failing test, can be preserved for debug.
 * See \link #sl_iec60730_test_failure_t sl_iec60730_test_failure_t \endlink
 *
 * @return Never.
 *
 * When a validation failure is detected, the Safe State function is
 * executed to prevent further system failures.
 * @note This is an example; an OEM must fully implement this according
 * to their system needs.
 *****************************************************************************/
void sl_iec60730_safe_state(sl_iec60730_test_failure_t failure);
/** @} (end addtogroup IEC60730_SAFE_STATE) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_PROGRAM_COUNTER
 * @{
 *****************************************************************************/

/// Global variable used by BIST to determine if modules are running
/// frequently enough to meet maximum fault time.
///
/// #sl_iec60730_program_counter_check equals 0 before BIST check. Each module
/// then sets its assigned bit once it has completed testing. Before the
/// maximum fault time, sl_iec60730_program_counter_test() verifies that the
/// value is @ref IEC60730_ALL_COMPLETE_BITS.
extern uint16_t sl_iec60730_program_counter_check;

/// Mask bit used for Variable Memory Checks completed
#define IEC60730_VMC_COMPLETE         (1 << 0)
/// Mask bit used for Invariable Memory Checks completed
#define IEC60730_IMC_COMPLETE         (1 << 1)
/// Mask bit used for CPU Clocks Checks completed
#define IEC60730_CPU_CLOCKS_COMPLETE  (1 << 2)
/// Mask bit used for CPU Register Checks completed
#define IEC60730_CPU_REGS_COMPLETE    (1 << 3)
/// Mask bit used for Interrupt Checks completed
#define IEC60730_INTERRUPT_COMPLETE   (1 << 4)
/// Mask bit used for GPIO Plausibility Checks completed
#define IEC60730_GPIO_COMPLETE        (1 << 5)
/// Mask bit used for Analog Plausibility Checks completed
#define IEC60730_ANALOG_COMPLETE      (1 << 6)
/// Mask bit used for Communications Plausibility Checks completed
#define IEC60730_COMMS_COMPLETE       (1 << 7)
/// Mask bit used for OEM Check 0 completed
#define IEC60730_OEM0_COMPLETE        (1 << 8)
/// Mask bit used for OEM Check 1 completed
#define IEC60730_OEM1_COMPLETE        (1 << 9)
/// Mask bit used for OEM Check 2 completed
#define IEC60730_OEM2_COMPLETE        (1 << 10)
/// Mask bit used for OEM Check 3 completed
#define IEC60730_OEM3_COMPLETE        (1 << 11)
/// Mask bit used for OEM Check 4 completed
#define IEC60730_OEM4_COMPLETE        (1 << 12)
/// Mask bit used for OEM Check 5 completed
#define IEC60730_OEM5_COMPLETE        (1 << 13)
/// Mask bit used for OEM Check 6 completed
#define IEC60730_OEM6_COMPLETE        (1 << 14)
/// Mask bit used for OEM Check 7 completed
#define IEC60730_OEM7_COMPLETE        (1 << 15)
/// Mask used for checking that all bits are set
#define IEC60730_ALL_COMPLETE_BITS                                            \
  (IEC60730_VMC_COMPLETE | IEC60730_IMC_COMPLETE                              \
   | IEC60730_CPU_CLOCKS_COMPLETE | IEC60730_CPU_REGS_COMPLETE                \
   | IEC60730_INTERRUPT_COMPLETE | IEC60730_GPIO_COMPLETE                     \
   | IEC60730_ANALOG_COMPLETE | IEC60730_COMMS_COMPLETE                       \
   | IEC60730_OEM0_COMPLETE | IEC60730_OEM1_COMPLETE | IEC60730_OEM2_COMPLETE \
   | IEC60730_OEM3_COMPLETE | IEC60730_OEM4_COMPLETE | IEC60730_OEM5_COMPLETE \
   | IEC60730_OEM6_COMPLETE | IEC60730_OEM7_COMPLETE)

/** @} (end addtogroup IEC60730_PROGRAM_COUNTER) */

/**************************************************************************/ /**
 * @addtogroup sl_iec60730_post
 * @{
 * sl_iec60730_post() validates system health before starting execution
 * of the main loop.
 * It must be called first in the main() function. If the result of a test
 * routine is a failure, sl_iec60730_safe_state() is called.
 * \anchor post_flowchart
 * \image html post_flowchart.png "Figure 1 Post flowchart"
 *****************************************************************************/

/**************************************************************************/ /**
 * public IEC60730 Power On Self Test
 *
 * @returns None.
 *
 * Each module is tested to validate system health before starting the execution of the main loop, and if a failure is detected sl_iec60730_safe_state()
 * is called.
 *****************************************************************************/
void sl_iec60730_post(void);
/** @} (end addtogroup sl_iec60730_post) */

/**************************************************************************/ /**
 * @addtogroup sl_iec60730_bist
 * @{
 * sl_iec60730_bist() confirms correct operation of the device during
 * main loop execution. If a module validation fails, sl_iec60730_safe_state()
 * is called.
 *
 * OEM code must call #sl_iec60730_safety_check_error_occur to
 * set **iec60730_safety_check** to one of the failure conditions in #sl_iec60730_test_failure_t
 * if the code determines a failure has occurred. This can be used for
 * communications channels or to trigger Safe State entry due to a failure within an interrupt.
 *
 * The validation time for the full memory varies between
 * devices depending on the size of memory available, and how frequently 
 * sl_iec60730_bist() is called by the main loop. For details see
 * @ref bist_frequency.
 *
 * Periodic BIST execution is shown in \link bist_flowchart Figure 1\endlink
 *
 * @anchor bist_flowchart
 * @image html bist_flowchart.png "Figure 1 Flow chart of periodic BIST execution"
 *****************************************************************************/

/// typedef struct used to relay
/// and remember status as bit field is type of local variable **iec60730_safety_check** save
/// IEC60730 safety code error. When a failure is detected, function
/// #sl_iec60730_safety_check_error_occur is called by OEM and
/// interrupt check functions to set **iec60730_safety_check** to type of sl_iec60730_test_failure_t.
typedef struct {
  uint32_t error;
  uint8_t number_error;
} sl_iec60730_safety_check_t;

/**************************************************************************/ /**
 * public IEC60730 save error to variable **iec60730_safety_check**
 *
 * @param failure Enum with the failing test, can be preserved for debug.
 *
 * @returns None.
 *
 * Called by OEM and interrupt check functions to set **iec60730_safety_check**
 * to #sl_iec60730_test_failure_t.
 *****************************************************************************/
void sl_iec60730_safety_check_error_occur(sl_iec60730_test_failure_t failure);

/**************************************************************************/ /**
 * public IEC60730 reset value local **iec60730_safety_check**
 *
 * @returns None
 *
 * Called by OEM when wanting to reset local variable value **iec60730_safety_check**
 * which containing the errors that occurred and the number of errors
 *****************************************************************************/
void sl_iec60730_safety_check_reset_error(void);

/**************************************************************************/ /**
 * public IEC60730 return value variable **iec60730_safety_check**
 *
 * @returns pointer type sl_iec60730_safety_check_t point to variable containing
 * the errors that occurred and the number of errors
 *
 * Called by OEM in sl_iec60730_safe_state functions to get value **iec60730_safety_check**,
 * so OEM can get all #sl_iec60730_test_failure_t occur
 *****************************************************************************/
sl_iec60730_safety_check_t* sl_iec60730_safety_check_get_error(void);

/**************************************************************************/ /**
 * public IEC60730 Built In Self Test
 *
 *
 * @returns None. If validation fails function never returns. If validation
 * passes, function returns.
 *
 * Each module is tested, and if a failure is detected sl_iec60730_safe_state()
 * is called. If a test failure occurs outside this function (such as comms),
 * called function #sl_iec60730_safety_check_error_occur set **iec60730_safety_check**
 * to #sl_iec60730_test_failure_t.
 *****************************************************************************/
void sl_iec60730_bist(void);
/** @} (end addtogroup sl_iec60730_bist) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_WDOG_TEST
 * @{
 *****************************************************************************/

/**************************************************************************/ /**
 * public IEC60730 Watchdog Restart
 *
 * @returns None.
 *
 * This function abstracts the differences between the EFR32 family watchdog
 * timers. OEMs can call it during long duration executions to restart
 * the watchdog timer.
 *****************************************************************************/
void sl_iec60730_restart_watchdogs(void);

/** @} (end addtogroup IEC60730_WDOG_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_SYSTEM_CLOCK_TEST
 * @{
 *****************************************************************************/

/// Enable/disable switch for system timer test
typedef enum {
  SL_IEC60730_TIMER_TEST_DISABLE = 0, ///< timer tests will not execute
  SL_IEC60730_TIMER_TEST_ENABLE  = 1, ///< timer tests will execute
} sl_iec60730_timer_test_control_t;

/// OEMs use the defines SL_IEC60730_SYS_CLOCK_TEST_CLK_FREQ and SL_IEC60730_SYS_CLOCK_TEST_TOLERANCE
/// in the config file to define the expected tolerance and clock frequency
/// of executions for each test clock tick included in a project.
/// OEM code need to disable ISR-based timer test execution when
/// in safe states.
/// This local variable allows OEM code to test when events occur.
/// static uint16_t sl_iec60730_number_test_timer_tick;
/// Provides the expected ratio of system clock to test clock ticks
/// Allows OEM code to run tests using timer tick.
/// Must be defined as system clock (faster clock) / test clock (slower clock).
/// The ratio value cannot exceed 0xFFFF.
///
/// This local variable allows customers to stop ISR-based tests
/// when code enters a safe state where BIST is not being called because the
/// code is not safety critical. Tests whose execution is gated by this
/// value are the system clock test, the interrupt plausibility
/// test, and the BIST execution frequency test.
/// static uint8_t sl_iec60730_timer_test_control
///
/// This local variable allows customers to know the number of times
/// the function sl_iec60730_test_clock_tick is called, used for customers testing
/// static uint16_t sl_iec60730_number_test_timer_tick

/**************************************************************************/ /**
 * public IEC60730 System Clock Tick
 *
 * @returns None.
 *
 * This function increments a system clock counter **sl_iec60730_sys_clock_count**,
 * which is compared to a test clock counter as part of the system clock frequency check.
 * It should be called in the timer interrupt service routine designated as the system
 * clock timer in oem_iec60730_timer.c.
 *****************************************************************************/
void sl_iec60730_sys_clock_count_tick(void);

/**************************************************************************/ /**
 * public IEC60730 Test Clock Tick
 *
 * @returns None.
 *
 * This function increments a test clock counter.  The function executes
 * all interrupt service routine-based IEC60730 tests, including the system
 * clock frequency check.  It should be called in the timer interrupt
 * service routine designated as the test clock timer in oem_iec60730_timer.c.
 *****************************************************************************/
void sl_iec60730_test_clock_tick(void);

/**************************************************************************/ /**
 * public IEC60730 Initialize iec60730 timer tick variables
 *
 * @returns None.
 *
 * This function should be called within the sl_iec60730_test_clock_tick()
 * callback.  During the initialization of the test clock timers, this
 * function resets the internal test clock and system clock tick variables
 * to known reset states.
 *****************************************************************************/
void sl_iec60730_sys_clock_count_reset(void);

/** @} (end addtogroup IEC60730_SYSTEM_CLOCK_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_IRQ_TEST
 * @{
 *****************************************************************************/

/// The maximum number of interrupted users that can be used for testing.
/// The param passed to the init function sl_iec60730_irq_init() must have a size value
/// smaller than this value
#define IEC60730_MAX_IRQ_CHECK          32

/// OEMs use this structure to define the expected min and max number
/// of executions for each interrupt included in a project.
typedef struct {
  SL_IEC60730_IRQ_TYPE_VARIABLE
    min;   ///< Minimum expected executions of ISR.  Can be 0-255, must be less than or equal to max.
  SL_IEC60730_IRQ_TYPE_VARIABLE
    max;   ///< Maximum executed executions of ISR.  Can be 0-255, must be greater than or equal to min
} sl_iec60730_irq_execution_bounds_t;

/// Struct contains information about the IRQ number and
/// the location of the failed IRQ in the order of the OEM array
// registered to check.
typedef struct {
  uint32_t irq_fail;
  uint8_t num_irq_fail;
} sl_iec60730_irq_fail_t;

/// Struct contains a pointer variable point to the number of execution interrupts and
/// the boundary of each interrupt count variable.
/// OEMs use this structure to init value.
/// Defines SL_IEC60730_IRQ_TYPE_VARIABLE using for set type variable **irq_count**.
/// This count is used by the test to step through each entry in
/// **irq_bounds** to determine if the count for each interrupt
/// exceeds expected min/max bounds. OEM code is responsible for
/// incrementing **irq_count** of this array corresponding to the IRQ described
/// by the corresponding element in **irq_bounds**. The
/// incrementing instruction should only be placed inside the IRQ referred
/// to by the element. The element will be reset to 0 by the interrupt
/// plausibility check executing in #sl_iec60730_test_clock_tick().
/// The interrupt plausibility test, which executes at the end of a test clock period,
/// compares the number of ISR executions for each ISR as defined in the
/// **irq_count** array.  A count outside the defined bounds results
/// in a safe state entry.
typedef struct {
  volatile SL_IEC60730_IRQ_TYPE_VARIABLE* irq_count;
  const sl_iec60730_irq_execution_bounds_t* irq_bounds;
  uint8_t size;
}sl_iec60730_irq_cfg_t;
/** @} (end addtogroup IEC60730_IRQ_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_INVARIABLE_MEMORY_TEST
 * @{
 *****************************************************************************/

/// Enter ATOMIC section.
#define SL_IEC60370_ENTER_ATOMIC()  CORE_ENTER_ATOMIC()
/// Exit ATOMIC section.
#define SL_IEC60370_EXIT_ATOMIC()   CORE_EXIT_ATOMIC()
/// Declare irq state
#define SL_IEC60370_DECLARE_IRQ_STATE CORE_DECLARE_IRQ_STATE;

/// Enter ATOMIC section of IMC Post
#define SL_IEC60730_IMC_POST_ENTER_ATOMIC() \
  CORE_DECLARE_IRQ_STATE;                   \
  CORE_ENTER_ATOMIC()

/// Exit ATOMIC section of IMC Post
#define SL_IEC60730_IMC_POST_EXIT_ATOMIC() CORE_EXIT_ATOMIC()

/// Enter ATOMIC section of IMC Bist
#define SL_IEC60730_IMC_BIST_ENTER_ATOMIC() \
  CORE_DECLARE_IRQ_STATE;                   \
  CORE_ENTER_ATOMIC()

/// Exit ATOMIC section of IMC Bist
#define SL_IEC60730_IMC_BIST_EXIT_ATOMIC() CORE_EXIT_ATOMIC()

/// Initialize the General Purpose Cyclic Redundancy Check (GPCRC) module.
#define SL_IEC60730_CRC_INIT(crc, init)               GPCRC_Init(crc, init)
/// Reset GPCRC registers to the hardware reset state.
#define SL_IEC60730_CRC_RESET(crc)                    GPCRC_Reset(crc)
/// Issues a command to initialize CRC calculation.
#define SL_IEC60730_CRC_START(crc)                    GPCRC_Start(crc)
/// Writes a 32-bit value to the input data register of the CRC.
#define SL_IEC60730_CRC_INPUTU32(crc, d)              GPCRC_InputU32(crc, d)
/// Writes a 32-bit value to the input data register of the CRC.
#define SL_IEC60730_CRC_INPUTU16(crc, d)              GPCRC_InputU16(crc, d)
/// Writes a 8-bit value to the input data register of the CRC.
#define SL_IEC60730_CRC_INPUTU8(crc, d)               GPCRC_InputU8(crc, d)
/// Reads the data register of the CRC.
#define SL_IEC60730_CRC_DATA_READ(crc)                GPCRC_DataRead(crc)
/// Reads the data register of the CRC.
#define SL_IEC60730_CRC_DATA_READ_BIT_REVERSED(crc)   GPCRC_DataReadBitReversed(crc)
/// Reads the data register of the CRC.
#define SL_IEC60730_CRC_DATA_READ_BYTE_REVERSED(crc)  GPCRC_DataReadByteReversed(crc)

// CRC
/// the General Purpose Cyclic Redundancy Check (GPCRC) module
typedef GPCRC_TypeDef  sl_iec60730_crc_typedef;
/// CRC initialization structure
typedef GPCRC_Init_TypeDef sl_iec60730_crc_init_typedef;

/// The way to read CRC value when using function #sl_iec60730_update_crc_with_data_buffer
typedef uint8_t sl_iec60730_read_type_t;
/// The way to read CRC value when using function #sl_iec60730_update_crc_with_data_buffer
enum {
  SL_IEC60730_IMC_DATA_READ = 0, ///< use function GPCRC_DataRead to read CRC
  SL_IEC60730_IMC_DATA_READ_BIT_REVERSED =
    1,   ///< use function GPCRC_DataReadBitReversed to read CRC
  SL_IEC60730_IMC_DATA_READ_BYTE_REVERSED =
    2,   ///< use function GPCRC_DataReadByteReversed to read CRC
};

/// This structure is used as configuration for IMC testing, that holds GPCRC Register.
typedef struct {
  sl_iec60730_crc_typedef *gpcrc; ///< GPCRC Register
} sl_iec60730_imc_params_t;

/// This structure is used as configuration for CRC Buffer calculation.
/// It used when using function #sl_iec60730_update_crc_with_data_buffer
/// @note Struct #sl_iec60730_crc_init_typedef defined in header file "em_gpcrc.h"
typedef struct {
#if (SL_IEC60730_CRC_USE_SW_ENABLE == 0)
  sl_iec60730_imc_params_t hal;       ///< struct #sl_iec60730_imc_params_t that contain GPCRC Register
  sl_iec60730_crc_init_typedef init; ///< CRC initialization structure.
  sl_iec60730_read_type_t readType;   ///< The way to read calculated CRC value
#endif /* SL_IEC60730_CRC_USE_SW_ENABLE */
  sl_iec60730_crc_t xorOut;          ///< XOR with calculated CRC value
} sl_iec60730_update_crc_params_t;

/// This structure is used as configuration for IMC testing. The variable contains information about the starting and ending address of the region you want to check.
typedef struct {
  uint32_t *start; ///< Start address of FLASH to check
  uint32_t *end;   ///< End address of FLASH to check
} sl_iec60730_imc_test_region_t;

/// This structure is used for multiple test regions for IMC testing
/// with the pointer pointing to the array containing the start and end values ​​of the regions you want to test
/// and the number of regions you want to test.
typedef struct {
  const sl_iec60730_imc_test_region_t *region;
  uint8_t number_of_test_regions; ///< Number of test regions
} sl_iec60730_imc_test_multiple_regions_t;

#ifdef DOXYGEN
/// This macro is the default of GPCRC Register.
///   * Default #SL_IEC60730_DEFAULT_GPRC is GPCRC (also GPCRC_NS).
///   * If the **SL_IEC60730_NON_SECURE_ENABLE** is not defined, and **SL_TRUSTZONE_SECURE** is defined. #SL_IEC60730_DEFAULT_GPRC is GPCRC_S.
#define SL_IEC60730_DEFAULT_GPRC
#else
#ifndef SL_IEC60730_DEFAULT_GPRC
#if (_SILICON_LABS_32B_SERIES == 2)
#if ((defined SL_IEC60730_NON_SECURE_ENABLE) || (!defined(SL_TRUSTZONE_SECURE)))
/// This macro is the default of GPCRC Register.
#define SL_IEC60730_DEFAULT_GPRC GPCRC_NS
#else
/// This macro is the default of GPCRC Register.
#define SL_IEC60730_DEFAULT_GPRC GPCRC
#endif
#else // (_SILICON_LABS_32B_SERIES == 2)
/// This macro is the default of GPCRC Register.
#define SL_IEC60730_DEFAULT_GPRC GPCRC
#endif // (_SILICON_LABS_32B_SERIES == 2)
#endif // !SL_IEC60730_DEFAULT_GPRC
#endif // DOXYGEN

/// This macro is the sample test buffer used for testing CRC algorithm. This
/// value is used in development phase. User DOES NOT take care this definition.
#define SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_TEST "123456789"

#ifdef DOXYGEN
/// This macro is the initial value used for CRC calculations.
/// User DOES NOT change this value
///   * <b>CRC-16</b>: 0x0000 (SW) <b>CRC-32</b>: 0xFFFFFFFFuL (SW)
///   * <b>CRC-16</b>: 0xFFFF (HW) <b>CRC-32</b>: 0xFFFFFFFFuL (HW)
#define SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE

/// This macro is the value that will XOR with calculated CRC value to get CRC
/// output value. User DOES NOT change this value
///   * <b>CRC-16</b>: 0x0000 (SW) <b>CRC-32</b>: 0xFFFFFFFFuL (SW)
///   * <b>CRC-16</b>: 0x0000 (HW) <b>CRC-32</b>: 0xFFFFFFFFuL (HW)
#define SL_IEC60730_IMC_CRC_BUFFER_XOR_OUTPUT

/// This macro is the expected CRC value with input buffer that equal to
/// "123456789" and init value CRC is 0x0000 (CRC16) or 0xFFFFFFFF (CRC32) . These value are only used in development phase. User DOES NOT
/// take care this definition.
///   * <b>CRC-16</b>: 0x31C3 <b>CRC-32</b>: 0xCBF43926
#define SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_RESULT

/// This macro is the default value of struct #sl_iec60730_update_crc_params_t. User DOES NOT
/// change this value
#define SL_IEC60730_IMC_CRC_BUFFER_UPDATE_DEFAULT

#else

#if (SL_IEC60730_CRC_USE_SW_ENABLE == 1)
#if (SL_IEC60730_USE_CRC_32_ENABLE == 1)
#define SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE       SL_IEC60730_IMC_INIT_VALUE
#define SL_IEC60730_IMC_CRC_BUFFER_XOR_OUTPUT       (0xFFFFFFFFuL)
#define SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_RESULT    (0xCBF43926)
#define SL_IEC60730_IMC_CRC_BUFFER_UPDATE_DEFAULT   { SL_IEC60730_IMC_CRC_BUFFER_XOR_OUTPUT }
#else /* !SL_IEC60730_USE_CRC_32_ENABLE  */
#define SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE       SL_IEC60730_IMC_INIT_VALUE
#define SL_IEC60730_IMC_CRC_BUFFER_XOR_OUTPUT       (0x0000)
#define SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_RESULT    (0x31C3)
#define SL_IEC60730_IMC_CRC_BUFFER_UPDATE_DEFAULT   { SL_IEC60730_IMC_CRC_BUFFER_XOR_OUTPUT }
#endif /* SL_IEC60730_USE_CRC_32_ENABLE  */
#else /* !SL_IEC60730_CRC_USE_SW_ENABLE */
#if (SL_IEC60730_USE_CRC_32_ENABLE == 1)
#define SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE       SL_IEC60730_IMC_INIT_VALUE
#define SL_IEC60730_IMC_CRC_BUFFER_XOR_OUTPUT       (0xFFFFFFFFuL)
#define SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_RESULT    (0xCBF43926)
#define SL_IEC60730_IMC_CRC_BUFFER_INIT_DEFAULT \
  {                                             \
    0x04C11DB7UL,                               \
    SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE,      \
    false,                                      \
    false,                                      \
    false,                                      \
    false,                                      \
    true,                                       \
  }

#define SL_IEC60730_IMC_CRC_BUFFER_UPDATE_DEFAULT \
  { { SL_IEC60730_DEFAULT_GPRC },                 \
    SL_IEC60730_IMC_CRC_BUFFER_INIT_DEFAULT,      \
    SL_IEC60730_IMC_DATA_READ,                    \
    SL_IEC60730_IMC_CRC_BUFFER_XOR_OUTPUT }
#else /* !SL_IEC60730_USE_CRC_32_ENABLE  */
#define SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE       SL_IEC60730_IMC_INIT_VALUE
#define SL_IEC60730_IMC_CRC_BUFFER_XOR_OUTPUT       (0x0000)
#define SL_IEC60730_IMC_CRC_BUFFER_SAMPLE_RESULT    (0x31C3)
#define SL_IEC60730_IMC_CRC_BUFFER_INIT_DEFAULT \
  {                                             \
    0x1021UL,                                   \
    SL_IEC60730_IMC_CRC_BUFFER_INIT_VALUE,      \
    false,                                      \
    true,                                       \
    false,                                      \
    false,                                      \
    true,                                       \
  }
#define SL_IEC60730_IMC_CRC_BUFFER_UPDATE_DEFAULT \
  { { SL_IEC60730_DEFAULT_GPRC },                 \
    SL_IEC60730_IMC_CRC_BUFFER_INIT_DEFAULT,      \
    SL_IEC60730_IMC_DATA_READ_BIT_REVERSED,       \
    SL_IEC60730_IMC_CRC_BUFFER_XOR_OUTPUT }
#endif /* SL_IEC60730_USE_CRC_32_ENABLE  */
#endif /* SL_IEC60730_CRC_USE_SW_ENABLE */

#endif // DOXYGEN

/**************************************************************************/ /**
 * public IEC60730 Invariable Memory Check (IMC) Init
 *
 * @param params input parameter hardware configuration of CRC
 * @param test_config input parameter of region and number of regions user want to test
 *
 * @return None
 *
 * Performs a initialization of global variables and hardware configuration in
 * case hardware support.
 *****************************************************************************/
void sl_iec60730_imc_init(sl_iec60730_imc_params_t *params, sl_iec60730_imc_test_multiple_regions_t *test_config);

/**************************************************************************/ /**
 * public IEC60730 Update CRC git pull with Data Buffer
 *
 * @param params input parameter hardware configuration of CRC
 * @param crc The CRC to be updated. The initial value of crc is also initial
 * value of CRC calculation.
 * @param buffer A contiguous array of 1-byte values to be used to update the CRC.
 * @param size The number of bytes stored in the buffer.
 *
 * @returns #sl_iec60730_test_result_t.
 *          * If test fails, returns #SL_IEC60730_TEST_FAILED;
 *          * Otherwise, return #SL_IEC60730_TEST_PASSED.
 *
 * This routine takes a CRC and updates it on a pointer **crc** using a buffer of one or more bytes of data.
 * For details on the CRC polynomial used, see
 * @ref imc_hardware_architecture
 *****************************************************************************/
sl_iec60730_test_result_t
sl_iec60730_update_crc_with_data_buffer(sl_iec60730_update_crc_params_t *params,
                                        sl_iec60730_crc_t *crc,
                                        uint8_t *buffer,
                                        uint32_t size);

/** @} (end addtogroup IEC60730_INVARIABLE_MEMORY_TEST) */

/**************************************************************************/ /**
 * @addtogroup IEC60730_VARIABLE_MEMORY_TEST
 * @{
 *****************************************************************************/

/// Enter ATOMIC section of VMC Post
#define SL_IEC60730_VMC_POST_ENTER_CRITICAL() \
  CORE_DECLARE_IRQ_STATE;                     \
  CORE_ENTER_CRITICAL()

/// Exit ATOMIC section of VMC Post
#define SL_IEC60730_VMC_POST_EXIT_CRITICAL()  CORE_EXIT_CRITICAL()

/// Enter ATOMIC section of VMC Bist
#define SL_IEC60730_VMC_BIST_ENTER_CRITICAL() \
  CORE_DECLARE_IRQ_STATE;                     \
  CORE_ENTER_CRITICAL()

/// Exit ATOMIC section of VMC Bist
#define SL_IEC60730_VMC_BIST_EXIT_CRITICAL()  CORE_EXIT_CRITICAL()

/// This structure is used as configuration for VMC testing
typedef struct {
  uint32_t *start; ///< Start address of RAM to check
  uint32_t *end;   ///< End address of RAM to check
} sl_iec60730_vmc_test_region_t;

/// This structure is used as multiple test regions for VMC testing
typedef struct {
  const sl_iec60730_vmc_test_region_t *region;
  uint8_t number_of_test_regions; ///< Number of test regions
} sl_iec60730_vmc_test_multiple_regions_t;

/**************************************************************************/ /**
 * public IEC60730 Variable Memory Check (VMC) Initialize
 *
 * @param params input parameter of struct #sl_iec60730_vmc_test_multiple_regions_t form
 *
 * @return void
 *
 * Performs a initialization of global variables. This function SHOULD call
 * before calling #sl_iec60730_vmc_post()
 *****************************************************************************/
void sl_iec60730_vmc_init(sl_iec60730_vmc_test_multiple_regions_t *test_config);

/** @} (end addtogroup IEC60730_VARIABLE_MEMORY_TEST) */

#if (_SILICON_LABS_32B_SERIES == 2)
// Write to backup ram module
// buram_inst: Pointer to BURAM instance
// idx: BURAM Reg index
// val: write value
#define SL_IEC60730_BURAM_WRITE(buram_inst, idx, val) \
  do {                                                \
    buram_inst->RET[idx].REG = val;                   \
  } while (0)

// Read from backup ram module
// buram_inst: Pointer to BURAM instance
// idx: BURAM Reg index
#define SL_IEC60730_BURAM_READ(buram_inst, idx) (buram_inst->RET[idx].REG)
#endif

/** @} (end addtogroup efr32_iec60730) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SL_IEC60730_H */
