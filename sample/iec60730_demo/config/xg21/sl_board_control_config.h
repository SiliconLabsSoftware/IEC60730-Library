 /***************************************************************************//**
 * @file
 * @brief Board Control.
 ******************************************************************************/

 #ifndef SL_BOARD_CONTROL_CONFIG_H
 #define SL_BOARD_CONTROL_CONFIG_H
 
 /* Dual-SDK compatibility */
 
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
 
 // <q SL_BOARD_ENABLE_VCOM> Enable Virtual COM UART
 // <i> Default: 0
 #define SL_BOARD_ENABLE_VCOM                    0
 
 // <q SL_BOARD_ENABLE_DISPLAY> Enable Display
 // <i> Default: 0
 #define SL_BOARD_ENABLE_DISPLAY                 0
 
 // <<< end of configuration section >>>
 
 // <<< sl:start pin_tool >>>
 
 // <gpio> SL_BOARD_ENABLE_VCOM
 #define SL_BOARD_ENABLE_VCOM_PORT              SL_GPIO_PORT_D
 #define SL_BOARD_ENABLE_VCOM_PIN               4
 
 // <gpio> SL_BOARD_ENABLE_DISPLAY
 #define SL_BOARD_ENABLE_DISPLAY_PORT           SL_GPIO_PORT_D
 #define SL_BOARD_ENABLE_DISPLAY_PIN            4
 
 // <<< sl:end pin_tool >>>
 
 #endif // SL_BOARD_CONTROL_CONFIG_H
 