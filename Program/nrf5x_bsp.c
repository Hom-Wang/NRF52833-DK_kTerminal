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
#include "modules\encoder.h"
#include "modules\serial.h"
#include "modules\icm42688.h"
#include "modules\ist8308.h"
#include "nrf5x_bsp.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

TIMER_BaseInitTypeDef htimer;
TIMER_ChannelInitTypeDef htimer_cc;

static float gyrSensitivity = 0, accSensitivity = 0, magSensitivity = 0;

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
    klogd("Updaterate: %d Hz\n", ds->updaterate);
    klogd("\n");
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

    Encoder_Config();
    Encoder_Cmd(ENABLE);
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

void bsp_sensor_init( void )
{
    uint32_t status;
    uint32_t error = 0;

    klogd("bsp sensor init ...\n");

    // icm42688 init
    ICM42688_Config();
    delay_ms(50);
    status = ICM42688_Init();
    if (status != KS_OK)
    {
        error++;
    }

    // ist8308 init
    IST8308_Config();
    delay_ms(50);
    status = IST8308_Init();
    if (status != KS_OK)
    {
        error++;
    }

    // check init status
    klogd("\n");
    if (error != KS_OK)
    {
#if 1
        LED_3_On();
        return;
#else
        while (1)
        {
            LED_3_Toggle();
            delay_ms(100);
        }
#endif
    }
# if 0
    gyrSensitivity = ICM42688_GetGyrSensitivity();
    accSensitivity = ICM42688_GetAccSensitivity();
    magSensitivity = IST8308_GetMagSensitivity();
    klogd("sensor resulation %.6f dps, %.6f mg, %.6f uT\n",
        gyrSensitivity, accSensitivity, magSensitivity);
#else
    gyrSensitivity = ICM42688_GetGyrSensitivity() * 3.14159 / 180;  // rad/s
    accSensitivity = ICM42688_GetAccSensitivity() * 0.00981;        // m/s^2
    magSensitivity = IST8308_GetMagSensitivity();                   // uT
    klogd("  sensor resulation %.6f rad/s, %.6f m/s^2, %.6f uT\n",
        gyrSensitivity, accSensitivity, magSensitivity);
#endif
    klogd("\n");
}

// gyr, acc, mag, temp
void bsp_sensor_get_lsb( int16_t *lsb )
{
    int16_t res[10];
    ICM42688_GetLsbData(&res[0]);   // temp[0], acc[1:3], gyr[4:6]
    IST8308_GetLsbData(&res[7]);    // mag[7:9]
    lsb[0] = res[4];    // gyr.x
    lsb[1] = res[5];    // gyr.y
    lsb[2] = res[6];    // gyr.z
    lsb[3] = res[1];    // acc.x
    lsb[4] = res[2];    // acc.y
    lsb[5] = res[3];    // acc.z
    lsb[6] = res[7];    // mag.x
    lsb[7] = res[8];    // mag.y
    lsb[8] = res[9];    // mag.z
    lsb[9] = res[0];    // temp
}

// gyr, acc, mag, temp
void bsp_sensor_get_raw( float *raw )
{
    int16_t lsb[10];
    bsp_sensor_get_lsb(lsb);
    raw[0] = lsb[0] * gyrSensitivity;   // gyr.x
    raw[1] = lsb[1] * gyrSensitivity;   // gyr.y
    raw[2] = lsb[2] * gyrSensitivity;   // gyr.z
    raw[3] = lsb[3] * accSensitivity;   // acc.x
    raw[4] = lsb[4] * accSensitivity;   // acc.y
    raw[5] = lsb[5] * accSensitivity;   // acc.z
    raw[6] = lsb[6] * magSensitivity;   // mag.x
    raw[7] = lsb[7] * magSensitivity;   // mag.y
    raw[8] = lsb[8] * magSensitivity;   // mag.z
    raw[9] = lsb[9] / 132.48f + 25;     // temp
}

/*************************************** END OF FILE ****************************************/
