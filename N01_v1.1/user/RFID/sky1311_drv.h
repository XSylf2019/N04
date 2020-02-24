#ifndef _SKY1311DRV_H_
#define _SKY1311DRV_H_
/******************************************************************************/
#include "stm8l15x.h"
#include "delay.h"
#include "gpio.h"
/******************************************************************************/
void sky1311WriteCmd(uint8_t cmd);
void sky1311WriteReg(uint8_t regAdd, uint8_t data);
uint8_t sky1311ReadReg(uint8_t regAdd);
void sky1311WriteFifo(uint8_t *data, uint8_t count);
void sky1311ReadFifo(uint8_t *data, uint8_t count);
void SKY1311_SYSCLK(void);
/******************************************************************************/
#endif
