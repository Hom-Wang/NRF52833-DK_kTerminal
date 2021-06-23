/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_spim.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_spim.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  SPIM_Init
 */
void SPIM_Init( SPIM_InitTypeDef *hspim )
{
    uint32_t config = 0;

    nrf_gpio_pin_set(hspim->PinCS);
    nrf_gpio_cfg_output(hspim->PinCS);

    // sck
    if ((hspim->Mode == NRF_SPIM_MODE_0) || (hspim->Mode == NRF_SPIM_MODE_1))
    {
        nrf_gpio_pin_clear(hspim->PinSCK);
    }
    else
    {
        nrf_gpio_pin_set(hspim->PinSCK);
    }
    nrf_gpio_cfg(
        hspim->PinSCK,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_CONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_S0S1,
        NRF_GPIO_PIN_NOSENSE
    );

    // mosi
    nrf_gpio_pin_clear(hspim->PinMOSI);
    nrf_gpio_cfg_output(hspim->PinMOSI);

    // miso
    nrf_gpio_cfg_input(hspim->PinMISO, NRF_GPIO_PIN_NOPULL);
    //nrf_gpio_cfg_input(hspim->PinMISO, NRF_GPIO_PIN_PULLDOWN);
    //nrf_gpio_cfg_input(hspim->PinMISO, NRF_GPIO_PIN_PULLUP);

    hspim->Instance->PSELSCK   = hspim->PinSCK;
    hspim->Instance->PSELMOSI  = hspim->PinMOSI;
    hspim->Instance->PSELMISO  = hspim->PinMISO;
    hspim->Instance->FREQUENCY = hspim->Freguency;

    config = hspim->BitOrder << SPIM_CONFIG_ORDER_Pos;
    switch (hspim->Mode)
    {
        case NRF_SPIM_MODE_0:
        {
            config |= (SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos) |
                      (SPIM_CONFIG_CPHA_Leading    << SPIM_CONFIG_CPHA_Pos);
            break;
        }
        case NRF_SPIM_MODE_1:
        {
            config |= (SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos) |
                      (SPIM_CONFIG_CPHA_Trailing   << SPIM_CONFIG_CPHA_Pos);
            break;
        }
        case NRF_SPIM_MODE_2:
        {
            config |= (SPIM_CONFIG_CPOL_ActiveLow  << SPIM_CONFIG_CPOL_Pos) |
                      (SPIM_CONFIG_CPHA_Leading    << SPIM_CONFIG_CPHA_Pos);
            break;
        }
        case NRF_SPIM_MODE_3:
        {
            config |= (SPIM_CONFIG_CPOL_ActiveLow  << SPIM_CONFIG_CPOL_Pos) |
                      (SPIM_CONFIG_CPHA_Trailing   << SPIM_CONFIG_CPHA_Pos);
            break;
        }
    }
    hspim->Instance->CONFIG = config;
}

/**
 *  @brief  SPIM_Cmd
 */
void SPIM_Cmd( SPIM_InitTypeDef *hspim, uint32_t state )
{
    if (state != ENABLE)
    {
        hspim->Instance->ENABLE = SPIM_ENABLE_ENABLE_Disabled;
    }
    else
    {
        hspim->Instance->ENABLE = SPIM_ENABLE_ENABLE_Enabled;
    }
}

/**
 *  @brief  SPIM_InterruptCmd
 */
void SPIM_InterruptCmd( SPIM_InitTypeDef *hspim, uint32_t mode, uint32_t state )
{
    if (state != ENABLE)
    {
        hspim->Instance->INTENCLR = mode;
    }
    else
    {
        hspim->Instance->INTENSET = mode;
    }
}

/**
 *  @brief  SPIM_SetFrequency
 */
void SPIM_SetFrequency( SPIM_InitTypeDef *hspim, int frequency )
{
    nrf_gpio_pin_set(hspim->PinCS);
    SPIM_TASKS_STOP(hspim->Instance) = SET;
//    while (SPIM_EVENTS_STOPPED(hspim->Instance) != SET);
    SPIM_InterruptCmd(hspim, 0xFFFFFFFF, DISABLE);
    SPIM_Cmd(hspim, DISABLE);
    hspim->Freguency = frequency;
    SPIM_Cmd(hspim, ENABLE);
}

/**
 *  @brief  SPIM_Transceive
 */
uint32_t SPIM_Transceive( SPIM_InitTypeDef *hspim, const uint8_t *wbuf, uint32_t wlens, const uint8_t *rbuf, uint32_t rlens )
{
    SPIM_EVENTS_ENDTX(hspim->Instance) = RESET;
    SPIM_EVENTS_ENDRX(hspim->Instance) = RESET;
    hspim->Instance->TXD.PTR    = (uint32_t)wbuf;
    hspim->Instance->TXD.MAXCNT = wlens;
    hspim->Instance->RXD.PTR    = (uint32_t)rbuf;
    hspim->Instance->RXD.MAXCNT = rlens;
    SPIM_TASKS_START(hspim->Instance) = SET;
    while ((SPIM_EVENTS_ENDTX(hspim->Instance) != SET) || (SPIM_EVENTS_ENDRX(hspim->Instance) != SET) || (SPIM_EVENTS_END(hspim->Instance) != SET));
    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
