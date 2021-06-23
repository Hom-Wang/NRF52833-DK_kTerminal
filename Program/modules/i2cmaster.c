/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    i2cmaster.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\simui2c\nrf5x_i2cSimu.h"

#include "i2cmaster.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static SimuI2C_InitTypeDef hi2c;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  i2c_config
 */
void i2c_config( const uint8_t sclport, const uint8_t sclpin, const uint8_t sdaport, const uint8_t sdapin, uint32_t freq )
{
    hi2c.PinSCL    = sclpin;
    hi2c.PortSCL   = sclport;
    hi2c.PinSDA    = sdapin;
    hi2c.PortSDA   = sdaport;
    hi2c.Frequency = freq;
    SimuI2C_Config(&hi2c);
}

/**
 *  @brief  i2c_write_byte
 */
void i2c_write_byte( uint8_t address, uint8_t reg, uint8_t data )
{
    SimuI2C_WriteReg(&hi2c, address, reg, data);
//    klogd("i2c_write_byte(0x%02X, 0x%02X, 0x%02X)\n", address, reg, data);
}

/**
 *  @brief  i2c_write_byte
 */
void i2c_write( uint8_t address, uint8_t reg, uint8_t length, uint8_t *data )
{
    SimuI2C_WriteRegs(&hi2c, address, reg, data, length);
}

/**
 *  @brief  i2c_read_byte
 */
uint8_t i2c_read_byte( uint8_t address, uint8_t reg )
{
    uint8_t data;
    SimuI2C_ReadReg(&hi2c, address, reg, &data);
//    klogd("0x%02X = i2c_read_byte(0x%02X, 0x%02X)\n", data, address, reg);
    return data;
}

/**
 *  @brief  i2c_read
 */
void i2c_read( uint8_t address, uint8_t reg, uint8_t length, uint8_t *data )
{
    SimuI2C_ReadRegs(&hi2c, address, reg, data, length);
}

/**
 *  @brief  i2c_scan_device
 */
uint32_t i2c_scan_device( uint8_t *address )
{
    return SimuI2C_ScanDevice(&hi2c, address);
}

/**
 *  @brief  i2c_scan_regs
 */
uint32_t i2c_scan_regs( const uint8_t address, uint8_t *reg )
{
    return SimuI2C_ScanRegs(&hi2c, address, reg);
}

/**
 *  @brief  i2c_scan_all
 */
void i2c_scan_all( const uint8_t sclpin, const uint8_t sdapin )
{
    SimuI2C_ScanAllGPIO(sclpin, sdapin);
}

/*************************************** END OF FILE ****************************************/
