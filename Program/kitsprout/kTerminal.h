/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kTerminal.c
 *  @author  KitSprout
 *  @brief   
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KTERMINAL_H
#define __KTERMINAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>
#include "kserial\kSerial.h"

/* Define ----------------------------------------------------------------------------------*/

//#define KLOGGER_PRINTF_SUPPORT                          (0U)
//#define KLOGGER_BUFFER_INDEX                            (0U)
//#define KLOGGER_JSCOPE_BUFFER_INDEX                     (1U)

/* Macro -----------------------------------------------------------------------------------*/

//#define klog_getkey()                                   SEGGER_RTT_GetKey()
//#define klog_waitkey()                                  SEGGER_RTT_WaitKey()
//#define klog_haskey()                                   SEGGER_RTT_HasKey()
//#define klog_read(__DATA, __LENS)                       SEGGER_RTT_Read(KLOGGER_BUFFER_INDEX, __DATA, __LENS)
//#define klog_write(__DATA, __LENS)                      SEGGER_RTT_Write(KLOGGER_BUFFER_INDEX, __DATA, __LENS)
//#define klog_setTerminal(__IDX)                         SEGGER_RTT_SetTerminal(__IDX)

/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t    kTerminal( uint32_t byte );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
