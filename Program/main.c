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
#include "modules\serial.h"
#include "modules\i2cmaster.h"
#include "kitsprout\kserial\clang\kSerial.h"
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

    bsp_timer_enable(ENABLE);
}

int main( void )
{
    bsp_init();
    i2c_config(TWI_SCL_PIN, TWI_SDA_PIN, 5);  // 400kHz

    while (1)
    {
        if (tick >= (TICK_FREQ / dv.updaterate))
        {
            tick = 0;
            LED_1_Toggle();

            if (dv.mode)
            {
                LED_2_Toggle();
                int16_t lsb[2] = {0};
                lsb[0] = sec;
                lsb[1] = msc * (1000.0f / TICK_FREQ);
                kSerial_SendPacket(NULL, lsb, 2, KS_I16);
            }
            else
            {
                LED_2_Off();
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

static uint8_t param[2] = {0};
static uint8_t rdata[KS_MAX_SEND_BUFFER_SIZE] = {0};
static uint32_t lens;
static uint32_t type;
void event_uart_serial_recv( void )
{
    uint8_t input = Serial_RecvByte();
#if 1
    if (kSerial_RecvPacket(input, param, rdata, &lens, &type) == KS_OK)
    {
        LED_4_Toggle();
        KSCommand(type, param, lens, rdata);
    }
#else
    Serial_SendByte(input);
#endif
}

void KSCommand( uint32_t type, uint8_t param[2], uint32_t lens, uint8_t *rdata )
{
    switch (type)
    {
        // system command
        // KSCMD_R0_DEVICE_ID           0xD0
        // KSCMD_R0_DEVICE_BAUDRATE     0xD1
        // KSCMD_R0_DEVICE_RATE         0xD2
        // KSCMD_R0_DEVICE_MDOE         0xD3
        // KSCMD_R0_DEVICE_GET          0xE3
        case KS_R0:
        {
            if ((param[0] == KSCMD_R0_DEVICE_ID) && (param[1] == 0x00))
            {
                kSerial_SendCommand(KS_R0, dv.id, dv.id >> 8, NULL);
            }
            else if ((param[0] == KSCMD_R0_DEVICE_BAUDRATE) && (param[1] == 4))
            {
                uint32_t baudrate = *(uint32_t*)rdata;
                uint32_t value = UART_Baudrate2RegValue(baudrate);
                if (value != 0)
                {
                    if (dv.baudrate != baudrate)
                    {
                        dv.baudrate = baudrate;
                        Serial_SetBaudrate(value);
                        klogd("  set baudrate %d bps\n", dv.baudrate);
                    }
                }
            }
            else if ((param[0] == KSCMD_R0_DEVICE_RATE) && (param[1] == 4))
            {
                dv.updaterate = *(uint32_t*)rdata;
                klogd("  set updaterate %d hz\n", dv.updaterate);
            }
            else if (param[0] == KSCMD_R0_DEVICE_MDOE)
            {
                dv.mode = param[1];
            }
            else if (param[0] == KSCMD_R0_DEVICE_GET)
            {
                switch (param[1])
                {
                    case KSCMD_R0_DEVICE_ID:        kSerial_SendPacket(param, &dv.id, 4, type);         break;
                    case KSCMD_R0_DEVICE_BAUDRATE:  kSerial_SendPacket(param, &dv.baudrate, 4, type);   break;
                    case KSCMD_R0_DEVICE_RATE:      kSerial_SendPacket(param, &dv.updaterate, 4, type); break;
                    case KSCMD_R0_DEVICE_MDOE:      kSerial_SendPacket(param, &dv.mode, 4, type);       break;
                }
            }
            break;
        }
        // i2c write/read command
        case KS_R1:
        {
            uint32_t address = param[0];
            uint32_t reg = param[1];
            if ((address & 0x01) && (lens == 1))  // read
            {
                uint32_t lenght = rdata[0];
                i2c_read(address >> 1, reg, lenght, rdata);
                kSerial_SendPacket(param, rdata, lenght, type);
            }
            else  // write
            {
                i2c_write(address >> 1, reg, lens, rdata);
            }
            break;
        }
        // i2c device command
        // KSCMD_R2_TWI_SCAN_DEVICE     0xA1
        // KSCMD_R2_TWI_SCAN_REGISTER   0xA2
        case KS_R2:
        {
            // scan device address
            if ((param[0] == KSCMD_R2_TWI_SCAN_DEVICE) && (param[1] == 0x00))
            {
                uint8_t address[128] = {0};
                uint8_t num;
                num = i2c_scan_device(address);
                kSerial_SendPacket(param, address, num, type);
            }
            // scan device reg
            else if (param[0] == KSCMD_R2_TWI_SCAN_REGISTER)
            {
                uint8_t address = param[1];
                uint8_t reg[256] = {0};
                i2c_scan_regs(address >> 1, reg);
                kSerial_SendPacket(param, reg, 256, KS_R2);
            }
            break;
        }
        case KS_R3:
        {
            break;
        }
        case KS_R4:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

/*************************************** END OF FILE ****************************************/
