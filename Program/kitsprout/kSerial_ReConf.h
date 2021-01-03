/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kSerial_Conf.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KSERIAL_CONF_H
#define __KSERIAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Define ----------------------------------------------------------------------------------*/

#ifndef KSERIAL_SEND_ENABLE
#define KSERIAL_SEND_ENABLE                             (1U)
#ifndef KS_MAX_SEND_BUFFER_SIZE
#define KS_MAX_SEND_BUFFER_SIZE                         (4096 + 32)
#endif
#endif

#ifndef KSERIAL_RECV_ENABLE
#define KSERIAL_RECV_ENABLE                             (1U)
#ifndef KS_MAX_RECV_BUFFER_SIZE
#define KS_MAX_RECV_BUFFER_SIZE                         (4096 + 1024 + 32)
#endif
#endif

#ifndef KSERIAL_RECV_TREAD_ENABLE
#define KSERIAL_RECV_TREAD_ENABLE                       (0U)
#define KSERIAL_MAX_PACKET_LENS                         (4096)
#define KSERIAL_RECV_PACKET_BUFFER_LENS                 (64 * 1024)
#endif

#ifndef KSERIAL_CMD_ENABLE
#define KSERIAL_CMD_ENABLE                              (0U)
#endif

#if KSERIAL_RECV_TREAD_ENABLE
#if !(KSERIAL_RECV_ENABLE)
#error "Need to enable recv"
#endif
#endif
#if KSERIAL_CMD_ENABLE
#if !(KSERIAL_SEND_ENABLE && KSERIAL_RECV_ENABLE)
#error "Need to enable send and recv"
#endif
#endif

#define KSERIAL_TYPE_LENS                               (16)

/* Includes --------------------------------------------------------------------------------*/

#if (KSERIAL_SEND_ENABLE || KSERIAL_RECV_ENABLE)
#include "modules\serial.h"
#endif

/* Macro -----------------------------------------------------------------------------------*/

#if KSERIAL_SEND_ENABLE
#ifndef kSerial_Send
#define kSerial_Send(__DATA, __LENS)                    Serial_SendData(__DATA, __LENS)
#define kSerial_SendByte(__DATA)                        Serial_SendByte(__DATA)
#endif
#endif
#if KSERIAL_RECV_ENABLE
#define kSerial_Recv(__DATA, __LENS)                    Serial_RecvData(__DATA, __LENS)
#define kSerial_RecvByte()                              Serial_RecvByte)
#define kSerial_RecvFlush()                             {}
#endif
#if (KSERIAL_SEND_ENABLE || KSERIAL_RECV_ENABLE)
#define kSerial_Delay(__MS)                             delay_ms(__MS)
#endif

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
