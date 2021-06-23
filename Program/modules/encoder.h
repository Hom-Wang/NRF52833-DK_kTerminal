/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    encoder.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>
#include "drivers\nrf5x_gpiote.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/

extern GPIOTE_InitTypeDef hencoderA;
extern GPIOTE_InitTypeDef hencoderB;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        Encoder_Config( void );
void        Encoder_Cmd( uint32_t state );

uint32_t    Encoder_GetState( void );
int32_t     Encoder_GetRotary( int32_t *counter );

void        Encoder_Event( void );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
