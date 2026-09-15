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

/* USART settings giữ nguyên */

#define SL_IOSTREAM_USART_VCOM_BAUDRATE              115200
#define SL_IOSTREAM_USART_VCOM_PARITY                usartNoParity
#define SL_IOSTREAM_USART_VCOM_STOP_BITS             usartStopbits1
#define SL_IOSTREAM_USART_VCOM_FLOW_CONTROL_TYPE     usartHwFlowControlCtsAndRts

#define SL_IOSTREAM_USART_VCOM_RX_BUFFER_SIZE        32
#define SL_IOSTREAM_USART_VCOM_CONVERT_BY_DEFAULT_LF_TO_CRLF 0
#define SL_IOSTREAM_USART_VCOM_RESTRICT_ENERGY_MODE_TO_ALLOW_RECEPTION 1

/* Pin Tool section */

#define SL_IOSTREAM_USART_VCOM_PERIPHERAL        USART0
#define SL_IOSTREAM_USART_VCOM_PERIPHERAL_NO     0

/* USART0 TX on PA05 */
#define SL_IOSTREAM_USART_VCOM_TX_PORT           SL_GPIO_PORT_A
#define SL_IOSTREAM_USART_VCOM_TX_PIN            5

/* USART0 RX on PA06 */
#define SL_IOSTREAM_USART_VCOM_RX_PORT           SL_GPIO_PORT_A
#define SL_IOSTREAM_USART_VCOM_RX_PIN            6

/* USART0 CTS on PA04 */
#define SL_IOSTREAM_USART_VCOM_CTS_PORT          SL_GPIO_PORT_A
#define SL_IOSTREAM_USART_VCOM_CTS_PIN           4

/* USART0 RTS on PC01 */
#define SL_IOSTREAM_USART_VCOM_RTS_PORT          SL_GPIO_PORT_C
#define SL_IOSTREAM_USART_VCOM_RTS_PIN           1

#endif /* SL_IOSTREAM_USART_VCOM_CONFIG_H */
