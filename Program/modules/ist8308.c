/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 *  
 *  @file    ist8308.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\simui2c\nrf5x_i2cSimu.h"
#include "modules\ist8308.h"

/* Define ----------------------------------------------------------------------------------*/

#define IST8308_Delay(__MS)     delay_ms(__MS)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static SimuI2C_InitTypeDef hi2c;
static double magSensitivity = 0;
 
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  IST8308_WriteReg
 */
void IST8308_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
    SimuI2C_WriteReg(&hi2c, IST8308_I2C_ADDR, writeAddr, writeData);
}

/**
 *  @brief  IST8308_WriteRegs
 */
void IST8308_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
    SimuI2C_WriteRegs(&hi2c, IST8308_I2C_ADDR, writeAddr, writeData, lens);
}

/**
 *  @brief  IST8308_ReadReg
 */
uint8_t IST8308_ReadReg( uint8_t readAddr )
{
    uint8_t readData;
    SimuI2C_ReadReg(&hi2c, IST8308_I2C_ADDR, readAddr, &readData);
    return readData;
}

/**
 *  @brief  IST8308_ReadRegs
 */
void IST8308_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
    SimuI2C_ReadRegs(&hi2c, IST8308_I2C_ADDR, readAddr, readData, lens);
}

/**
 *  @brief  IST8308_Config
 */
void IST8308_Config( void )
{
    hi2c.PinSCL    = IMU_SCL_PIN;
    hi2c.PortSCL   = IMU_SCL_PORT;
    hi2c.PinSDA    = IMU_SDA_PIN;
    hi2c.PortSDA   = IMU_SDA_PORT;
    hi2c.Frequency = 5;
    SimuI2C_Config(&hi2c);
}

/**
 *  @brief  IST8308_Init
 */
uint32_t IST8308_Init( void )
{
    uint8_t deviceid;

    klogd("\n");
    klogd("  >> ist8308 initial (address %02X)\n", IST8308_I2C_ADDR);
    klogd("  ist8308 device id check           ... ");
    deviceid = IST8308_ReadReg(IST8308_WAI);
    klogd("0x%02X", deviceid);
    if (IST8308_DeviceCheck(deviceid) != KS_OK)
    {
        klogd(" ... error\n");
        return KS_ERROR;
    }
    else
    {
        klogd(" ... ok\n");
    }

    klogd("  ist8308 software reset\n");
    IST8308_Delay(10);
    IST8308_WriteReg(IST8308_CNTL3, 0x01);
    IST8308_Delay(50);

    uint8_t reg[4][2] =
    {
        {IST8308_CNTL1, 0x00},  // Noise Suppression Filter : Disable
//        {IST8308_CNTL1, 0x20},  // Noise Suppression Filter : Low
//        {IST8308_CNTL1, 0x40},  // Noise Suppression Filter : Middle
//        {IST8308_CNTL1, 0x60},  // Noise Suppression Filter : High

//        {IST8308_CNTL4, 0x01},  // Dynamic Range ±200uT, Sensitivity 13.2 LSB/uT
        {IST8308_CNTL4, 0x00},  // Dynamic Range ±500uT, Sensitivity 6.6 LSB/uT

//        {IST8308_OSRCNTL, 0x00},    // OSR =  1, ODR max = 1000Hz
//        {IST8308_OSRCNTL, 0x09},    // OSR =  2, ODR max = 500Hz
//        {IST8308_OSRCNTL, 0x12},    // OSR =  4, ODR max = 333Hz
        {IST8308_OSRCNTL, 0x1B},    // OSR =  8, ODR max = 200Hz
//        {IST8308_OSRCNTL, 0x24},    // OSR = 16, ODR max = 100Hz (Default)
//        {IST8308_OSRCNTL, 0x2D},    // OSR = 32, ODR max = 50Hz

//        {IST8308_CNTL2, 0x06},      // Continuous Measurement Mode with ODR 50Hz
//        {IST8308_CNTL2, 0x08},      // Continuous Measurement Mode with ODR 100Hz
//        {IST8308_CNTL2, 0x0A},      // Continuous Measurement Mode with ODR 200Hz
//        {IST8308_CNTL2, 0x11},      // Continuous Measurement Mode with ODR 333H
//        {IST8308_CNTL2, 0x12},      // Continuous Measurement Mode with ODR 500H
        {IST8308_CNTL2, 0x13}       // Continuous Measurement Mode with no-sleep
    };

    klogd("  ist8308 digital filter setting    ... %02Xh = 0x%02X\n", reg[0][0], reg[0][1]);
    IST8308_WriteReg(reg[0][0], reg[0][1]);
    klogd("  ist8308 dynamic range setting     ... %02Xh = 0x%02X\n", reg[1][0], reg[1][1]);
    IST8308_WriteReg(reg[1][0], reg[1][1]);
    klogd("  ist8308 over-sampling setting     ... %02Xh = 0x%02X\n", reg[2][0], reg[2][1]);
    IST8308_WriteReg(reg[2][0], reg[2][1]);
    klogd("  ist8308 output data rate setting  ... %02Xh = 0x%02X\n", reg[3][0], reg[3][1]);
    IST8308_WriteReg(reg[3][0], reg[3][1]);
    IST8308_Delay(10);

    switch (IST8308_ReadReg(IST8308_CNTL4) & 0x03)
    {
        case 0x00:  magSensitivity = 1.0 / IST8308_SENS_500UT_LSB;  break;
        case 0x01:  magSensitivity = 1.0 / IST8308_SENS_200UT_LSB;  break;
        default :   return KS_ERROR;
    }

    return KS_OK;
}

/**
 *  @brief  IST8308_DeviceCheck
 */
uint32_t IST8308_DeviceCheck( uint8_t deviceid )
{
#if 0
    printf(" ... 0x%02X (%02X) ... ", deviceid, IST8308_I2C_ADDR);
//    return KS_ERROR;
#endif

    if (deviceid != IST8308_DEVICD_ID)
    {
        return KS_ERROR;
    }

    return KS_OK;
}

/**
 *  @brief  IST8308_GetMagSensitivity
 */
double IST8308_GetMagSensitivity( void )
{
    return magSensitivity;
}

/**
 *  @brief  IST8308_GetLsbData
 */
uint32_t IST8308_GetLsbData( int16_t *lsb )
{
    uint8_t res[6] = {0};
    if ((IST8308_ReadReg(IST8308_STAT) & 0x01) != 0x01)
    {
        return KS_ERROR;
    }
    IST8308_ReadRegs(IST8308_DATAXL, res, 6);
    lsb[0] = (int16_t)(res[1] << 8) | res[0];
    lsb[1] = (int16_t)(res[3] << 8) | res[2];
    lsb[2] = (int16_t)(res[5] << 8) | res[4];
    return KS_OK;
}

/**
 *  @brief  IST8308_GetRawData
 */
uint32_t IST8308_GetRawData( float *raw )
{
    uint32_t status;
    int16_t res[3] = {0};
    status = IST8308_GetLsbData(res);
    if (status == KS_OK)
    {
        raw[0] = res[0] * magSensitivity;
        raw[1] = res[1] * magSensitivity;
        raw[2] = res[2] * magSensitivity;
    }
    return status;
}

/*************************************** END OF FILE ****************************************/
