/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    dw1000.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __DW1000_H
#define __DW1000_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "decadriver\deca_port.h"
#include "decadriver\deca_device_api.h"
#include "decadriver\deca_regs.h"

/* Define ----------------------------------------------------------------------------------*/

/* The sum of the values is the TX to RX antenna delay, experimentally determined by a
 * calibration process. Here we use a hard coded typical value but, in a real application,
 * each device should have its own antenna delay properly calibrated to get the best possible
 * precision when performing ange measurements. */
#define UWB_TX_ANT_DELAY        16436
#define UWB_RX_ANT_DELAY        16436

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
 * 1 uus = 512 / 499.2 µs and 1 µs = 499.2 * 128 dtu. */
#define UWB_UUS_TO_DWT_TIME     65536

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        delay_nop( __IO uint32_t __n );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
