/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    serial.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "modules\serial.h"

/* Define ----------------------------------------------------------------------------------*/

#define SERIAL_PRINTF_SUPPORT                           (1UL)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

UART_InitTypeDef hserial =
{
    .Instance            = SERIAL_UARTx,
    .PinTXD              = SERIAL_TXD_PIN,
    .PinRXD              = SERIAL_RXD_PIN,
    .PinRTS              = SERIAL_RTS_PIN,
    .PinCTS              = SERIAL_CTS_PIN,
    .Parity              = SERIAL_PARITY,
    .HardwareFlowControl = SERIAL_HARDWARECTRL,
    .BaudRate            = SERIAL_BAUDRATE,
    .TxEventCallback     = NULL,
    .RxEventCallback     = NULL
};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  Serial_RecvByte
 */
void Serial_Config( void )
{
    /* UART Init *****************************************************************/
    UART_Init(&hserial);

    /* UART IT *******************************************************************/
    if (hserial.RxEventCallback != NULL)
    {
        UART_InterruptCmd(&hserial, SERIAL_INTERRUPT_MODE, ENABLE);
        NVIC_SetPriority(SERIAL_UARTx_IRQn, SERIAL_UARTx_IRQn_PRIORITY);
        NVIC_EnableIRQ(SERIAL_UARTx_IRQn);
    }

    /* UART Enable ***************************************************************/
//    UART_Cmd(&hserial, ENABLE);
}

/**
 *  @brief  Serial_RecvByte
 */
void Serial_Cmd( uint32_t state )
{
    if (state != ENABLE)
    {
        UART_Cmd(&hserial, DISABLE);
    }
    else
    {
        UART_Cmd(&hserial, ENABLE);
    }
}

/**
 *  @brief  Serial_RecvByte
 */
void Serial_SetBaudrate( uint32_t baudrate )
{
    uint32_t state = KS_CLOSE;
    if (UART_Check(&hserial) == ENABLE)
    {
        state = KS_OPEN;
        Serial_Cmd(DISABLE);
    }
    hserial.BaudRate = baudrate;
    Serial_Config();
    if (state == KS_OPEN)
    {
        Serial_Cmd(ENABLE);
    }
}

/**
 *  @brief  Serial_SendByte
 */
__INLINE void Serial_SendByte( uint8_t sendByte )
{
    UART_SendByte(&hserial, &sendByte);
}

/**
 *  @brief  Serial_RecvByte
 */
__INLINE uint8_t Serial_RecvByte( void )
{
    uint8_t recvByte;
    UART_RecvByte(&hserial, &recvByte);
    return recvByte;
}

/**
 *  @brief  Serial Send Data
 */
__INLINE uint32_t Serial_SendData( uint8_t *sendData, uint32_t lens )
{
    return UART_SendData(&hserial, sendData, lens);
}

/**
 *  @brief  Serial Recv Data
 */
__INLINE uint32_t Serial_RecvData( uint8_t *recvData, uint32_t lens )
{
    return UART_RecvData(&hserial, recvData, lens);
}

/**
 *  @brief  Serial Send Data
 */
__INLINE uint32_t Serial_SendDataWaitTimeout( uint8_t *sendData, uint32_t lens, uint32_t timuout )
{
    return UART_SendDataWaitTimeout(&hserial, sendData, lens, timuout);
}

/**
 *  @brief  Serial Recv Data
 */
__INLINE uint32_t Serial_RecvDataWaitTimeout( uint8_t *recvData, uint32_t lens, uint32_t timeout )
{
    return UART_RecvDataWaitTimeout(&hserial, recvData, lens, timeout);
}

/**
 *  @brief  Serial Recv Wait
 */
__INLINE uint32_t Serial_RecvDataContinuous( uint8_t *recvData )
{
    return UART_RecvDataContinuous(&hserial, recvData);
}

#if SERIAL_PRINTF_SUPPORT
/**
 *  @brief  fputc
 */
int fputc( int ch, FILE *f )
{
    hserial.Instance->TXD = (uint8_t)ch;
    while (UART_EVENTS_TXDRDY(hserial.Instance) != SET) {;};  // Wait for TXD data to be sent
    UART_EVENTS_TXDRDY(hserial.Instance) = RESET;
    return (ch);
}

/**
 *  @brief  fgetc
 */
int fgetc( FILE *f )
{
    while (UART_EVENTS_RXDRDY(hserial.Instance) != SET) {;};    // Wait for RXD data to be received
    UART_EVENTS_RXDRDY(hserial.Instance) = RESET;
    return ((uint8_t)hserial.Instance->RXD);
}
#endif

/*************************************** END OF FILE ****************************************/
