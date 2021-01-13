/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_system.h
 *  @author  KitSprout
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5X_SYSTEM_H
#define __NRF5X_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf5x_delay.h"

#include "kitsprout\kLogger.h"
#include "kitsprout\kserial\kStatus.h"

#include "boardConfig.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/

#define KS_SYSCLK                                       SystemCoreClock
#define KS_UUID64()                                     ((uint64_t)(((uint64_t)NRF_FICR->DEVICEID[1] << 32) |  NRF_FICR->DEVICEID[0]))
#define KS_UUID32()                                     ((uint32_t)NRF_FICR->DEVICEID[0])
#define KS_UUID16()                                     ((uint16_t)NRF_FICR->DEVICEID[0])

#define __GPIO_SET(__PORT, __PIN)                       ((NRFX_PORT[__PORT])->OUTSET = 1UL << (__PIN))
#define __GPIO_RST(__PORT, __PIN)                       ((NRFX_PORT[__PORT])->OUTCLR = 1UL << (__PIN))
#define __GPIO_TOG(__PORT, __PIN)                       ((NRFX_PORT[__PORT])->OUT ^= 1UL << (__PIN))
#define __GPIO_READ(__PORT, __PIN)                      (((NRFX_PORT[__PORT])->IN >> (__PIN)) & 1UL)

#define BYTE32(__BH2, __BH1, __BL2, __BL1)              (((uint8_t)(__BH2)<<24U) | ((uint8_t)(__BH1)<<16U) | ((uint8_t)(__BL2)<<8U)  | ((uint8_t)(__BL1)))
#define BYTE32U16H(__B32U16)                            ((uint16_t)(((uint32_t)(__B32U16)&0xFFFF0000)>>16U))
#define BYTE32U16L(__B32U16)                            ((uint16_t)( (uint32_t)(__B32U16)&0x0000FFFF))
#define BYTE32U8_4(__B32U8)                             ((uint8_t)(((uint32_t)(__B32U8)&0xFF000000)>>24U))
#define BYTE32U8_3(__B32U8)                             ((uint8_t)(((uint32_t)(__B32U8)&0x00FF0000)>>16U))
#define BYTE32U8_2(__B32U8)                             ((uint8_t)(((uint32_t)(__B32U8)&0x0000FF00)>>8U))
#define BYTE32U8_1(__B32U8)                             ((uint8_t)( (uint32_t)(__B32U8)&0x000000FF))

#define BYTE16(__BH, __BL)                              ((((uint16_t)(__BH))<<8U) | ((uint16_t)(__BL)))
#define BYTE16U8H(__B16U8)                              ((uint8_t)(((uint16_t)(__B16U8)&0xFF00)>>8U))
#define BYTE16U8L(__B16U8)                              ((uint8_t)( (uint16_t)(__B16U8)&0x00FF))

/* Typedef ---------------------------------------------------------------------------------*/

typedef void (*pFunc)( void );

typedef enum
{
    RESET = 0U, 
    SET = !RESET

} FlagStatus;

typedef enum
{
    DISABLE = 0U, 
    ENABLE = !DISABLE

} FunctionalState;

typedef struct
{
    uint32_t id;
    uint32_t mode;
    uint32_t baudrate;
    uint32_t updaterate;

} device_info_t;

/* Extern ----------------------------------------------------------------------------------*/

extern NRF_GPIO_Type* const NRFX_PORT[2];
extern device_info_t dv;

/* Functions -------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
