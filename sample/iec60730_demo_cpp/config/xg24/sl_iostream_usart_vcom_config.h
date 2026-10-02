/***************************************************************************//**
 * @file
 * @brief IOSTREAM_USART Config.
 * @note Device-only override based on BRD4187C A01 (BRD4002A).
 *       RX/CTS remapped for QFN40 (e.g. EFR32BG24A010F1024IM40): PA09/PB05
 *       are not bonded, so Pin Tool would otherwise drop those defines.
 *       Selecting board BRD4187C uses the BSP pin map instead.
 ******************************************************************************/

 #ifndef SL_IOSTREAM_USART_VCOM_CONFIG_H
 #define SL_IOSTREAM_USART_VCOM_CONFIG_H
 
 /* Dual-SDK compatibility layer */
 
 #ifndef SL_GPIO_PORT_A
 #define SL_GPIO_PORT_A gpioPortA
 #endif
 
 #ifndef SL_GPIO_PORT_B
 #define SL_GPIO_PORT_B gpioPortB
 #endif
 
 #ifndef SL_GPIO_PORT_C
 #define SL_GPIO_PORT_C gpioPortC
 #endif
 
 #ifndef SL_GPIO_PORT_D
 #define SL_GPIO_PORT_D gpioPortD
 #endif
 
 #ifndef SL_GPIO_PORT_E
 #define SL_GPIO_PORT_E gpioPortE
 #endif
 
 #ifndef SL_GPIO_PORT_F
 #define SL_GPIO_PORT_F gpioPortF
 #endif
 
 // <<< Use Configuration Wizard in Context Menu >>>
 
 // <h> USART settings
 
 // <o SL_IOSTREAM_USART_VCOM_BAUDRATE> Baud rate
 // <i> Default: 115200
 #define SL_IOSTREAM_USART_VCOM_BAUDRATE              115200
 
 // <o SL_IOSTREAM_USART_VCOM_PARITY> Parity mode to use
 // <usartNoParity=> No Parity
 // <usartEvenParity=> Even parity
 // <usartOddParity=> Odd parity
 // <i> Default: usartNoParity
 #define SL_IOSTREAM_USART_VCOM_PARITY                usartNoParity
 
 // <o SL_IOSTREAM_USART_VCOM_STOP_BITS> Number of stop bits to use.
 // <usartStopbits0p5=> 0.5 stop bits
 // <usartStopbits1=> 1 stop bits
 // <usartStopbits1p5=> 1.5 stop bits
 // <usartStopbits2=> 2 stop bits
 // <i> Default: usartStopbits1
 #define SL_IOSTREAM_USART_VCOM_STOP_BITS             usartStopbits1
 
 // <o SL_IOSTREAM_USART_VCOM_FLOW_CONTROL_TYPE> Flow control
 // <usartHwFlowControlNone=> None
 // <usartHwFlowControlCts=> CTS
 // <usartHwFlowControlRts=> RTS
 // <usartHwFlowControlCtsAndRts=> CTS/RTS
 // <uartFlowControlSoftware=> Software Flow control (XON/XOFF)
 // <i> Default: usartHwFlowControlNone
 #define SL_IOSTREAM_USART_VCOM_FLOW_CONTROL_TYPE     usartHwFlowControlCtsAndRts
 
 // <o SL_IOSTREAM_USART_VCOM_RX_BUFFER_SIZE> Receive buffer size
 // <i> Default: 32
 #define SL_IOSTREAM_USART_VCOM_RX_BUFFER_SIZE        32
 
 // <q SL_IOSTREAM_USART_VCOM_CONVERT_BY_DEFAULT_LF_TO_CRLF> Convert \n to \r\n
 // <i> It can be changed at runtime using the C API.
 // <i> Default: 0
 #define SL_IOSTREAM_USART_VCOM_CONVERT_BY_DEFAULT_LF_TO_CRLF 0
 
 // <q SL_IOSTREAM_USART_VCOM_RESTRICT_ENERGY_MODE_TO_ALLOW_RECEPTION>
 // <i> Default: 1
 // <i> Limits the lowest energy mode the system can sleep to in order to
 // <i> keep the reception on. May cause higher power consumption.
 #define SL_IOSTREAM_USART_VCOM_RESTRICT_ENERGY_MODE_TO_ALLOW_RECEPTION 1
 
 // </h>
 
 // <<< end of configuration section >>>
 
 // <<< sl:start pin_tool >>>
 
 // <usart signal=TX,RX,(CTS),(RTS)> SL_IOSTREAM_USART_VCOM
 
 #define SL_IOSTREAM_USART_VCOM_PERIPHERAL        USART0
 #define SL_IOSTREAM_USART_VCOM_PERIPHERAL_NO     0
 
 // USART0 TX on PA08 (BRD4187C)
 #define SL_IOSTREAM_USART_VCOM_TX_PORT           SL_GPIO_PORT_A
 #define SL_IOSTREAM_USART_VCOM_TX_PIN            8
 
 // USART0 RX on PA07 (BRD4187C: PA09 not bonded on QFN40)
 #define SL_IOSTREAM_USART_VCOM_RX_PORT           SL_GPIO_PORT_A
 #define SL_IOSTREAM_USART_VCOM_RX_PIN            7
 
 // USART0 CTS on PB04 (BRD4187C: PB05 not bonded on QFN40)
 #define SL_IOSTREAM_USART_VCOM_CTS_PORT          SL_GPIO_PORT_B
 #define SL_IOSTREAM_USART_VCOM_CTS_PIN           4
 
 // USART0 RTS on PA00 (BRD4187C)
 #define SL_IOSTREAM_USART_VCOM_RTS_PORT          SL_GPIO_PORT_A
 #define SL_IOSTREAM_USART_VCOM_RTS_PIN           0
 
 // <<< sl:end pin_tool >>>
 
 #endif /* SL_IOSTREAM_USART_VCOM_CONFIG_H */