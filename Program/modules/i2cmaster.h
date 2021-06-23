/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    i2cmaster.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __I2CMASTER_H
#define __I2CMASTER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/

#define i2c_delay(__MS)                                 delay_ms(__MS)
#define i2c_delay_us(__US)                              delay_us(__US)

/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        i2c_config( const uint8_t sclport, const uint8_t sclpin, const uint8_t sdaport, const uint8_t sdapin, uint32_t freq );

void        i2c_delay( uint32_t milliseconds );
void        i2c_delay_us( uint32_t milliseconds );

void        i2c_write_byte( uint8_t address, uint8_t reg, uint8_t data );
void        i2c_write( uint8_t address, uint8_t reg, uint8_t length, uint8_t *data );
uint8_t     i2c_read_byte( uint8_t address, uint8_t reg );
void        i2c_read( uint8_t address, uint8_t reg, uint8_t length, uint8_t *data );

uint32_t    i2c_scan_device( uint8_t *address );
uint32_t    i2c_scan_regs( const uint8_t address, uint8_t *reg );
void        i2c_scan_all( const uint8_t sclpin, const uint8_t sdapin );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
