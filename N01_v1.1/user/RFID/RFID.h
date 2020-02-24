#ifndef _RFID_H_
#define _RFID_H_
/******************************************************************************/
#include "stm8l15x.h"
/******************************************************************************/
uint8_t sky1311_init(void);
uint8_t sky1311sleep_init(void);
uint8_t SKY1311_IRQ_Callback(uint8_t *uid_data, uint8_t *uid_len);
void sky1311_checkCardInit(void);
/******************************************************************************/
#endif
