#include "uart.h"
#include "globe.h"
#include "string.h"
#include "rtc.h"
#include "gpio.h"
#include "save.h"
/******************************************************************************/
StruUSART1 UART1;
StruUSART2 UART2;
StruUSART3 UART3;
/******************************************************************************/
void UART1_Send_Byte(uint8_t ucData)
{
    USART_SendData8(USART1, ucData);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == 0);
}
/******************************************************************************/
void UART1_Send_String(uint8_t *Str)
{
    while (*Str) // "\0" meaning is the end of a string
    {
        UART1_Send_Byte(*Str);
        Str++;
    }
}
/******************************************************************************/
void UART1_Send_Bytes(uint8_t *DataBuff,uint8_t length)
{
    uint8_t i;
    
    for(i=0;i<length;i++)
    {
        UART1_Send_Byte(DataBuff[i]);
    }
}
/******************************************************************************/
void UART1_Init(uint32_t uiBaudRate)
{
    //INIT UART1 PINS
    GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);
    GPIO_SetBits(GPIOC, GPIO_Pin_3);

    //enable UART1 Clock
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);

    //setting the UART1
    USART_Init(USART1, uiBaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,
                       (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
    
    USART_ITConfig (USART1,USART_IT_RXNE,ENABLE);//使能接收中断,中断向量号为28
    
    //enable UART1
    USART_Cmd(USART1, ENABLE);
}
/**************************************************************/
void UART1_Send_ACK(uint8_t cmd)//应答
{
	uint8_t i;
	uint8_t txbuf[4] = {0};

	txbuf[0] = 0x7F;
	txbuf[1] = 0x01;
	txbuf[2] = cmd;
	for(i=0;i<3;i++)
	{
		txbuf[3] += txbuf[i];
	}
	UART1_Send_Bytes(txbuf,4);
}
/******************************************************************************/
uint8_t UART1_CheckReceivedata(void)//检验串口接收完成
{
    if(UART1.completeflag)
    {
        UART1.completeflag = 0;             
        return 1;
    }
    return 0;
}
/******************************************************************************/
void UART1_ReceiveData(void)//处理串口接收数据
{
    uint8_t Settimebuf[6];
    
    if(UART1_CheckReceivedata())
    {
        SleepDelayCount = 10;
        switch(UART1.buff[2])
        {
            case 0x10://清空密码
              ChangeDesignatedAddress(SAVE_Addr_FingerCounter,UART1.buff[3]);
              LED_ON();
              Delayms(1000);
              UART1_Send_ACK(0x10);//应答
              break;
              
            case 0x11://同步时间
              memcpy(Settimebuf,&UART1.buff[3],6);//复制时间
              Set_RTC_Time(Settimebuf);//设置时间
              LED_ON();
              Delayms(1000);
              UART1_Send_ACK(0x11);//应答
              break;
              
            case 0x12://开门
              Settimebuf[0] = getDesignatedAddress(SAVE_Addr_FingerCounter);
              LED_ON();
              Delayms(1000);
              UART1_Send_Byte(Settimebuf[0]);//应答
              Delayms(100);
              break;
              
            case 0x13://恢复出厂
              clearSaveAllUser();
//              LED_ON();
//              Delayms(1000);
              UART1_Send_ACK(0x13);//应答
              break;
        }
        memset(UART1.buff,0,sizeof(UART1.buff));
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
void UART2_Send_Byte(uint8_t ucData)
{
    USART_SendData8(USART2, ucData);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == 0);
}
/******************************************************************************/
void UART2_Send_String(uint8_t *Str)
{
    while (*Str) // "\0" meaning is the end of a string
    {
        UART2_Send_Byte(*Str);
        Str++;
    }
}
/******************************************************************************/
void UART2_Send_Bytes(uint8_t *DataBuff,uint8_t length)
{
    uint8_t i;
    
    for(i=0;i<length;i++)
    {
        UART2_Send_Byte(DataBuff[i]);
    }
}
/******************************************************************************/
void UART2_Init(uint32_t uiBaudRate)
{
    //INIT UART1 PINS
    GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);
    GPIO_SetBits(GPIOE, GPIO_Pin_4);

    //enable UART1 Clock
    CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE);

    //setting the UART1
    USART_Init(USART2, uiBaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,
                       (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
    
    USART_ITConfig (USART2,USART_IT_RXNE,ENABLE);//使能接收中断,中断向量号为20
    
    //enable UART1
    USART_Cmd(USART2, ENABLE);
}
/**************************************************************/
void UART2_Send_ACK(uint8_t cmd)//应答
{
	uint8_t i;
	uint8_t txbuf[4] = {0};

	txbuf[0] = 0x7F;
	txbuf[1] = 0x01;
	txbuf[2] = cmd;
	for(i=0;i<3;i++)
	{
		txbuf[3] += txbuf[i];
	}
	UART2_Send_Bytes(txbuf,4);
}
/******************************************************************************/
uint8_t UART2_CheckReceivedata(void)//检验串口接收完成
{
    if(UART2.completeflag)
    {
        UART2.completeflag = 0;             
        return 1;
    }
    return 0;
}
/******************************************************************************/
void UART2_ReceiveData(void)//处理串口接收数据
{
    uint8_t Settimebuf[6];
    
    if(UART2_CheckReceivedata())
    {
        SleepDelayCount = 10;
        switch(UART2.buff[2])
        {
            case 0x10://清空密码
              ChangeDesignatedAddress(SAVE_Addr_FingerCounter,UART2.buff[3]);
              LED_ON();
              Delayms(1000);
              UART2_Send_ACK(0x10);//应答
              break;
              
            case 0x11://同步时间
              memcpy(Settimebuf,&UART2.buff[3],6);//复制时间
              Set_RTC_Time(Settimebuf);//设置时间
              LED_ON();
              Delayms(1000);
              UART2_Send_ACK(0x11);//应答
              break;
              
            case 0x12://开门
              Settimebuf[0] = getDesignatedAddress(SAVE_Addr_FingerCounter);
              LED_ON();
              Delayms(1000);
              UART2_Send_Byte(Settimebuf[0]);//应答
              Delayms(100);
              break;
              
            case 0x13://恢复出厂
              clearSaveAllUser();
//              LED_ON();
//              Delayms(1000);
              UART2_Send_ACK(0x13);//应答
              break;
        }
        memset(UART2.buff,0,sizeof(UART2.buff));
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
void UART3_Send_Byte(uint8_t ucData)
{
    USART_SendData8(USART3, ucData);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == 0);
}
/******************************************************************************/
void UART3_Send_String(uint8_t *Str)
{
    while (*Str) // "\0" meaning is the end of a string
    {
        UART3_Send_Byte(*Str);
        Str++;
    }
}
/******************************************************************************/
void UART3_Send_Bytes(uint8_t *DataBuff,uint8_t length)
{
    uint8_t i;
    
    for(i=0;i<length;i++)
    {
        UART3_Send_Byte(DataBuff[i]);
    }
}
/******************************************************************************/
void UART3_Init(uint32_t uiBaudRate)
{
    //INIT UART1 PINS
    GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(GPIOG, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT);
    GPIO_SetBits(GPIOG, GPIO_Pin_1);

    //enable UART1 Clock
    CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);

    //setting the UART1
    USART_Init(USART3, uiBaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,
                       (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
    
    USART_ITConfig (USART3,USART_IT_RXNE,ENABLE);//使能接收中断,中断向量号为22
    
    //enable UART1
    USART_Cmd(USART3, ENABLE);
}
/**************************************************************/
void UART3_Send_ACK(uint8_t cmd)//应答
{
	uint8_t i;
	uint8_t txbuf[4] = {0};

	txbuf[0] = 0x7F;
	txbuf[1] = 0x01;
	txbuf[2] = cmd;
	for(i=0;i<3;i++)
	{
		txbuf[3] += txbuf[i];
	}
	UART3_Send_Bytes(txbuf,4);
}
/******************************************************************************/
uint8_t UART3_CheckReceivedata(void)//检验串口接收完成
{
    if(UART3.completeflag)
    {
        UART3.completeflag = 0;             
        return 1;
    }
    return 0;
}
/******************************************************************************/
void UART3_ReceiveData(void)//处理串口接收数据
{
    uint8_t Settimebuf[6];
    
    if(UART3_CheckReceivedata())
    {
        SleepDelayCount = 10;
        switch(UART3.buff[2])
        {
            case 0x10://清空密码
              ChangeDesignatedAddress(SAVE_Addr_FingerCounter,UART2.buff[3]);
              LED_ON();
              Delayms(1000);
              UART3_Send_ACK(0x10);//应答
              break;
              
            case 0x11://同步时间
              memcpy(Settimebuf,&UART3.buff[3],6);//复制时间
              Set_RTC_Time(Settimebuf);//设置时间
              LED_ON();
              Delayms(1000);
              UART3_Send_ACK(0x11);//应答
              break;
              
            case 0x12://开门
              Settimebuf[0] = getDesignatedAddress(SAVE_Addr_FingerCounter);
              LED_ON();
              Delayms(1000);
              UART3_Send_Byte(Settimebuf[0]);//应答
              Delayms(100);
              break;
              
            case 0x13://恢复出厂
              clearSaveAllUser();
//              LED_ON();
//              Delayms(1000);
              UART3_Send_ACK(0x13);//应答
              break;
        }
        memset(UART3.buff,0,sizeof(UART3.buff));
    }
}
/******************************************************************************/