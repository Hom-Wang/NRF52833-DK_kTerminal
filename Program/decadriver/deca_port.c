/*! ----------------------------------------------------------------------------
 * @file    deca_port.h
 * @brief   HW specific definitions and functions for portability
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

#include "drivers\nrf5x_spim.h"

#include "deca_port.h"
#include "deca_device_api.h"

static SPIM_InitTypeDef hdw1000;

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      dw1000_setup()
 * @brief   setup the DW1000
 */
void dw1000_setup(void)
{
    setup_DW1000RSTnIRQ(DISABLE);
    reset_DW1000();
    dw1000_spi_open();
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      dw1000_init()
 * @brief   initialize the DW1000
 */
uint32_t dw1000_init(dwt_config_t *config)
{
    uint32_t status;
    // set low rate spi to initialize
    dw1000_spi_slowrate();
#if 1
    printf("dwt_readdevid = 0x%08X\n", (uint32_t)dwt_readdevid());
#endif
    // initialize DW1000
    status = dwt_initialise(DWT_LOADUCODE);
    if (status != DWT_SUCCESS)
    {
        return status;
    }
    // set fast rate spi to configure
    dw1000_spi_fastrate();
    // configure DW1000
    dwt_configure(config);
    return status;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      dw1000_reset()
 * @brief   DW_RESET pin on DW1000 has 2 functions
 *          In general it is output, but it also can be used to reset the digital
 *          part of DW1000 by driving this pin low.
 *          Note, the DW_RESET pin should not be driven high externally.
 */
void dw1000_reset(void)
{
    uint32_t rstpin = NRF_GPIO_PIN_MAP(DW1000_RST_PORT, DW1000_RST_PIN);
    nrf_gpio_cfg_output(rstpin);
    nrf_gpio_pin_clear(rstpin);  
    deca_sleep(2);
    nrf_gpio_cfg_input(rstpin, NRF_GPIO_PIN_NOPULL);
    deca_sleep(2);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      dw1000_setirq()
 * @brief   setup the DW_IRQ pin mode
 *          0 - output Open collector mode
 *          !0 - input mode with connected EXTI0 IRQ
 */
void dw1000_setirq(int enable)
{
    uint32_t irqpin = NRF_GPIO_PIN_MAP(DW1000_RST_PORT, DW1000_RST_PIN);
    if (enable)
    {
        
    }
    else
    {
        nrf_gpio_cfg_input(irqpin, NRF_GPIO_PIN_NOPULL);
    }
}
/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      dw1000_spi_slowrate()
 * @brief   Set SPI rate to less than 3 MHz to properly perform DW1000 initialisation.
 */
void dw1000_spi_slowrate(void)
{
    SPIM_SetFrequency(&hdw1000, DW1000_SPIMx_SLOW_RATE);
    delay_ms(2);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      dw1000_spi_fastrate()
 * @brief   Set SPI rate as close to 20 MHz as possible for optimum performances.
 */
void dw1000_spi_fastrate(void)
{
    SPIM_SetFrequency(&hdw1000, DW1000_SPIMx_FAST_RATE);
    delay_ms(2);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      dw1000_spi_open()
 * @brief   Low level abstract function to open and initialise access to the SPI device.
 *          returns 0 for success, or -1 for error
 */
int dw1000_spi_open(void)
{
    hdw1000.Instance  = DW1000_SPIMx;
    hdw1000.PinCS     = DW1000_CSN_PIN;
    hdw1000.PinSCK    = DW1000_SCK_PIN;
    hdw1000.PinMOSI   = DW1000_SDI_PIN;
    hdw1000.PinMISO   = DW1000_SDO_PIN;
    hdw1000.BitOrder  = DW1000_SPIMx_BITORDER;
    hdw1000.Mode      = DW1000_SPIMx_MODE;
    hdw1000.Freguency = DW1000_SPIMx_SLOW_RATE;
    SPIM_Init(&hdw1000);
    SPIM_Cmd(&hdw1000, ENABLE);
    return 0;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      dw1000_spi_close()
 * @brief   Low level abstract function to close the the SPI device.
 *          returns 0 for success, or -1 for error
 */
int dw1000_spi_close(void)
{
    SPIM_Cmd(&hdw1000, DISABLE);
    return 0;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      port_wakeup_dw1000()
 * @brief   "slow" waking up of DW1000 using DW_CS only
 */
void port_wakeup_dw1000(void)
{
    // TODO
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      port_wakeup_dw1000_fast()
 * @brief   waking up of DW1000 using DW_CS and DW_RESET pins.
 *          The DW_RESET signalling that the DW1000 is in the INIT state.
 *          the total fast wakeup takes ~2.2ms and depends on crystal startup time
 */
void port_wakeup_dw1000_fast(void)
{
    // TODO
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      port_DisableEXT_IRQ()
 * @brief   wrapper to disable DW_IRQ pin IRQ
 */
void port_DisableEXT_IRQ(void)
{
    // TODO
} 

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      port_EnableEXT_IRQ()
 * @brief   wrapper to enable DW_IRQ pin IRQ
 */
void port_EnableEXT_IRQ(void)
{
    // TODO
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      port_GetEXT_IRQStatus()
 * @brief   wrapper to read a DW_IRQ pin IRQ status
 */
uint32_t port_GetEXT_IRQStatus(void)
{
    // TODO
    return 0;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      port_CheckEXT_IRQ()
 * @brief   wrapper to read DW_IRQ input pin state
 */
uint32_t port_CheckEXT_IRQ(void)
{
    // TODO
    return 0;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      deca_sleep()
 * @brief   Wait for a given amount of time.
 */
void deca_sleep(unsigned int time_ms)
{
    delay_ms(time_ms);
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      writetospi()
 * @brief   Low level abstract function to write to the SPI
 *          Takes two separate byte buffers for write header and write data
 *          returns 0 for success, or -1 for error
 */
int writetospi(uint16 headerLength, const uint8 *headerBuffer, uint32 bodylength, const uint8 *bodyBuffer)
{
    uint8 * p1;
    uint32 idatalength=0;

    idatalength = headerLength + bodylength;

    uint8 idatabuf[idatalength];
    uint8 itempbuf[idatalength];

    memset(idatabuf, 0, idatalength);
    memset(itempbuf, 0, idatalength);

    p1 = idatabuf;
    memcpy(p1, headerBuffer, headerLength);
    p1 += headerLength;
    memcpy(p1, bodyBuffer, bodylength);

    nrf_gpio_pin_clear(DW1000_CSN_PIN);
    SPIM_Transceive(&hdw1000, idatabuf, idatalength, itempbuf, idatalength);
    nrf_gpio_pin_set(DW1000_CSN_PIN);

    return 0;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      readfromspi()
 * @brief   Low level abstract function to read from the SPI
 *          Takes two separate byte buffers for write header and read data
 *          returns the offset into read buffer where first byte of read data may be found,
 *          or returns -1 if there was an error
 */
int readfromspi(uint16 headerLength, const uint8 *headerBuffer, uint32 readlength, uint8 *readBuffer)
{
    uint8 * p1;
    uint32 idatalength=0;

    idatalength= headerLength + readlength;

    uint8 idatabuf[idatalength];
    uint8 itempbuf[idatalength];

    memset(idatabuf, 0, idatalength);
    memset(itempbuf, 0, idatalength);

    p1=idatabuf;
    memcpy(p1,headerBuffer, headerLength);

    p1 += headerLength;
    memset(p1,0x00,readlength);

    nrf_gpio_pin_clear(DW1000_CSN_PIN);
    SPIM_Transceive(&hdw1000, idatabuf, idatalength, itempbuf, idatalength);
    nrf_gpio_pin_set(DW1000_CSN_PIN);

    p1=itempbuf + headerLength;

    memcpy(readBuffer, p1, readlength);
    return 0;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      decamutexon()
 * @brief   This function should disable interrupts. This is called at the start of a critical section
 *          It returns the irq state before disable, this value is used to re-enable in decamutexoff call
 */
decaIrqStatus_t decamutexon(void)
{
    decaIrqStatus_t s = port_GetEXT_IRQStatus();
    if (s)
    {
        // disable the external interrupt line
        port_DisableEXT_IRQ();
    }
    // return state before disable, value is used to re-enable in decamutexoff call
    return s;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn      decamutexoff()
 * @brief   This function should re-enable interrupts, or at least restore their state as returned(&saved) by decamutexon
 *          This is called at the end of a critical section
 */
void decamutexoff(decaIrqStatus_t s)
{
    // put a function here that re-enables the interrupt at the end of the critical section
    if (s)
    {
        // need to check the port state as we can't use level sensitive interrupt on the STM ARM
        // port_EnableEXT_IRQ();
    }
}

