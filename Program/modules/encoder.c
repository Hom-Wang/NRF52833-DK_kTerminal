/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    rotary_encoder.c
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "modules\encoder.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/

#define GET_ENCODER_STATE()     (RTEN_A_Read() << 1 | RTEN_B_Read())

/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static const uint8_t decode_table[4][4] =
{
    {0, 1, 3, 2},
    {3, 0, 2, 1},
    {1, 2, 0, 3},
    {2, 3, 1, 0},
};

static uint32_t lastEncoderState = 0;
static uint32_t lastState = 0;
__IO int32_t rotaryStep = 0;

GPIOTE_InitTypeDef hencoderA;
GPIOTE_InitTypeDef hencoderB;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  encoder
 */
void Encoder_Config( void )
{
    // rotary encoder
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(RTEN_KEY_PORT, RTEN_KEY_PIN), NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(RTEN_A_PORT, RTEN_A_PIN), NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(RTEN_B_PORT, RTEN_B_PIN), NRF_GPIO_PIN_PULLUP);
    lastEncoderState = GET_ENCODER_STATE();

    // interrupt
    hencoderA.Instance   = RTEN_GPIOTEx;
    hencoderA.Line       = RTEN_A_GPIOTEx_LINE;
    hencoderA.Pin        = NRF_GPIO_PIN_MAP(RTEN_A_PORT, RTEN_A_PIN);
    hencoderA.Mode       = RTEN_GPIOTEx_MODE;
    hencoderA.Polarity   = RTEN_GPIOTEx_POLARITY;
    hencoderA.OutInit    = RTEN_GPIOTEx_OUTINIT;
    GPIOTE_Init(&hencoderA);

    hencoderB.Instance   = RTEN_GPIOTEx;
    hencoderB.Line       = RTEN_B_GPIOTEx_LINE;
    hencoderB.Pin        = NRF_GPIO_PIN_MAP(RTEN_B_PORT, RTEN_B_PIN);
    hencoderB.Mode       = RTEN_GPIOTEx_MODE;
    hencoderB.Polarity   = RTEN_GPIOTEx_POLARITY;
    hencoderB.OutInit    = RTEN_GPIOTEx_OUTINIT;
    GPIOTE_Init(&hencoderB);

    hencoderA.EventCallback = Encoder_Event;
    hencoderB.EventCallback = Encoder_Event;

    NVIC_SetPriority(RTEN_GPIOTEx_IRQn, RTEN_GPIOTEx_IRQn_PRIORITY);
    NVIC_EnableIRQ(RTEN_GPIOTEx_IRQn);
}

/**
 *  @brief  Encoder_Cmd
 */
void Encoder_Cmd( uint32_t state )
{
    GPIOTE_InterruptCmd(&hencoderA, state);
    GPIOTE_InterruptCmd(&hencoderB, state);
}

/**
 *  @brief  Encoder_GetState
 */
uint32_t Encoder_GetState( void )
{
    uint32_t encoderState = GET_ENCODER_STATE();
    uint32_t state = decode_table[encoderState][lastEncoderState];
    lastEncoderState = encoderState;
    return state;
}

/**
 *  @brief  Encoder_GetRotary
 */
int32_t Encoder_GetRotary( int32_t *counter )
{
    int32_t rotary = rotaryStep;
    if (rotary != 0)
    {
        rotaryStep = 0;
        if (counter != NULL)
        {
            *counter += rotary;
        }
    }
    return rotary;
}

/**
 *  @brief  Encoder_Event
 */
void Encoder_Event( void )
{
    uint32_t state = Encoder_GetState();
    int32_t delta = state - lastState;
    if (delta > 2)
    {
        delta -= 4;
    }
    else if (delta < -2)
    {
        delta += 4;
    }
    rotaryStep = delta;
}

/*************************************** END OF FILE ****************************************/
