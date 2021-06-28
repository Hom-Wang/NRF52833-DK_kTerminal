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

#ifndef _DECA_PORT_H_
#define _DECA_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "drivers\nrf5x_system.h"
#include "deca_device_api.h"


#define reset_DW1000                                    dw1000_reset
#define setup_DW1000RSTnIRQ                             dw1000_setirq

#ifndef UWB_SPIMx
#define DW1000_SPIMx                                    NRF_SPIM0
#define DW1000_SPIMx_IRQn                               UWB_SPIMx_IRQn
#define DW1000_SPIMx_IRQHANDLER                         UWB_SPIMx_IRQHANDLER
#define DW1000_SPIMx_IRQn_PRIORITY                      UWB_SPIMx_IRQn_PRIORITY

#define DW1000_SPIMx_BITORDER                           UWB_SPIMx_BITORDER
#define DW1000_SPIMx_MODE                               UWB_SPIMx_MODE
#define DW1000_SPIMx_SLOW_RATE                          UWB_SPIMx_SLOW_RATE
#define DW1000_SPIMx_FAST_RATE                          UWB_SPIMx_FAST_RATE

#define DW1000_CSN_PIN                                  UWB_CSN_PIN
#define DW1000_CSN_PORT                                 UWB_CSN_PORT
#define DW1000_CSN_Set()                                __GPIO_SET(DW1000_CSN_PORT, DW1000_CSN_PIN)
#define DW1000_CSN_Reset()                              __GPIO_RST(DW1000_CSN_PORT, DW1000_CSN_PIN)

#define DW1000_SCK_PIN                                  UWB_SCK_PIN
#define DW1000_SCK_PORT                                 UWB_SCK_PORT

#define DW1000_SDI_PIN                                  UWB_SDI_PIN
#define DW1000_SDI_PORT                                 UWB_SDI_PORT

#define DW1000_SDO_PIN                                  UWB_SDO_PIN
#define DW1000_SDO_PORT                                 UWB_SDO_PORT

#define DW1000_RST_PIN                                  UWB_RST_PIN
#define DW1000_RST_PORT                                 UWB_RST_PORT
#define DW1000_RST_Set()                                __GPIO_SET(DW1000_RST_PORT, DW1000_RST_PIN)
#define DW1000_RST_Reset()                              __GPIO_RST(DW1000_RST_PORT, DW1000_RST_PIN)

#define DW1000_IRQ_PIN                                  UWB_IRQ_PIN
#define DW1000_IRQ_PORT                                 UWB_IRQ_PORT
#define DW1000_IRQ_Read()                               (__GPIO_READ(DW1000_IRQ_PORT, DW1000_IRQ_PIN) == RESET)

#define DW1000_WAK_PIN                                  UWB_WAK_PIN
#define DW1000_WAK_PORT                                 UWB_WAK_PORT

#define DW1000_EXT_PIN                                  UWB_EXT_PIN
#define DW1000_EXT_PORT                                 UWB_EXT_PORT
#endif


void        dw1000_setup(void);
uint32_t    dw1000_init(dwt_config_t *config);

void        dw1000_reset(void);
void        dw1000_setirq(int enable);

void        dw1000_spi_slowrate(void);
void        dw1000_spi_fastrate(void);
int         dw1000_spi_open(void);
int         dw1000_spi_close(void);

#endif
