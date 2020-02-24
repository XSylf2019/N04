#ifndef __USART_H_
#define __USART_H_
/******************************************************************************/
#include "stm8l15x.h"
/******************************************************************************/
void UART1_Init(uint32_t uiBaudRate);
void UART1_Send_Byte(uint8_t ucData);
void UART1_Send_String(uint8_t *Str);
void UART1_Send_Bytes(uint8_t *DataBuff,uint8_t length);
void UART1_ReceiveData(void);//处理串口接收数据
////////////////////////////////////////////////////////////////////////////////
void UART2_Init(uint32_t uiBaudRate);
void UART2_Send_Byte(uint8_t ucData);
void UART2_Send_String(uint8_t *Str);
void UART2_Send_Bytes(uint8_t *DataBuff,uint8_t length);
void UART2_ReceiveData(void);//处理串口接收数据
////////////////////////////////////////////////////////////////////////////////
void UART3_Init(uint32_t uiBaudRate);
void UART3_Send_Byte(uint8_t ucData);
void UART3_Send_String(uint8_t *Str);
void UART3_Send_Bytes(uint8_t *DataBuff,uint8_t length);
void UART3_ReceiveData(void);//处理串口接收数据
/******************************************************************************/
#endif 
