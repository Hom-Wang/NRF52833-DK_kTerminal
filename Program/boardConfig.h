/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    boardConfig.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __BOARDCONFIG_H
#define __BOARDCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
/* Define ----------------------------------------------------------------------------------*/

#define KS_HW_BOARD_NAME                                "PCA10100"
#define KS_HW_MCU_NAME                                  "NRF52833"

#define KS_HW_USE_CLOCK_SOUCE_INT                       (0U)
#define KS_HW_USE_CLOCK_SOUCE_EXT                       (1U)
#define KS_HW_HCLOCK_SOUCE                              KS_HW_USE_CLOCK_SOUCE_EXT
#define KS_HW_LCLOCK_SOUCE                              KS_HW_USE_CLOCK_SOUCE_EXT

/* -------- LED and KEY */

#define LED_1_PIN                                       13
#define LED_1_PORT                                      0
#define LED_1_Set()                                     __GPIO_SET(LED_1_PORT, LED_1_PIN)
#define LED_1_Reset()                                   __GPIO_RST(LED_1_PORT, LED_1_PIN)
#define LED_1_Toggle()                                  __GPIO_TOG(LED_1_PORT, LED_1_PIN)
#define LED_1_On()                                      LED_1_Reset()
#define LED_1_Off()                                     LED_1_Set()

#define LED_2_PIN                                       14
#define LED_2_PORT                                      0
#define LED_2_Set()                                     __GPIO_SET(LED_2_PORT, LED_2_PIN)
#define LED_2_Reset()                                   __GPIO_RST(LED_2_PORT, LED_2_PIN)
#define LED_2_Toggle()                                  __GPIO_TOG(LED_2_PORT, LED_2_PIN)
#define LED_2_On()                                      LED_2_Reset()
#define LED_2_Off()                                     LED_2_Set()

#define LED_3_PIN                                       15
#define LED_3_PORT                                      0
#define LED_3_Set()                                     __GPIO_SET(LED_3_PORT, LED_3_PIN)
#define LED_3_Reset()                                   __GPIO_RST(LED_3_PORT, LED_3_PIN)
#define LED_3_Toggle()                                  __GPIO_TOG(LED_3_PORT, LED_3_PIN)
#define LED_3_On()                                      LED_3_Reset()
#define LED_3_Off()                                     LED_3_Set()

#define LED_4_PIN                                       16
#define LED_4_PORT                                      0
#define LED_4_Set()                                     __GPIO_SET(LED_4_PORT, LED_4_PIN)
#define LED_4_Reset()                                   __GPIO_RST(LED_4_PORT, LED_4_PIN)
#define LED_4_Toggle()                                  __GPIO_TOG(LED_4_PORT, LED_4_PIN)
#define LED_4_On()                                      LED_4_Reset()
#define LED_4_Off()                                     LED_4_Set()

#define KEY_1_PIN                                       11
#define KEY_1_PORT                                      0
#define KEY_1_Read()                                    (__GPIO_READ(KEY_1_PORT, KEY_1_PIN) == RESET)

#define KEY_2_PIN                                       12
#define KEY_2_PORT                                      0
#define KEY_2_Read()                                    (__GPIO_READ(KEY_2_PORT, KEY_2_PIN) == RESET)

#define KEY_3_PIN                                       24
#define KEY_3_PORT                                      0
#define KEY_3_Read()                                    (__GPIO_READ(KEY_3_PORT, KEY_3_PIN) == RESET)

#define KEY_4_PIN                                       25
#define KEY_4_PORT                                      0
#define KEY_4_Read()                                    (__GPIO_READ(KEY_4_PORT, KEY_4_PIN) == RESET)


/* -------- Timer */

#define TIMERx                                          NRF_TIMER0
#define TIMERx_IRQn                                     TIMER0_IRQn
#define TIMERx_IRQHANDLER                               TIMER0_IRQHandler
#define TIMERx_IRQn_PRIORITY                            1
#define TIMERx_MODE                                     NRF_TIMER_MODE_TIMER
#define TIMERx_PRESCALER                                NRF_TIMER_FREQ_1MHz
#define TIMERx_BIT_MODE                                 NRF_TIMER_BIT_WIDTH_32

#define TIMERx_CHANNEL                                  NRF_TIMER_CC_CHANNEL0
#define TIMERx_CHANNEL_PERIOD                           1000


/* -------- Serial */

#define SERIAL_UARTx                                    NRF_UART0
#define SERIAL_UARTx_IRQn                               UARTE0_UART0_IRQn
#define SERIAL_UARTx_IRQHANDLER                         UARTE0_UART0_IRQHandler
#define SERIAL_UARTx_IRQn_PRIORITY                      1
#define SERIAL_TXD_PIN                                  6
#define SERIAL_RXD_PIN                                  8
#define SERIAL_RTS_PIN                                  5
#define SERIAL_CTS_PIN                                  7

#define SERIAL_BAUDRATE                                 NRF_UART_BAUDRATE_115200
//#define SERIAL_BAUDRATE                                 NRF_UART_BAUDRATE_230400
//#define SERIAL_BAUDRATE                                 NRF_UART_BAUDRATE_250000
//#define SERIAL_BAUDRATE                                 NRF_UART_BAUDRATE_460800
//#define SERIAL_BAUDRATE                                 NRF_UART_BAUDRATE_921600
//#define SERIAL_BAUDRATE                                 NRF_UART_BAUDRATE_1000000
#define SERIAL_PARITY                                   NRF_UART_PARITY_EXCLUDED
#define SERIAL_HARDWARECTRL                             NRF_UART_HWFC_DISABLED
#define SERIAL_INTERRUPT_MODE                           NRF_UART_INT_MASK_RXDRDY


/* -------- Rotary Encoder */

#define RTEN_GPIOTEx                                    NRF_GPIOTE
#define RTEN_GPIOTEx_IRQn                               GPIOTE_IRQn
#define RTEN_GPIOTEx_IRQHANDLER                         GPIOTE_IRQHandler
#define RTEN_GPIOTEx_IRQn_PRIORITY                      1
#define RTEN_GPIOTEx_POLARITY                           NRF_GPIOTE_POLARITY_TOGGLE
#define RTEN_GPIOTEx_MODE                               GPIOTE_CONFIG_MODE_Event
#define RTEN_GPIOTEx_OUTINIT                            NRF_GPIOTE_INITIAL_VALUE_HIGH

#define RTEN_KEY_PIN                                    3
#define RTEN_KEY_PORT                                   0
#define RTEN_KEY_Read()                                 (__GPIO_READ(RTEN_KEY_PORT, RTEN_KEY_PIN) == RESET)

#define RTEN_A_PIN                                      28
#define RTEN_A_PORT                                     0
#define RTEN_A_Read()                                   (__GPIO_READ(RTEN_A_PORT, RTEN_A_PIN) == SET)
#define RTEN_A_GPIOTEx_LINE                             0

#define RTEN_B_PIN                                      4
#define RTEN_B_PORT                                     0
#define RTEN_B_Read()                                   (__GPIO_READ(RTEN_B_PORT, RTEN_B_PIN) == SET)
#define RTEN_B_GPIOTEx_LINE                             1

/* -------- Simulation i2c */

#define TWI_SCL_PIN                                     IMU_SCL_PIN
#define TWI_SCL_PORT                                    IMU_SCL_PORT

#define TWI_SDA_PIN                                     IMU_SDA_PIN
#define TWI_SDA_PORT                                    IMU_SDA_PORT


/* -------- Inertial Sensor */

#define IMU_CSM_PIN                                     4
#define IMU_CSM_PORT                                    1
#define IMU_CSM_H()                                     __GPIO_SET(IMU_CSM_PORT, IMU_CSM_PIN)
#define IMU_CSM_L()                                     __GPIO_RST(IMU_CSM_PORT, IMU_CSM_PIN)

#define IMU_CSB_PIN                                     8
#define IMU_CSB_PORT                                    1
#define IMU_CSB_H()                                     __GPIO_SET(IMU_CSB_PORT, IMU_CSB_PIN)
#define IMU_CSB_L()                                     __GPIO_RST(IMU_CSB_PORT, IMU_CSB_PIN)

#define IMU_SCK_PIN                                     5
#define IMU_SCK_PORT                                    1

#define IMU_SDI_PIN                                     6
#define IMU_SDI_PORT                                    1

#define IMU_SDO_PIN                                     7
#define IMU_SDO_PORT                                    1
#define IMU_SDO_H()                                     __GPIO_SET(IMU_SDO_PORT, IMU_SDO_PIN)
#define IMU_SDO_L()                                     __GPIO_RST(IMU_SDO_PORT, IMU_SDO_PIN)

#define IMU_INT_PIN                                     3
#define IMU_INT_PORT                                    1

#define IMU_SDA_PIN                                     1
#define IMU_SDA_PORT                                    1

#define IMU_SCL_PIN                                     2
#define IMU_SCL_PORT                                    1


/* -------- OLED */

#define OLED_CS_PIN                                     8
#define OLED_CS_PORT                                    0
#define OLED_CS_H()                                     __GPIO_SET(OLED_CS_PORT, OLED_CS_PIN)
#define OLED_CS_L()                                     __GPIO_RST(OLED_CS_PORT, OLED_CS_PIN)

#define OLED_SCK_PIN                                    7
#define OLED_SCK_PORT                                   0

#define OLED_SDI_PIN                                    6
#define OLED_SDI_PORT                                   0

#define OLED_DC_PIN                                     9
#define OLED_DC_PORT                                    0
#define OLED_DC_H()                                     __GPIO_SET(OLED_DC_PORT, OLED_DC_PIN)
#define OLED_DC_L()                                     __GPIO_RST(OLED_DC_PORT, OLED_DC_PIN)

#define OLED_RST_PIN                                    10
#define OLED_RST_PORT                                   0
#define OLED_RST_H()                                    __GPIO_SET(OLED_RST_PORT, OLED_RST_PIN)
#define OLED_RST_L()                                    __GPIO_RST(OLED_RST_PORT, OLED_RST_PIN)


/* -------- Ultra-Wideband */

#define UWB_CSN_PIN                                     17
#define UWB_CSN_PORT                                    0

#define UWB_SCK_PIN                                     21
#define UWB_SCK_PORT                                    0

#define UWB_SDI_PIN                                     19
#define UWB_SDI_PORT                                    0

#define UWB_SDO_PIN                                     20
#define UWB_SDO_PORT                                    0

#define UWB_IRQ_PIN                                     22
#define UWB_IRQ_PORT                                    0

#define UWB_RST_PIN                                     23
#define UWB_RST_PORT                                    0

#define UWB_WAK_PIN                                     2
#define UWB_WAK_PORT                                    0

#define UWB_EXT_PIN                                     26
#define UWB_EXT_PORT                                    0


/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
