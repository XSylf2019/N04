#ifndef __CARDOPER_H
#define __CARDOPER_H

#include "typedef.h"

extern uint8_t block_length;
extern uint8_t mifare_block_buff[256];

uint8_t mifare1_readDemo(void);
uint8_t SmartTypeA_readDemo(void);

sta_result_t TypeA_test(uint8_t *TypeA_data, uint8_t *TypeA_uid_len);
sta_result_t TypeB_test(uint8_t *TypeB_data, uint8_t *TypeB_uid_len);
#endif
