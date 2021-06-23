/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kTerminal.c
 *  @author  KitSprout
 *  @brief   
 */

/* Includes --------------------------------------------------------------------------------*/
#include "kserial\kSerial.h"
#include "kTerminal.h"
#include "kLogger.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

static uint8_t param[2] = {0};
static uint8_t rdata[KS_MAX_SEND_BUFFER_SIZE] = {0};
static uint32_t lens;
static uint32_t type;

/* Prototypes ------------------------------------------------------------------------------*/

extern void     i2c_read( uint8_t address, uint8_t reg, uint8_t length, uint8_t *data );
extern void     i2c_write( uint8_t address, uint8_t reg, uint8_t length, uint8_t *data );
extern uint32_t i2c_scan_device( uint8_t *address );
extern uint32_t i2c_scan_regs( const uint8_t address, uint8_t *reg );

/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  KSCommand
 */
uint32_t KSCommand( uint32_t type, uint8_t param[2], uint32_t lens, uint8_t *rdata )
{
    uint32_t status = KS_OK;
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
                klogd("  KSCMD_R0_DEVICE_ID\n");
                klogd("  device id %04X, %d\n", dv.id & 0x0000FFFF, dv.id);
                kSerial_SendCommand(KS_R0, dv.id, dv.id >> 8, NULL);
            }
            else if ((param[0] == KSCMD_R0_DEVICE_BAUDRATE) && (param[1] == 4))
            {
                uint32_t baudrate = *(uint32_t*)rdata;
                uint32_t value = UART_Baudrate2RegValue(baudrate);
                if (value != 0)
                {
                    klogd("  KSCMD_R0_DEVICE_BAUDRATE\n");
                    klogd("  set baudrate %d bps\n", dv.baudrate);
                    if (dv.baudrate != baudrate)
                    {
                        dv.baudrate = baudrate;
                        Serial_SetBaudrate(value);
                    }
                }
            }
            else if ((param[0] == KSCMD_R0_DEVICE_RATE) && (param[1] == 4))
            {
                dv.updaterate = *(uint32_t*)rdata;
                klogd("  KSCMD_R0_DEVICE_RATE\n");
                klogd("  set updaterate %d hz\n", dv.updaterate);
            }
            else if (param[0] == KSCMD_R0_DEVICE_MDOE)
            {
                dv.mode = param[1];
                klogd("  KSCMD_R0_DEVICE_MDOE\n");
                klogd("  set device mode %d\n", dv.mode);
            }
            else if (param[0] == KSCMD_R0_DEVICE_GET)
            {
                klogd("  KSCMD_R0_DEVICE_GET ... %d\n", param[1]);
                switch (param[1])
                {
                    case KSCMD_R0_DEVICE_ID:        kSerial_SendPacket(param, &dv.id, 4, type);         break;
                    case KSCMD_R0_DEVICE_BAUDRATE:  kSerial_SendPacket(param, &dv.baudrate, 4, type);   break;
                    case KSCMD_R0_DEVICE_RATE:      kSerial_SendPacket(param, &dv.updaterate, 4, type); break;
                    case KSCMD_R0_DEVICE_MDOE:      kSerial_SendPacket(param, &dv.mode, 4, type);       break;
                    default:                        status = KS_ERROR;
                }
            }
            else
            {
                status = KS_ERROR;
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
                klogd("  KSCMD_R2_TWI_SCAN_DEVICE\n");
                uint8_t address[128] = {0};
                uint8_t num;
                num = i2c_scan_device(address);
                kSerial_SendPacket(param, address, num, type);
            }
            // scan device reg
            else if (param[0] == KSCMD_R2_TWI_SCAN_REGISTER)
            {
                klogd("  KSCMD_R2_TWI_SCAN_REGISTER ... %d\n", param[1]);
                uint8_t address = param[1];
                uint8_t reg[256] = {0};
                i2c_scan_regs(address >> 1, reg);
                kSerial_SendPacket(param, reg, 256, KS_R2);
            }
            else
            {
                status = KS_ERROR;
            }
            break;
        }
        case KS_R3:
        {
            status = KS_ERROR;
            break;
        }
        case KS_R4:
        {
            status = KS_ERROR;
            break;
        }
        default:
        {
            status = KS_ERROR;
            break;
        }
    }
    return status;
}

/**
 *  @brief  kTerminal
 */
uint32_t kTerminal( uint32_t byte )
{
    if (kSerial_RecvPacket(byte, param, rdata, &lens, &type) == KS_OK)
    {
        KSCommand(type, param, lens, rdata);
        return KS_OK;
    }
    return KS_ERROR;
}

/*************************************** END OF FILE ****************************************/
