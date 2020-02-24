#include "Key.h"
#include "delay.h"
#include "gpio.h"
#include "globe.h"
/******************************************************************************/
KeyPads Key;

uint8_t guPressContiueFlag;
/******************************************************************************/
void Key_io_init(void)
{   
    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);                                      
    GPIO_Init(GPIOE, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOG, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    
    GPIO_Write(GPIOA,0x00);
    GPIO_Write(GPIOB,0x00);
    GPIO_Write(GPIOC,0x00);
    GPIO_Write(GPIOD,0x00);
    GPIO_Write(GPIOE,0x00);
    GPIO_Write(GPIOF,0x00);
    GPIO_Write(GPIOG,0x00);
    
    //scl
    GPIO_Init(TSM12_SCL_PORT,TSM12_SCL_PIN,GPIO_Mode_Out_PP_High_Slow);         //输出上拉 2MHz
    GPIO_SetBits(TSM12_SCL_PORT, TSM12_SCL_PIN);                                //输出高电平
              
    //sda
    GPIO_Init(TSM12_SDA_PORT,TSM12_SDA_PIN,GPIO_Mode_Out_PP_High_Slow);         //输出上拉 2MHz
    GPIO_SetBits(TSM12_SDA_PORT, TSM12_SDA_PIN);                                //输出高电平
      
    //IIC EN
    GPIO_Init(TSM12_I2CE_PORT,TSM12_I2CE_PIN,GPIO_Mode_Out_PP_High_Slow);       //输出上拉 2MHz
    GPIO_SetBits(TSM12_I2CE_PORT, TSM12_I2CE_PIN);                              //输出高电平

    //IIC IRQ
    EXTI_ClearITPendingBit(EXTI_IT_Pin1);
    GPIO_Init(TSM12_IRQ_PORT, TSM12_IRQ_PIN, GPIO_Mode_In_PU_IT);               //输入上拉,有中断
    EXTI_SelectPort(EXTI_Port_F);
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);                   //仅在下降沿中断
      
    //Aey
    EXTI_ClearITPendingBit(EXTI_IT_Pin1);
    GPIO_Init(RESET_PORT, RESET_PIN, GPIO_Mode_In_PU_IT);                       //输入上拉,有中断
    EXTI_SelectPort(EXTI_Port_F);
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);                   //仅在下降沿中断
    
    //MOTO
    GPIO_Init(MOTOR_PORT,MOTOR_PIN,GPIO_Mode_Out_PP_Low_Slow);                  //输出下拉 2MHz
    GPIO_ResetBits(MOTOR_PORT, MOTOR_PIN);                                      //输出低电平
      
    GPIO_Init(MOTOL_PORT,MOTOL_PIN,GPIO_Mode_Out_PP_Low_Slow);                  //输出下拉 2MHz
    GPIO_ResetBits(MOTOL_PORT, MOTOL_PIN);                                      //输出低电平
      
    //LED
    GPIO_Init(LED_PORT,LED_PIN,GPIO_Mode_Out_PP_High_Slow);                     //输出上拉 2MHz
    GPIO_ResetBits(LED_PORT, LED_PIN);                                            //输出高电平
      
    //SPEAK
    GPIO_Init(SPEAK_PORT,SPEAK_PIN,GPIO_Mode_Out_PP_Low_Slow);                  //输出下拉 2MHz
    GPIO_ResetBits(SPEAK_PORT, SPEAK_PIN);                                      //输出低电平
      
    //FINGER PWR
    GPIO_Init(FINGER_PWR_PORT,FINGER_PWR_PIN,GPIO_Mode_Out_PP_Low_Slow);        //输出下拉 2MHz
    GPIO_ResetBits(FINGER_PWR_PORT, FINGER_PWR_PIN);                            //输出低电平
      
//    //FINGER IRQ
//    EXTI_ClearITPendingBit(EXTI_IT_Pin4);
//    GPIO_Init(FINGER_IRQ_PORT, FINGER_IRQ_PIN, GPIO_Mode_In_PU_IT);             //输入上拉,有中断
//    EXTI_SelectPort(EXTI_Port_B);
//    EXTI_SetPinSensitivity(EXTI_Pin_4, EXTI_Trigger_Rising);                    //仅在上升沿中断
}
/******************************************************************************/
void Key_Resetbutton(void)
{
    uint8_t p = 0;
    
    if(!Read_Akey())
    {
        while(1)
        {
            p++;
            Delayms(1); //防抖
            if(Read_Akey())return;
            if(p>100)
            {
                break;
            }
        }
    }
    if(!Read_Akey())
    {
        if(guPressContiueFlag)return;
        guPressContiueFlag = 1;
        LED_Toggle();
    }
    else guPressContiueFlag = 0;
}
/******************************************************************************/
void SoundBeep(uint8_t type)//蜂鸣器声音
{   
    uint8_t i;
    switch(type)
    {
        case BEEP_POINT:
            for(i = 0;i < 1;i ++)
            {
                SPEAK_ON();
                delay_ms(30);
                SPEAK_OFF();
            }
            break;            
        case BEEP_OK:
            for(i = 0;i < 1;i ++)
            {
                SPEAK_ON();
                delay_ms(300);
                SPEAK_OFF();
            }
            break;    
        case BEEP_DEFAIT:  
            for(i = 0;i < 3;i ++)
            {         
                LED_OFF();
                SPEAK_ON();
                delay_ms(30);
                SPEAK_OFF();   
                LED_ON();
                delay_ms(50);
            }       
            break;  
        case BEEP_ALARM:
            for(i = 0;i < 5;i ++)
            {
                SPEAK_ON();
                delay_ms(30);
                SPEAK_OFF();
                delay_ms(50);
            }
            break;
        case BEEP_set:
            for(i = 0;i < 3;i ++)
            {
                SPEAK_ON();
                delay_ms(30);
                SPEAK_OFF();
                delay_ms(30);
            }
            break;
        case BEEP_clear:
            for(i = 0;i < 3;i ++)
            {
                SPEAK_ON();
                delay_ms(30);
                SPEAK_OFF();
                delay_ms(30);
            }
            break; 
    }
}
/**************************************************************/
void IIC_delay(unsigned int delaytime)
{
    unsigned  int i;
    while(delaytime--)
    {
        for(i = 0;i < 25;i ++)asm("nop");  //60
    }
}
/**************************************************************/
void I2C_Start(void)
{       
    TSM_SDA_OUT();
    TSM_SCL_H;
    TSM_SDA_H;
    IIC_delay(3);
    TSM_SDA_L;
    IIC_delay(10);
    TSM_SCL_L;
    IIC_delay(10);
}
/**************************************************************/
void I2C_Stop(void)
{
    TSM_SDA_OUT();
    TSM_SCL_L;
    TSM_SDA_L; 
    IIC_delay(3);
    TSM_SCL_H;
    IIC_delay(3);
    TSM_SDA_H; 
    IIC_delay(20);
}
/**************************************************************/
void I2C_SendAck(uint8_t bAck)
{
    TSM_SDA_OUT();
    if(bAck) TSM_SDA_H;
    else TSM_SDA_L;
    IIC_delay(2);
    TSM_SCL_H;
    IIC_delay(2);
    TSM_SCL_L;
    IIC_delay(2);
}
/**************************************************************/
uint8_t I2C_WaitAck(void)
{  
    TSM_SDA_IN();
    TSM_SCL_H;
    uint8_t i;
    for(i = 0;i < 6;i ++)
    {
        IIC_delay(2);
        if(!TSM_SDA_READ())
        {
            TSM_SCL_L;
            IIC_delay(2);
            return 0;
        }          
    }
    TSM_SCL_L; 
    IIC_delay(2);
    return 1; 
}
/**************************************************************/
void I2C_WriteByte(uint8_t dat) 
{
    uint8_t i;
    TSM_SDA_OUT(); 
    IIC_delay(2);
    for(i=0; i<8; i++ )
    {
        if(dat & 0x80)TSM_SDA_H;
        else TSM_SDA_L;
        IIC_delay(1);
        TSM_SCL_H;
        IIC_delay(1);
        TSM_SCL_L;
        IIC_delay(1);
        dat <<= 1;
    }
}
/**************************************************************/
uint8_t I2C_ReadByte(void) 
{
    uint8_t i,temp = 0;
    TSM_SDA_IN();
    TSM_SCL_L;
    for(i= 0; i<8; i++) 
    {
        temp <<= 1; 
        TSM_SCL_H; 
        IIC_delay(1);
        if(TSM_SDA_READ()) temp++; 
        TSM_SCL_L;
        IIC_delay(1);
    } 
    return temp;
}
/**************************************************************/
uint8_t TSM_Write_Byte(uint8_t WriteAddr,uint8_t WriteData)
{
    I2C_Start(); 
    I2C_WriteByte(DEVICE_WRITE_ADDR); 
    if(I2C_WaitAck()){I2C_Stop();return ERROR;}
    I2C_WriteByte(WriteAddr);
    if(I2C_WaitAck()){I2C_Stop();return ERROR;}
    I2C_WriteByte(WriteData); 
    if(I2C_WaitAck()){I2C_Stop();return ERROR;}
    I2C_Stop(); 
    return SUCCESS;      
}
/**************************************************************/
uint8_t TSM_Write_2Byte(uint8_t WriteAddr, uint8_t WriteData1, uint8_t WriteData2) 
{
    I2C_Start();
    I2C_WriteByte(DEVICE_WRITE_ADDR);
    if(I2C_WaitAck()){I2C_Stop();return ERROR;}
    I2C_WriteByte(WriteAddr); 
    if(I2C_WaitAck()){I2C_Stop();return ERROR;}
    I2C_WriteByte(WriteData1);
    if(I2C_WaitAck()){I2C_Stop();return ERROR;}
    I2C_WriteByte(WriteData2);
    if(I2C_WaitAck()){I2C_Stop();return ERROR;} 
    I2C_Stop();
    return SUCCESS;
}
/**************************************************************/
uint8_t TSM_ReadOneByte(uint8_t writeAddr)
{
    uint8_t buffer = 0; 
    I2C_Start(); 
    I2C_WriteByte(DEVICE_WRITE_ADDR);
    if(I2C_WaitAck()) goto L_OUT;
    I2C_WriteByte(writeAddr);
    if(I2C_WaitAck()) goto L_OUT; 
    I2C_Stop();
    I2C_Start();
    I2C_WriteByte(DEVICE_READ_ADDR);
    if(I2C_WaitAck()) goto L_OUT;
    buffer = I2C_ReadByte();
    I2C_SendAck(1);
    I2C_Stop(); 
    return buffer;
 L_OUT: 
    I2C_Stop();
    return ERROR;   
}
/**************************************************************/
uint8_t TSM_ReadMutiBytes(uint8_t writeAddr, uint8_t *buffer, u8 length) 
{
    uint8_t i; 
    I2C_Start();
    I2C_WriteByte(DEVICE_WRITE_ADDR);
    if(I2C_WaitAck()) goto L_OUT;
    I2C_WriteByte(writeAddr);
    if(I2C_WaitAck()) goto L_OUT; 
    I2C_Stop();
    I2C_Start();
    I2C_WriteByte(DEVICE_READ_ADDR);
    if(I2C_WaitAck()) goto L_OUT;
    for( i = 0; i<length; i++ )
    {
        buffer[i] = I2C_ReadByte(); 
        if( i == (length-1) )  I2C_SendAck(1); // NAck = 1          
        else I2C_SendAck(0); // Ack = 0
    }      
    I2C_Stop(); 
    return SUCCESS;
      
 L_OUT: 
    I2C_Stop();
    return ERROR;    
}
/**************************************************************/
void TSM12_Init(void)
{
    TSM_EN_L;
    delay_ms(2);         
    TSM_Write_Byte(CTRL2, 0x0f);//enable sleep mode
    //Sensitivity setting ,0x00灵敏度最高
    TSM_Write_Byte(SENS1, 0xaa);// 0, #
    TSM_Write_Byte(SENS2, 0xaa);// 7, *
    TSM_Write_Byte(SENS3, 0xaa);// 5, 8
    TSM_Write_Byte(SENS4, 0xaa);// 1, 4
    TSM_Write_Byte(SENS5, 0xaa);// 3, 2
    TSM_Write_Byte(SENS6, 0xaa);// 9, 6
    //General Control Register1 setting
    TSM_Write_Byte(CTRL1, 0x31);
    TSM_Write_2Byte(REF_RST1, 0x00, 0x00);//renew value of everyone channel
    TSM_Write_2Byte(CH_HOLD1, 0x00, 0x00);//enable all channels
    TSM_Write_Byte(CTRL2, 0x03);
    delay_ms(2); 
    TSM_EN_H;       //TSM12 I2C disable        
}
/**************************************************************/
void TSM12_SleepInit(void)
{
    TSM_EN_L;
    delay_ms(2);         
    TSM_Write_Byte(CTRL2, 0x0f);//enable sleep mode
    //Sensitivity setting ,0x00灵敏度最高
    TSM_Write_Byte(SENS1, 0xcc);// ., 1     0xcc
    TSM_Write_Byte(SENS2, 0xcc);// ., .
    TSM_Write_Byte(SENS3, 0xcc);// ., .
    TSM_Write_Byte(SENS4, 0xcc);// ., .
    TSM_Write_Byte(SENS5, 0xcc);// ., .
    TSM_Write_Byte(SENS6, 0xcc);// 2, .
    //General Control Register1 setting
    TSM_Write_Byte(CTRL1, 0x31);
    TSM_Write_2Byte(REF_RST1, 0x00, 0x00);//renew value of everyone channel
    TSM_Write_2Byte(CH_HOLD1, 0x00, 0x00);//enable all channels
    TSM_Write_Byte(CTRL2, 0x03);
    delay_ms(2); 
    TSM_EN_H;       //TSM12 I2C disable        
}
/**************************************************************/
void TSM12_Into_Sleep_Mode(void)
{
    TSM_EN_H;
    delay_ms(5); 
    TSM_EN_L;       //i2c enable
    delay_ms(5);  
    TSM_Write_Byte(CTRL2, 0x07);         
    TSM_EN_H;
}
/**************************************************************/
void TSM12_close(void)
{
    TSM_EN_L;       //i2c enable
    delay_ms(2);  
    TSM_Write_2Byte(CH_HOLD1, 0xff, 0x0f); 
    delay_ms(2);
    TSM_EN_H;
   // TSM12_Into_Sleep_Mode();
}
/**************************************************************/
void TSM12_open(void)
{
    TSM_EN_L;       //i2c enable
    delay_ms(2);  
    TSM_Write_2Byte(CH_HOLD1, 0x00, 0x00); 
    delay_ms(2);
    TSM_EN_H;
   // TSM12_Into_Sleep_Mode();
}
/**************************************************************/
void TSM12_Exit_Sleep_Mode(void)
{
    TSM_EN_L;
    delay_ms(1); 

    TSM_Write_Byte(CTRL2, 0x03);         
    delay_ms(2); 
    TSM_EN_H;       //TSM12 I2C disable   
}

/******************************************************************************/
