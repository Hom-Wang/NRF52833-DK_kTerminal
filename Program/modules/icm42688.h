/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    icm42688.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __ICM42688_H
#define __ICM42688_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>

/* Define ----------------------------------------------------------------------------------*/

#define ICM42688_I2C_ADDR                               ((uint8_t)0x68)     // CADx = [0, 0]
#define ICM42688_DEVICD_ID                              ((uint8_t)0x47)

// BANK 0
#define ICM42688_DEVICE_CONFIG                          ((uint8_t)0x11)
#define ICM42688_DRIVE_CONFIG                           ((uint8_t)0x13)
#define ICM42688_INT_CONFIG                             ((uint8_t)0x14)
#define ICM42688_FIFO_CONFIG                            ((uint8_t)0x16)
#define ICM42688_TEMP_DATA1                             ((uint8_t)0x1D)
#define ICM42688_TEMP_DATA0                             ((uint8_t)0x1E)
#define ICM42688_ACCEL_DATA_X1                          ((uint8_t)0x1F)
#define ICM42688_ACCEL_DATA_X0                          ((uint8_t)0x20)
#define ICM42688_ACCEL_DATA_Y1                          ((uint8_t)0x21)
#define ICM42688_ACCEL_DATA_Y0                          ((uint8_t)0x22)
#define ICM42688_ACCEL_DATA_Z1                          ((uint8_t)0x23)
#define ICM42688_ACCEL_DATA_Z0                          ((uint8_t)0x24)
#define ICM42688_GYRO_DATA_X1                           ((uint8_t)0x25)
#define ICM42688_GYRO_DATA_X0                           ((uint8_t)0x26)
#define ICM42688_GYRO_DATA_Y1                           ((uint8_t)0x27)
#define ICM42688_GYRO_DATA_Y0                           ((uint8_t)0x28)
#define ICM42688_GYRO_DATA_Z1                           ((uint8_t)0x29)
#define ICM42688_GYRO_DATA_Z0                           ((uint8_t)0x2A)
#define ICM42688_TMST_FSYNCH                            ((uint8_t)0x2B)
#define ICM42688_TMST_FSYNCL                            ((uint8_t)0x2C)
#define ICM42688_INT_STATUS                             ((uint8_t)0x2D)
#define ICM42688_FIFO_COUNTH                            ((uint8_t)0x2E)
#define ICM42688_FIFO_COUNTL                            ((uint8_t)0x2F)
#define ICM42688_FIFO_DATA                              ((uint8_t)0x30)
#define ICM42688_APEX_DATA0                             ((uint8_t)0x31)
#define ICM42688_APEX_DATA1                             ((uint8_t)0x32)
#define ICM42688_APEX_DATA2                             ((uint8_t)0x33)
#define ICM42688_APEX_DATA3                             ((uint8_t)0x34)
#define ICM42688_APEX_DATA4                             ((uint8_t)0x35)
#define ICM42688_APEX_DATA5                             ((uint8_t)0x36)
#define ICM42688_INT_STATUS2                            ((uint8_t)0x37)
#define ICM42688_INT_STATUS3                            ((uint8_t)0x38)
#define ICM42688_SIGNAL_PATH_RESET                      ((uint8_t)0x4B)
#define ICM42688_INTF_CONFIG0                           ((uint8_t)0x4C)
#define ICM42688_INTF_CONFIG1                           ((uint8_t)0x4D)
#define ICM42688_PWR_MGMT0                              ((uint8_t)0x4E)
#define ICM42688_GYRO_CONFIG0                           ((uint8_t)0x4F)
#define ICM42688_ACCEL_CONFIG0                          ((uint8_t)0x50)
#define ICM42688_GYRO_CONFIG1                           ((uint8_t)0x51)
#define ICM42688_GYRO_ACCEL_CONFIG0                     ((uint8_t)0x52)
#define ICM42688_ACCEL_CONFIG1                          ((uint8_t)0x53)
#define ICM42688_TMST_CONFIG                            ((uint8_t)0x54)
#define ICM42688_APEX_CONFIG0                           ((uint8_t)0x56)
#define ICM42688_SMD_CONFIG                             ((uint8_t)0x57)
#define ICM42688_FIFO_CONFIG1                           ((uint8_t)0x5F)
#define ICM42688_FIFO_CONFIG2                           ((uint8_t)0x60)
#define ICM42688_FIFO_CONFIG3                           ((uint8_t)0x61)
#define ICM42688_FSYNC_CONFIG                           ((uint8_t)0x62)
#define ICM42688_INT_CONFIG0                            ((uint8_t)0x63)
#define ICM42688_INT_CONFIG1                            ((uint8_t)0x64)
#define ICM42688_INT_SOURCE0                            ((uint8_t)0x65)
#define ICM42688_INT_SOURCE1                            ((uint8_t)0x66)
#define ICM42688_INT_SOURCE3                            ((uint8_t)0x68)
#define ICM42688_INT_SOURCE4                            ((uint8_t)0x69)
#define ICM42688_FIFO_LOST_PKT0                         ((uint8_t)0x6C)
#define ICM42688_FIFO_LOST_PKT1                         ((uint8_t)0x6D)
#define ICM42688_SELF_TEST_CONFIG                       ((uint8_t)0x70)
#define ICM42688_WHO_AM_I                               ((uint8_t)0x75)
#define ICM42688_REG_BANK_SEL                           ((uint8_t)0x76)

// BANK 1
#define ICM42688_SENSOR_CONFIG0                         ((uint8_t)0x03)
#define ICM42688_GYRO_CONFIG_STATIC2                    ((uint8_t)0x0B)
#define ICM42688_GYRO_CONFIG_STATIC3                    ((uint8_t)0x0C)
#define ICM42688_GYRO_CONFIG_STATIC4                    ((uint8_t)0x0D)
#define ICM42688_GYRO_CONFIG_STATIC5                    ((uint8_t)0x0E)
#define ICM42688_GYRO_CONFIG_STATIC6                    ((uint8_t)0x0F)
#define ICM42688_GYRO_CONFIG_STATIC7                    ((uint8_t)0x10)
#define ICM42688_GYRO_CONFIG_STATIC8                    ((uint8_t)0x11)
#define ICM42688_GYRO_CONFIG_STATIC9                    ((uint8_t)0x12)
#define ICM42688_GYRO_CONFIG_STATIC10                   ((uint8_t)0x13)
#define ICM42688_XG_ST_DATA                             ((uint8_t)0x5F)
#define ICM42688_YG_ST_DATA                             ((uint8_t)0x60)
#define ICM42688_ZG_ST_DATA                             ((uint8_t)0x61)
#define ICM42688_TMSTVAL0                               ((uint8_t)0x62)
#define ICM42688_TMSTVAL1                               ((uint8_t)0x63)
#define ICM42688_TMSTVAL2                               ((uint8_t)0x64)
#define ICM42688_INTF_CONFIG4                           ((uint8_t)0x7A)
#define ICM42688_INTF_CONFIG5                           ((uint8_t)0x7B)
#define ICM42688_INTF_CONFIG6                           ((uint8_t)0x7C)

// BANK 2
#define ICM42688_ACCEL_CONFIG_STATIC2                   ((uint8_t)0x03)
#define ICM42688_ACCEL_CONFIG_STATIC3                   ((uint8_t)0x04)
#define ICM42688_ACCEL_CONFIG_STATIC4                   ((uint8_t)0x05)
#define ICM42688_XA_ST_DATA                             ((uint8_t)0x3B)
#define ICM42688_YA_ST_DATA                             ((uint8_t)0x3C)
#define ICM42688_ZA_ST_DATA                             ((uint8_t)0x3D)

// BANK 4
#define ICM42688_APEX_CONFIG1                           ((uint8_t)0x40)
#define ICM42688_APEX_CONFIG2                           ((uint8_t)0x41)
#define ICM42688_APEX_CONFIG3                           ((uint8_t)0x42)
#define ICM42688_APEX_CONFIG4                           ((uint8_t)0x43)
#define ICM42688_APEX_CONFIG5                           ((uint8_t)0x44)
#define ICM42688_APEX_CONFIG6                           ((uint8_t)0x45)
#define ICM42688_APEX_CONFIG7                           ((uint8_t)0x46)
#define ICM42688_APEX_CONFIG8                           ((uint8_t)0x47)
#define ICM42688_APEX_CONFIG9                           ((uint8_t)0x48)
#define ICM42688_ACCEL_WOM_X_THR                        ((uint8_t)0x4A)
#define ICM42688_ACCEL_WOM_Y_THR                        ((uint8_t)0x4B)
#define ICM42688_ACCEL_WOM_Z_THR                        ((uint8_t)0x4C)
#define ICM42688_INT_SOURCE6                            ((uint8_t)0x4D)
#define ICM42688_INT_SOURCE7                            ((uint8_t)0x4E)
#define ICM42688_INT_SOURCE8                            ((uint8_t)0x4F)
#define ICM42688_INT_SOURCE9                            ((uint8_t)0x50)
#define ICM42688_INT_SOURCE10                           ((uint8_t)0x51)
#define ICM42688_OFFSET_USER0                           ((uint8_t)0x77)
#define ICM42688_OFFSET_USER1                           ((uint8_t)0x78)
#define ICM42688_OFFSET_USER2                           ((uint8_t)0x79)
#define ICM42688_OFFSET_USER3                           ((uint8_t)0x7A)
#define ICM42688_OFFSET_USER4                           ((uint8_t)0x7B)
#define ICM42688_OFFSET_USER5                           ((uint8_t)0x7C)
#define ICM42688_OFFSET_USER6                           ((uint8_t)0x7D)
#define ICM42688_OFFSET_USER7                           ((uint8_t)0x7E)
#define ICM42688_OFFSET_USER8                           ((uint8_t)0x7F)


/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void        ICM42688_WriteReg( uint8_t writeAddr, uint8_t writeData );
void        ICM42688_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens );
uint8_t     ICM42688_ReadReg( uint8_t readAddr );
void        ICM42688_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens );
void        ICM42688_SetRegBank( uint32_t bank );

void        ICM42688_Config( void );
uint32_t    ICM42688_Init( void );

uint32_t    ICM42688_DeviceCheck( uint8_t deviceid );
double      ICM42688_GetGyrSensitivity( void );
double      ICM42688_GetAccSensitivity( void );
uint32_t    ICM42688_GetLsbData( int16_t *lsb );
uint32_t    ICM42688_GetRawData( float *raw );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
