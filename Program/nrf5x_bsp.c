/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_bsp.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_clock.h"
#include "drivers\nrf5x_timer.h"
#include "modules\serial.h"
#include "nrf5x_bsp.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

TIMER_BaseInitTypeDef htimer;
TIMER_ChannelInitTypeDef htimer_cc;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void bsp_system_information( device_info_t *ds )
{
    klogd("Board Name: %s\n", KS_HW_BOARD_NAME);
    klogd("Controller: %s\n", KS_HW_MCU_NAME);
    ds->id = KS_UUID32();
    ds->baudrate = UART_RegValue2Baudrate(SERIAL_BAUDRATE);
    klogd("\n");
    klogd("UUID: %04X%04X\n", (uint32_t)(KS_UUID64() >> 32), KS_UUID32());
    klogd("Mode: %d\n", ds->mode);
    klogd("Baudrate: %d bps\n", ds->baudrate);
    klogd("updaterate: %d Hz\n", ds->updaterate);
}

void bsp_gpio_init( void )
{
    nrf5x_clock_config();

    LED_1_Off();
    LED_2_Off();
    LED_3_Off();
    LED_4_Off();
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED_1_PORT, LED_1_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED_2_PORT, LED_2_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED_3_PORT, LED_3_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED_4_PORT, LED_4_PIN));
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(KEY_1_PORT, KEY_1_PIN), NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(KEY_2_PORT, KEY_2_PIN), NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(KEY_3_PORT, KEY_3_PIN), NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(KEY_4_PORT, KEY_4_PIN), NRF_GPIO_PIN_PULLUP);
}

void bsp_timer_init( pFunc event, uint32_t freq )
{
    htimer.Instance  = TIMERx;
    htimer.Mode      = TIMERx_MODE;
    htimer.BitMode   = TIMERx_BIT_MODE;
    htimer.Prescaler = TIMERx_PRESCALER;
    TIMER_BaseInit(&htimer);

    htimer_cc.Channel = TIMERx_CHANNEL;
    htimer_cc.Period  = (uint32_t)((16000000U >> NRF_TIMER_FREQ_1MHz) / freq);
    htimer_cc.EventCallback = event;
    TIMER_ChannelInit(&htimer, &htimer_cc);
    TIMER_InterruptCmd(&htimer, htimer_cc.Channel, ENABLE);

    NVIC_SetPriority(TIMERx_IRQn, TIMERx_IRQn_PRIORITY);
    NVIC_EnableIRQ(TIMERx_IRQn);
}

void bsp_timer_enable( uint32_t state )
{
    TIMER_Cmd(&htimer, state);
}

void bsp_uart_serial_init( pFunc event )
{
    hserial.RxEventCallback = event;

    Serial_Config();
    Serial_Cmd(ENABLE);
    printf("\r\n\f\r\n");
}

/*************************************** END OF FILE ****************************************/
