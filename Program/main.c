/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    main.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "modules\encoder.h"
#include "modules\serial.h"
#include "modules\i2cmaster.h"
#include "modules\ist8308.h"
#include "kitsprout\kTerminal.h"
#include "nrf5x_bsp.h"

/* Define ----------------------------------------------------------------------------------*/

#define TICK_FREQ                   (1000)
#define DEFAULT_UPDATE_FREQ         (100)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static uint32_t sec = 0;
static uint32_t msc = 0;
static uint32_t tick = 0;

/* Prototypes ------------------------------------------------------------------------------*/

void event_timer_tick( void );
void event_uart_serial_recv( void );
void KSCommand( uint32_t type, uint8_t param[2], uint32_t lens, uint8_t *rdata );

/* Functions -------------------------------------------------------------------------------*/

void bsp_init( void )
{
    dv.updaterate = DEFAULT_UPDATE_FREQ;
    bsp_system_information(&dv);
    bsp_gpio_init();
    bsp_timer_init(event_timer_tick, TICK_FREQ);
    bsp_uart_serial_init(event_uart_serial_recv);
    bsp_sensor_init();
    bsp_uwb_init();
    klogd("\n");
    bsp_timer_enable(ENABLE);
}

int32_t rotaryCounter = 0;
void rotary_demo( void )
{
    int32_t rotary = Encoder_GetRotary(&rotaryCounter);
    int32_t button = RTEN_KEY_Read();
    if (button)
    {
        LED_4_On();
    }
    else
    {
        LED_4_Off();
    }
    if (rotary != 0)
    {
        klogd("Rotary Encoder, step %2d, push %d, step counter %d\n", rotary, button, rotaryCounter);
        printf("Rotary Encoder, step %2d, push %d, step counter %d\n", rotary, button, rotaryCounter);
    }
}

void sensor_log( void )
{
    float raw[10] = {0};
    bsp_sensor_get_raw(raw);
    klogd("[G] %7.3f %7.3f %7.3f [A] %7.3f %7.3f %7.3f [M] %7.2f %7.2f %7.2f [T] %.2f\n",
        raw[0], raw[1], raw[2],
        raw[3], raw[4], raw[5],
        raw[6], raw[7], raw[8],
        raw[9]);
}

int main( void )
{
    bsp_init();
    i2c_config(TWI_SCL_PORT, TWI_SCL_PIN, TWI_SDA_PORT, TWI_SDA_PIN, 5);    // 400kHz

    while (1)
    {
        if (tick >= (TICK_FREQ / dv.updaterate))
        {
            tick = 0;
            LED_1_Toggle();

            switch (dv.mode)
            {
                case 1:
                {
                    LED_2_Toggle();
                    int16_t lsb[12] = {0};
                    lsb[0] = sec;
                    lsb[1] = msc * (1000.0f / TICK_FREQ);
                    bsp_sensor_get_lsb(&lsb[2]);
                    kSerial_SendPacket(NULL, lsb, 12, KS_I16);
                }
                case 2:
                {
                    LED_2_Off();
                    rotary_demo();
                }
                default:
                {
                    LED_2_Off();
//                    sensor_log();
                }
            }



        }
    }
}

void event_timer_tick( void )
{
    tick++;
    if (++msc >= TICK_FREQ)
    {
        msc = 0;
        sec++;
    }
}

void event_uart_serial_recv( void )
{
    uint8_t input = Serial_RecvByte();
    kTerminal(input);
}

/*************************************** END OF FILE ****************************************/
