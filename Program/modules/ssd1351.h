/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    ssd1351.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __SSD1351_H
#define __SSD1351_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "nrf.h"

/* Define ----------------------------------------------------------------------------------*/

#define SSD1351_WIDTH   128
#define SSD1351_HIGHT   128

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        SSD1351_WriteCmd( uint8_t command );
void        SSD1351_WriteData( uint8_t data );
void        SSD1351_WriteColor( uint16_t color );

void        SSD1351_Config( void );
void        SSD1351_Init( void );

void        SSD1351_Display( uint8_t cmd );
void        SSD1351_Clear( uint16_t color );
void        SSD1351_SetWindow( int16_t posX1, int16_t posY1, int16_t posX2, int16_t posY2 );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
