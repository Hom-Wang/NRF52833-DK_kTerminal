/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_uart.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_uart.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

const uint32_t BAUDRATE_REGVAL[18][2] = 
{
       1200,    NRF_UART_BAUDRATE_1200,     //   1200 baud (actual rate: 1205)      ... 0.995
       2400,    NRF_UART_BAUDRATE_2400,     //   2400 baud (actual rate: 2396)      ... 1.001
       4800,    NRF_UART_BAUDRATE_4800,     //   4800 baud (actual rate: 4808)      ... 0.998
       9600,    NRF_UART_BAUDRATE_9600,     //   9600 baud (actual rate: 9598)      ... 1.000
      14400,    NRF_UART_BAUDRATE_14400,    //  14400 baud (actual rate: 14414)     ... 0.999
      19200,    NRF_UART_BAUDRATE_19200,    //  19200 baud (actual rate: 19208)     ... 0.999
      28800,    NRF_UART_BAUDRATE_28800,    //  28800 baud (actual rate: 28829)     ... 0.998
      31250,    NRF_UART_BAUDRATE_31250,    //  31250 baud                          ... 1.000
      38400,    NRF_UART_BAUDRATE_38400,    //  38400 baud (actual rate: 38462)     ... 0.998
      56000,    NRF_UART_BAUDRATE_56000,    //  56000 baud (actual rate: 55944)     ... 1.001
      57600,    NRF_UART_BAUDRATE_57600,    //  57600 baud (actual rate: 57762)     ... 0.997
      76800,    NRF_UART_BAUDRATE_76800,    //  76800 baud (actual rate: 76923)     ... 0.998
     115200,    NRF_UART_BAUDRATE_115200,   // 115200 baud (actual rate: 115942)    ... 0.993
     230400,    NRF_UART_BAUDRATE_230400,   // 230400 baud (actual rate: 231884)    ... 0.993
     250000,    NRF_UART_BAUDRATE_250000,   // 250000 baud                          ... 1.000
     460800,    NRF_UART_BAUDRATE_460800,   // 460800 baud (actual rate: 470588)    ... 0.979
     921600,    NRF_UART_BAUDRATE_921600,   // 921600 baud (actual rate: 941176)    ... 0.979
    1000000,    NRF_UART_BAUDRATE_1000000   // 1 Mega baud                          ... 1.000
};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  UART_Init
 */
void UART_Init( UART_InitTypeDef *huart )
{
    nrf_gpio_cfg_output(huart->PinTXD);
    nrf_gpio_cfg_input(huart->PinRXD, NRF_GPIO_PIN_NOPULL);

    huart->Instance->PSELTXD = huart->PinTXD;
    huart->Instance->PSELRXD = huart->PinRXD;

    if (huart->HardwareFlowControl == NRF_UART_HWFC_ENABLED)
    {
        nrf_gpio_cfg_output(huart->PinRTS);
        nrf_gpio_cfg_input(huart->PinCTS, NRF_GPIO_PIN_NOPULL);
        huart->Instance->PSELCTS = huart->PinCTS;
        huart->Instance->PSELRTS = huart->PinRTS;
    }

    huart->Instance->CONFIG = huart->Parity | huart->HardwareFlowControl;
    huart->Instance->BAUDRATE = huart->BaudRate;
    UART_Start(huart);
}

/**
 *  @brief  UART_Cmd
 */
void UART_Cmd( UART_InitTypeDef *huart, uint32_t state )
{
    if (state != ENABLE)
    {
        huart->Instance->ENABLE = UART_ENABLE_ENABLE_Disabled;
    }
    else
    {
        huart->Instance->ENABLE = UART_ENABLE_ENABLE_Enabled;
    }
}

/**
 *  @brief  UART_InterruptCmd
 */
void UART_InterruptCmd( UART_InitTypeDef *huart, uint32_t mode, uint32_t state )
{
    if (state != ENABLE)
    {
        huart->Instance->INTENCLR = mode;
    }
    else
    {
        huart->Instance->INTENSET = mode;
    }
}

/**
 *  @brief  UART_Check
 */
uint32_t UART_Check( UART_InitTypeDef *huart )
{
    // check enable or disable
    return huart->Instance->ENABLE;
    
}

/**
 *  @brief  UART_Start
 */
void UART_Start( UART_InitTypeDef *huart )
{
    UART_EVENTS_TXDRDY(huart->Instance) = RESET;
    UART_EVENTS_RXDRDY(huart->Instance) = RESET;
    UART_TASKS_STARTTX(huart->Instance) = SET;
    UART_TASKS_STARTRX(huart->Instance) = SET;
}

/**
 *  @brief  UART_Stop
 */
void UART_Stop( UART_InitTypeDef *huart )
{
    UART_TASKS_SUSPEND(huart->Instance) = SET;
}

/**
 *  @brief  UART_SendByte
 */
void UART_SendByte( UART_InitTypeDef *huart, uint8_t *sendByte )
{
    huart->Instance->TXD = *sendByte;
    while (UART_EVENTS_TXDRDY(huart->Instance) != SET);   // Wait for TXD data to be sent
    UART_EVENTS_TXDRDY(huart->Instance) = RESET;
}

/**
 *  @brief  UART_RecvByte
 */
void UART_RecvByte( UART_InitTypeDef *huart, uint8_t *recvByte )
{
    while (UART_EVENTS_RXDRDY(huart->Instance) != SET);   // Wait for RXD data to be received
    UART_EVENTS_RXDRDY(huart->Instance) = RESET;
    *recvByte = (uint8_t)huart->Instance->RXD;
}

/**
 *  @brief  UART_SendData
 */
uint32_t UART_SendData( UART_InitTypeDef *huart, uint8_t *sendData, uint32_t lens )
{
    if ((sendData == NULL) || (lens == 0))
    {
        return KS_ERROR;
    }

    while (lens--)
    {
        huart->Instance->TXD = *sendData++;
        while (UART_EVENTS_TXDRDY(huart->Instance) != SET);
        UART_EVENTS_TXDRDY(huart->Instance) = RESET;
    }

    return KS_OK;
}

/**
 *  @brief  UART_RecvData
 */
uint32_t UART_RecvData( UART_InitTypeDef *huart, uint8_t *recvData, uint32_t lens )
{
    if ((recvData == NULL) || (lens == 0))
    {
        return KS_ERROR;
    }

    while (lens--)
    {
        while (UART_EVENTS_RXDRDY(huart->Instance) != SET);
        UART_EVENTS_RXDRDY(huart->Instance) = RESET;
        *recvData++ = (uint8_t)huart->Instance->RXD;
    }

    return KS_OK;
}

/**
 *  @brief  UART_SendDataWaitTimeout
 */
uint32_t UART_SendDataWaitTimeout( UART_InitTypeDef *huart, uint8_t *sendData, uint32_t lens, uint32_t timeout )
{
    if ((sendData == NULL) || (lens == 0))
    {
        return KS_ERROR;
    }

    while (lens--)
    {
        huart->Instance->TXD = *sendData++;
        while (UART_EVENTS_TXDRDY(huart->Instance) != SET)
        {
            if (--timeout)
            {
                delay_us(1);
            }
            else
            {
                UART_EVENTS_TXDRDY(huart->Instance) = RESET;
                return KS_TIMEOUT;
            }
        }
        UART_EVENTS_TXDRDY(huart->Instance) = RESET;
    }

    return KS_OK;
}

/**
 *  @brief  UART_RecvDataWaitTimeout
 */
uint32_t UART_RecvDataWaitTimeout( UART_InitTypeDef *huart, uint8_t *recvData, uint32_t lens, uint32_t timeout )
{
    if ((recvData == NULL) || (lens == 0))
    {
        return KS_ERROR;
    }

    while (lens--)
    {
        while (UART_EVENTS_RXDRDY(huart->Instance) != SET)
        {
            if (--timeout)
            {
                delay_us(1);
            }
            else
            {
                UART_EVENTS_RXDRDY(huart->Instance) = RESET;
                return KS_TIMEOUT;
            }
        }
        UART_EVENTS_RXDRDY(huart->Instance) = RESET;
        *recvData++ = (uint8_t)huart->Instance->RXD;
    }

    return KS_OK;
}

/**
 *  @brief  UART_RecvDataContinuous
 */
uint32_t UART_RecvDataContinuous( UART_InitTypeDef *huart, uint8_t *recvData )
{
    uint32_t lens = 0;
    uint32_t wait = 0x7FFF; // about 32767 clocks

    while (wait--)
    {
        while (UART_EVENTS_RXDRDY(huart->Instance) != SET)
        {
            UART_EVENTS_RXDRDY(huart->Instance) = RESET;
            *recvData++ = (uint8_t)huart->Instance->RXD;
            lens++;
            wait = 0x7FFF;
        }
    }

    return lens;
}

/**
 *  @brief  UART_RegValue2Baudrate
 */
uint32_t UART_RegValue2Baudrate( uint32_t regvalue )
{
    for (uint32_t i = 0; i < 18; i++)
    {
        if (regvalue == BAUDRATE_REGVAL[i][1])
        {
            return BAUDRATE_REGVAL[i][0];
        }
    }
    return 0;
}

/**
 *  @brief  UART_Baudrate2RegValue
 */
uint32_t UART_Baudrate2RegValue( uint32_t baudrate )
{
    for (uint32_t i = 0; i < 18; i++)
    {
        if (baudrate == BAUDRATE_REGVAL[i][0])
        {
            return BAUDRATE_REGVAL[i][1];
        }
    }
    return 0;
}

/*************************************** END OF FILE ****************************************/
