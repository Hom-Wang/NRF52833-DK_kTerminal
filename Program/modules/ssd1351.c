/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    ssd1351.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_spim.h"
#include "modules\ssd1351.h"

/* Define ----------------------------------------------------------------------------------*/

#define OLED_USE_SIMU_SPI

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

#if !defined(OLED_USE_SIMU_SPI)
static SPIM_InitTypeDef holed;
#endif

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  SSD1351_WriteCmd
 */
void SSD1351_WriteCmd( uint8_t command )
{
#if !defined(OLED_USE_SIMU_SPI)
    uint8_t ack = 0;
    OLED_CS_L();
    OLED_DC_L();
    SPIM_Transceive(&holed, &command, 1, &ack, 0);
    OLED_DC_H();
    OLED_CS_H();

#else
    OLED_CS_L();
    OLED_DC_L();
    for (uint32_t i = 0; i < 8; i++)
    {
        OLED_SCK_L();
        if (command & 0x80)
        {
            OLED_SDI_H();
        }
        else
        {
            OLED_SDI_L();
        }
        command <<= 1;
        OLED_SCK_H();
    }
    OLED_CS_H();

#endif
}

/**
 *  @brief  SSD1351_WriteData
 */
void SSD1351_WriteData( uint8_t data )
{
#if !defined(OLED_USE_SIMU_SPI)
    uint8_t ack = 0;
    OLED_CS_L();
    OLED_DC_H();
    SPIM_Transceive(&holed, &data, 1, &ack, 0);
    OLED_DC_H();
    OLED_CS_H();

#else
    OLED_CS_L();
    OLED_DC_H();
    for (uint32_t i = 0; i < 8; i++)
    {
        OLED_SCK_L();
        if (data & 0x80)
        {
            OLED_SDI_H();
        }
        else
        {
            OLED_SDI_L();
        }
        data <<= 1;
        OLED_SCK_H();
    }
    OLED_CS_H();

#endif
}

/**
 *  @brief  SSD1351_WriteColor
 */
void SSD1351_WriteColor( uint16_t color )
{
#if !defined(OLED_USE_SIMU_SPI)
    uint8_t wbuf[2], ack = 0;
    OLED_CS_L();
    OLED_DC_H();
    wbuf[0] = BYTE16U8H(color);
    wbuf[1] = BYTE16U8L(color);
    SPIM_Transceive(&holed, wbuf, 2, &ack, 0);
    OLED_CS_H();

#else
    OLED_CS_L();
    OLED_DC_H();
    for (uint32_t i = 0; i < 16; i++)
    {
        OLED_SCK_L();
        if (color & 0x8000)
        {
            OLED_SDI_H();
        }
        else
        {
            OLED_SDI_L();
        }
        color <<= 1;
        OLED_SCK_H();
    }
    OLED_CS_H();

#endif
}

/**
 *  @brief  SSD1351_Config
 */
void SSD1351_Config( void )
{
    OLED_CS_H();
    OLED_DC_H();
    OLED_RST_H();
//    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(OLED_CS_PORT, OLED_CS_PIN));
//    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(OLED_DC_PORT, OLED_DC_PIN));
//    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(OLED_RST_PORT, OLED_RST_PIN));
    nrf_gpio_cfg(
        NRF_GPIO_PIN_MAP(OLED_CS_PORT, OLED_CS_PIN),
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_PULLUP,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE
    );
    nrf_gpio_cfg(
        NRF_GPIO_PIN_MAP(OLED_DC_PORT, OLED_DC_PIN),
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_PULLUP,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE
    );
    nrf_gpio_cfg(
        NRF_GPIO_PIN_MAP(OLED_RST_PORT, OLED_RST_PIN),
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_PULLUP,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE
    );

#if !defined(OLED_USE_SIMU_SPI)
    holed.Instance  = OLED_SPIMx;
    holed.PinCS     = NRF_GPIO_PIN_MAP(OLED_CS_PORT, OLED_CS_PIN);
    holed.PinSCK    = NRF_GPIO_PIN_MAP(OLED_SCK_PORT, OLED_SCK_PIN);
    holed.PinMOSI   = NRF_GPIO_PIN_MAP(OLED_SDI_PORT, OLED_SDI_PIN);
    holed.PinMISO   = NRF_GPIO_PIN_MAP(OLED_SDO_PORT, OLED_SDO_PIN);
    holed.BitOrder  = NRF_SPIM_BIT_ORDER_MSB_FIRST;
    holed.Mode      = NRF_SPIM_MODE_0;
    holed.Freguency = NRF_SPIM_FREQ_2M;
    SPIM_Init(&holed);
    SPIM_Cmd(&holed, ENABLE);

#else
    OLED_SCK_H();
    OLED_SDI_H();
//    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(OLED_SCK_PORT, OLED_SCK_PIN));
//    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(OLED_SDI_PORT, OLED_SDI_PIN));
    nrf_gpio_cfg(
        NRF_GPIO_PIN_MAP(OLED_SCK_PORT, OLED_SCK_PIN),
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_PULLUP,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE
    );
    nrf_gpio_cfg(
        NRF_GPIO_PIN_MAP(OLED_SDI_PORT, OLED_SDI_PIN),
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_PULLUP,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE
    );
#endif
}

/**
 *  @brief  SSD1351_Init
 */
void SSD1351_Init( void )
{
    // reset
    OLED_RST_L();
    delay_ms(10);
    OLED_RST_H();
    delay_ms(20);

    // init
    SSD1351_WriteCmd(0xFD);     // Unlock Driver IC
    SSD1351_WriteData(0x12);
    SSD1351_WriteCmd(0xFD);     // Unlock All Commands
    SSD1351_WriteData(0xB1);

    SSD1351_Display(DISABLE);   // Display Off

    SSD1351_WriteCmd(0xB3);     // Set Clock as 90 Frames/Sec
    SSD1351_WriteCmd(0xF1);
    SSD1351_WriteCmd(0xCA);     // 1/128 Duty
    SSD1351_WriteData(0x7F);
    SSD1351_WriteCmd(0xA0);     // Set Horizontal Address Increment
    SSD1351_WriteData(0x74);

    SSD1351_WriteCmd(0xA1);     // Set Mapping RAM Display Start Line
    SSD1351_WriteData(0x00);
    SSD1351_WriteCmd(0xA2);     // Shift Mapping RAM Counter
    SSD1351_WriteData(0x00);
    SSD1351_WriteCmd(0xB5);     // Disable GPIO Pins Input
    SSD1351_WriteData(0x00);
    SSD1351_WriteCmd(0xAB);     // Enable Internal VDD Regulator
    SSD1351_WriteData(0x01);
    SSD1351_WriteCmd(0xB4);     // Enable External VSL
    SSD1351_WriteData(0xA0);
    SSD1351_WriteData(0xB5);
    SSD1351_WriteData(0x55);
    SSD1351_WriteCmd(0xC1);     // Set Contrast of Color
    SSD1351_WriteData(0xC8);
    SSD1351_WriteData(0x80);
    SSD1351_WriteData(0xC8);
    SSD1351_WriteCmd(0xC7);     // Set Scale Factor of Segment Output Current Control
    SSD1351_WriteData(0x0F);
    SSD1351_WriteCmd(0xB8);     // Set Pulse Width for Gray Scale Table
    SSD1351_WriteData(0x02);
    SSD1351_WriteData(0x03);
    SSD1351_WriteData(0x04);
    SSD1351_WriteData(0x05);
    SSD1351_WriteData(0x06);
    SSD1351_WriteData(0x07);
    SSD1351_WriteData(0x08);
    SSD1351_WriteData(0x09);
    SSD1351_WriteData(0x0A);
    SSD1351_WriteData(0x0B);
    SSD1351_WriteData(0x0C);
    SSD1351_WriteData(0x0D);
    SSD1351_WriteData(0x0E);
    SSD1351_WriteData(0x0F);
    SSD1351_WriteData(0x10);
    SSD1351_WriteData(0x11);
    SSD1351_WriteData(0x12);
    SSD1351_WriteData(0x13);
    SSD1351_WriteData(0x15);
    SSD1351_WriteData(0x17);
    SSD1351_WriteData(0x19);
    SSD1351_WriteData(0x1B);
    SSD1351_WriteData(0x1D);
    SSD1351_WriteData(0x1F);
    SSD1351_WriteData(0x21);
    SSD1351_WriteData(0x23);
    SSD1351_WriteData(0x25);
    SSD1351_WriteData(0x27);
    SSD1351_WriteData(0x2A);
    SSD1351_WriteData(0x2D);
    SSD1351_WriteData(0x30);
    SSD1351_WriteData(0x33);
    SSD1351_WriteData(0x36);
    SSD1351_WriteData(0x39);
    SSD1351_WriteData(0x3C);
    SSD1351_WriteData(0x3F);
    SSD1351_WriteData(0x42);
    SSD1351_WriteData(0x45);
    SSD1351_WriteData(0x48);
    SSD1351_WriteData(0x4C);
    SSD1351_WriteData(0x50);
    SSD1351_WriteData(0x54);
    SSD1351_WriteData(0x58);
    SSD1351_WriteData(0x5C);
    SSD1351_WriteData(0x60);
    SSD1351_WriteData(0x64);
    SSD1351_WriteData(0x68);
    SSD1351_WriteData(0x6C);
    SSD1351_WriteData(0x70);
    SSD1351_WriteData(0x74);
    SSD1351_WriteData(0x78);
    SSD1351_WriteData(0x7D);
    SSD1351_WriteData(0x82);
    SSD1351_WriteData(0x87);
    SSD1351_WriteData(0x8C);
    SSD1351_WriteData(0x91);
    SSD1351_WriteData(0x96);
    SSD1351_WriteData(0x9B);
    SSD1351_WriteData(0xA0);
    SSD1351_WriteData(0xA5);
    SSD1351_WriteData(0xAA);
    SSD1351_WriteData(0xAF);
    SSD1351_WriteData(0xB4);
    SSD1351_WriteCmd(0xB1);     // Set Phase 1 as 5 Clocks & Phase 2 as 3 Clocks
    SSD1351_WriteData(0x32);
    SSD1351_WriteCmd(0xBB);     // Set Pre-Charge Voltage Level as 0.50 * VCC
    SSD1351_WriteData(0x17);
    SSD1351_WriteCmd(0xB2);     // Enhance Display Performance
    SSD1351_WriteData(0xA4);
    SSD1351_WriteData(0x00);
    SSD1351_WriteData(0x00);
    SSD1351_WriteCmd(0xB6);     // Set Second Pre-Charge Period as 1 Clock
    SSD1351_WriteData(0x01);
    SSD1351_WriteCmd(0xBE);     // Set Common Pins Deselect Voltage Level as 0.82 * VCC
    SSD1351_WriteData(0x05);
    SSD1351_WriteCmd(0xA6);     // Normal Display Mode

    SSD1351_Clear(0x0000);
    SSD1351_Display(ENABLE);
}

/**
 *  @brief  SSD1351_Init
 */
void SSD1351_Display( uint8_t cmd )
{
    if (cmd != ENABLE)
    {
        SSD1351_WriteCmd(0xAE);
    }
    else
    {
        SSD1351_WriteCmd(0xAF);
    }
}

/**
 *  @brief  SSD1351_Init
 */
void SSD1351_Clear( uint16_t color )
{
    uint32_t point = SSD1351_WIDTH * SSD1351_HIGHT;
    SSD1351_SetWindow(0, 0, SSD1351_WIDTH - 1, SSD1351_HIGHT - 1);
    while (point--)
    {
        SSD1351_WriteColor(color);
    }
}

/**
 *  @brief  SSD1351_Init
 */
void SSD1351_SetWindow( int16_t posX1, int16_t posY1, int16_t posX2, int16_t posY2 )
{
    SSD1351_WriteCmd(0x15);
    SSD1351_WriteData(posX1);
    SSD1351_WriteData(posX2);
    SSD1351_WriteCmd(0x75);
    SSD1351_WriteData(posY1);
    SSD1351_WriteData(posY2);
    SSD1351_WriteCmd(0x5C);
}

/*************************************** END OF FILE ****************************************/
