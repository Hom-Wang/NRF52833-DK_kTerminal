/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 *  
 *  @file    icm42688.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\simui2c\nrf5x_i2cSimu.h"
#include "modules\icm42688.h"

/* Define ----------------------------------------------------------------------------------*/

#define ICM42688_Delay(__MS)     delay_ms(__MS)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static SimuI2C_InitTypeDef hi2c;
static double gyrSensitivity = 0;
static double accSensitivity = 0;
 
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  ICM42688_WriteReg
 */
void ICM42688_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
    SimuI2C_WriteReg(&hi2c, ICM42688_I2C_ADDR, writeAddr, writeData);
}

/**
 *  @brief  ICM42688_WriteRegs
 */
void ICM42688_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
    SimuI2C_WriteRegs(&hi2c, ICM42688_I2C_ADDR, writeAddr, writeData, lens);
}

/**
 *  @brief  ICM42688_ReadReg
 */
uint8_t ICM42688_ReadReg( uint8_t readAddr )
{
    uint8_t readData;
    SimuI2C_ReadReg(&hi2c, ICM42688_I2C_ADDR, readAddr, &readData);
    return readData;
}

/**
 *  @brief  ICM42688_ReadRegs
 */
void ICM42688_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
    SimuI2C_ReadRegs(&hi2c, ICM42688_I2C_ADDR, readAddr, readData, lens);
}

/**
 *  @brief  ICM42688_SetRegBank
 */
void ICM42688_SetRegBank( uint32_t bank )
{
    SimuI2C_WriteReg(&hi2c, ICM42688_I2C_ADDR, ICM42688_REG_BANK_SEL, bank);
}

/**
 *  @brief  ICM42688_Config
 */
void ICM42688_Config( void )
{
    IMU_CSM_H();
    IMU_SDO_L();

    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(IMU_CSM_PORT, IMU_CSM_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(IMU_SDO_PORT, IMU_SDO_PIN));
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(IMU_INT_PORT, IMU_INT_PIN), NRF_GPIO_PIN_PULLUP);

    hi2c.PinSCL    = IMU_SCK_PIN;
    hi2c.PortSCL   = IMU_SCK_PORT;
    hi2c.PinSDA    = IMU_SDI_PIN;
    hi2c.PortSDA   = IMU_SDI_PORT;
    hi2c.Frequency = 5;
    SimuI2C_Config(&hi2c);
}

/**
 *  @brief  ICM42688_Init
 */
uint32_t ICM42688_Init( void )
{
    uint8_t deviceid;

    klogd("\n");
    klogd("  >> icm42688 initial (address %02X)\n", ICM42688_I2C_ADDR);
    klogd("  icm42688 device id check          ... ");
    deviceid = ICM42688_ReadReg(ICM42688_WHO_AM_I);
    klogd("0x%02X", deviceid);
    if (ICM42688_DeviceCheck(deviceid) != KS_OK)
    {
        klogd(" ... error\n");
        return KS_ERROR;
    }
    else
    {
        klogd(" ... ok\n");
    }

    klogd("  icm42688 software reset\n");
    ICM42688_Delay(10);
    ICM42688_WriteReg(ICM42688_DEVICE_CONFIG, 0x01);
    ICM42688_Delay(50);

    uint8_t reg[3][2] =
    {
        {ICM42688_GYRO_CONFIG0, 0x06},      // ODR = 1KHz, ±2000dps
//        {ICM42688_GYRO_CONFIG0, 0x26},      // ODR = 1KHz, ±1000dps
//        {ICM42688_GYRO_CONFIG0, 0x46},      // ODR = 1KHz, ±500dps
//        {ICM42688_GYRO_CONFIG0, 0x66},      // ODR = 1KHz, ±250dps
//        {ICM42688_GYRO_CONFIG0, 0x86},      // ODR = 1KHz, ±125dps
//        {ICM42688_GYRO_CONFIG0, 0xA6},      // ODR = 1KHz, ±62.5dps
//        {ICM42688_GYRO_CONFIG0, 0xC6},      // ODR = 1KHz, ±31.25dps
//        {ICM42688_GYRO_CONFIG0, 0xE6},      // ODR = 1KHz, ±15.625dps

//        {ICM42688_ACCEL_CONFIG0, 0x06},     // ODR = 1KHz, ±16g
//        {ICM42688_ACCEL_CONFIG0, 0x26},     // ODR = 1KHz, ±8g
        {ICM42688_ACCEL_CONFIG0, 0x46},     // ODR = 1KHz, ±4g
//        {ICM42688_ACCEL_CONFIG0, 0x66},     // ODR = 1KHz, ±2g

        {ICM42688_PWR_MGMT0, 0x0F}          // enable gyr and acc, Low Noise Mode
    };

    klogd("  icm42688 gyro dynamic range       ... %02Xh = 0x%02X\n", reg[0][0], reg[0][1]);
    ICM42688_WriteReg(reg[0][0], reg[0][1]);
    klogd("  icm42688 accel dynamic range      ... %02Xh = 0x%02X\n", reg[1][0], reg[1][1]);
    ICM42688_WriteReg(reg[1][0], reg[1][1]);
    klogd("  icm42688 enable gyro and accel    ... %02Xh = 0x%02X\n", reg[2][0], reg[2][1]);
    ICM42688_WriteReg(reg[2][0], reg[2][1]);
    ICM42688_Delay(10);

    switch (ICM42688_ReadReg(ICM42688_GYRO_CONFIG0) & 0xE0)
    {
        case 0x00:  gyrSensitivity = 1.0 / 16.4;    break;  // ±2000dps
        case 0x20:  gyrSensitivity = 1.0 / 32.8;    break;  // ±1000dps
        case 0x40:  gyrSensitivity = 1.0 / 65.5;    break;  // ±500dps
        case 0x60:  gyrSensitivity = 1.0 / 131;     break;  // ±250dps
        case 0x80:  gyrSensitivity = 1.0 / 262;     break;  // ±125dps
        case 0xA0:  gyrSensitivity = 1.0 / 524.3;   break;  // ±62.5dps
        case 0xC0:  gyrSensitivity = 1.0 / 1048.6;  break;  // ±31.25dps
        case 0xE0:  gyrSensitivity = 1.0 / 2097.2;  break;  // ±15.625dps
    }

    switch (ICM42688_ReadReg(ICM42688_ACCEL_CONFIG0) & 0xE0)
    {
        case 0x00:  accSensitivity = 1.0 / 2.048;   break;  // ±16g
        case 0x20:  accSensitivity = 1.0 / 4.096;   break;  // ±8g
        case 0x40:  accSensitivity = 1.0 / 8.192;   break;  // ±4g
        case 0x60:  accSensitivity = 1.0 / 16.384;  break;  // ±2g
        default :   return KS_ERROR;
    }

    return KS_OK;
}

/**
 *  @brief  ICM42688_DeviceCheck
 */
uint32_t ICM42688_DeviceCheck( uint8_t deviceid )
{
#if 0
    printf(" ... 0x%02X (%02X) ... ", deviceid, ICM42688_I2C_ADDR);
//    return KS_ERROR;
#endif

    if (deviceid != ICM42688_DEVICD_ID)
    {
        return KS_ERROR;
    }

    return KS_OK;
}

/**
 *  @brief  ICM42688_GetGyrSensitivity
 */
double ICM42688_GetGyrSensitivity( void )
{
    return gyrSensitivity;
}

/**
 *  @brief  ICM42688_GetAccSensitivity
 */
double ICM42688_GetAccSensitivity( void )
{
    return accSensitivity;
}

/**
 *  @brief  ICM42688_GetLsbData
 */
uint32_t ICM42688_GetLsbData( int16_t *lsb )
{
    uint8_t res[14] = {0};
    ICM42688_ReadRegs(ICM42688_TEMP_DATA1, res, 14);
    lsb[0] = (int16_t)(res[0]  << 8) | res[1];  // temp
    lsb[1] = (int16_t)(res[2]  << 8) | res[3];  // acc.x
    lsb[2] = (int16_t)(res[4]  << 8) | res[5];  // acc.y
    lsb[3] = (int16_t)(res[6]  << 8) | res[7];  // acc.z
    lsb[4] = (int16_t)(res[8]  << 8) | res[9];  // gyr.x
    lsb[5] = (int16_t)(res[10] << 8) | res[11]; // gyr.y
    lsb[6] = (int16_t)(res[12] << 8) | res[13]; // gyr.z
    return KS_OK;
}

/**
 *  @brief  ICM42688_GetRawData
 */
uint32_t ICM42688_GetRawData( float *raw )
{
    uint32_t status;
    int16_t res[7] = {0};
    status = ICM42688_GetLsbData(res);
    if (status == KS_OK)
    {
        raw[0] = res[0] / 132.48f + 25;
        raw[1] = res[1] * accSensitivity;
        raw[2] = res[2] * accSensitivity;
        raw[3] = res[3] * accSensitivity;
        raw[4] = res[4] * gyrSensitivity;
        raw[5] = res[5] * gyrSensitivity;
        raw[6] = res[6] * gyrSensitivity;
    }
    return status;
}

/*************************************** END OF FILE ****************************************/
