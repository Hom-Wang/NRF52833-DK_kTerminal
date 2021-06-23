/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    ist8308.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __IST8308_H
#define __IST8308_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>

/* Define ----------------------------------------------------------------------------------*/

#define IST8308_I2C_ADDR                                ((uint8_t)0x0C)     // CADx = [0, 0]
#define IST8308_DEVICD_ID                               ((uint8_t)0x08)

#define IST8308_WAI                                     ((uint8_t)0x00)     // Who am i
#define IST8308_STR                                     ((uint8_t)0x0C)     // Self-Test Register
#define IST8308_STAT                                    ((uint8_t)0x10)     // Data status information
#define IST8308_DATAXL                                  ((uint8_t)0x11)     // Low  byte for X-axis data
#define IST8308_DATAXH                                  ((uint8_t)0x12)     // High byte for X-axis data
#define IST8308_DATAYL                                  ((uint8_t)0x13)     // Low  byte for Y-axis data
#define IST8308_DATAYH                                  ((uint8_t)0x14)     // High byte for Y-axis data
#define IST8308_DATAZL                                  ((uint8_t)0x15)     // Low  byte for Z-axis data
#define IST8308_DATAZH                                  ((uint8_t)0x16)     // High byte for Z-axis data
#define IST8308_ACTR                                    ((uint8_t)0x20)     // Suspend Mode control
#define IST8308_CNTL1                                   ((uint8_t)0x30)     // Chip control settings 1
#define IST8308_CNTL2                                   ((uint8_t)0x31)     // Chip control settings 2
#define IST8308_CNTL3                                   ((uint8_t)0x32)     // Chip control settings 3
#define IST8308_CNTL4                                   ((uint8_t)0x34)     // Chip control settings 4
#define IST8308_OSRCNTL                                 ((uint8_t)0x41)     // Oversampling ratio setting

#define IST8308_SENS_500UT_LSB                          (6.6)               // ±500 uT, LSB/uT
#define IST8308_SENS_200UT_LSB                          (13.2)              // ±200 uT, LSB/uT

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        IST8308_WriteReg( uint8_t writeAddr, uint8_t writeData );
void        IST8308_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens );
uint8_t     IST8308_ReadReg( uint8_t readAddr );
void        IST8308_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens );

void        IST8308_Config( void );
uint32_t    IST8308_Init( void );

uint32_t    IST8308_DeviceCheck( uint8_t deviceid );
double      IST8308_GetMagSensitivity( void );
uint32_t    IST8308_GetLsbData( int16_t *lsb );
uint32_t    IST8308_GetRawData( float *raw );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
