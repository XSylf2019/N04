/**
  ******************************************************************************
  * @file    EXTI/EXTI_IOControl/main.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    13-May-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "SYS_CLOCK.h"
#include "TIM.h"
#include "adc.h"
#include "uart.h"
#include "rtc.h"
#include "iwdg.h"
#include "key.h"
#include "delay.h"
#include "gpio.h"
#include "globe.h"
/******************************************************************************/
#define STATUS_RUN_MODE         0x01
#define STATUS_STOP_MODE	0x02

uint8_t sysStatus;//模式
volatile uint8_t SleepDelayCount;//睡眠倒计时
volatile uint16_t Time1ms_count;
volatile uint8_t gu_rtcwkflag = 0;//rtc定时唤醒喂狗
/*******************************************************************************
void delay(unsigned int n)
{
  while(n-- > 0);
}
*******************************************************************************/
void Delay(__IO uint16_t nCount)
{
  while (nCount != 0)/* Decrement nCount value */
  {
    nCount--;
  }
}
/******************************************************************************/
//About 1 second, not exactly, just for demonstration purposes
void Delayms(uint16_t time)//最大延时不能超1700,否则复位
{
  uint16_t i;
  
  IWDG_ReloadCounter();//喂狗
  while (time--)
  {
    for (i = 2250; i > 0; i--)
    {
      asm("nop");
    }
  }
}
/******************************************************************************/
void TimingDelay_Decrement(void)
{
    Time1ms_count ++;
    if(Time1ms_count > 1000)//1s
    {
        Time1ms_count = 0;
        if(SleepDelayCount)SleepDelayCount--;
    }
}
/******************************************************************************/
void EnterStop_Init(void)
{
    Delayms(100);
    
    LED_OFF();
    USART_DeInit(USART1);
    TIM4_DeInit();
    ADC_DeInit(ADC1);
    
    PWR_FastWakeUpCmd(DISABLE);//关闭快速唤醒功能
    PWR_UltraLowPowerCmd(ENABLE);//使能电源的低功耗模式（这句话的作用很大）
    CLK_HSICmd(DISABLE);//关闭内部高速时钟（之前内核是工作在HSI下的）
    
ENTERSTOPMODE:
  
    RTCWakeUp_Config(2047);//2047计数正好1s
    halt();//到这一步执行完后，内核时钟就停止了，进入停机模式

    if(gu_rtcwkflag)
    {
        gu_rtcwkflag = 0;
        LED_Toggle();
        IWDG_ReloadCounter();//喂狗
        goto ENTERSTOPMODE;//喂完狗进入停止模式
    }
}
/******************************************************************************/
void WakeUp_Init(void)
{
    SystemClock_Init();                                                         //系统时钟初始化
    SleepDelayCount = 10;
    sysStatus = STATUS_RUN_MODE;
    
    UART1_Init(9600);
    TIM4_init();
    ADC_init();
//    RTC_WakeUpCmd(DISABLE);
}
/******************************************************************************/
int main(void)
{
    uint8_t timeBackup = 0;
    
    SystemClock_Init();                                                         //系统时钟初始化
    
    Key_io_init();                                                              //IO初始化
    
    UART1_Init(9600);                                                           //串口1初始化
    
    RTC_Setting_Init();                                                         //RTC初始化
    
    watchdog_init();                                                            //看门狗初始化
    
    TIM4_init();                                                                //1 ms定时计数器
    
    ADC_init();                                                                 //adc初始化

    enableInterrupts();                                                         //使能中断
    
    sysStatus = STATUS_RUN_MODE;
    SleepDelayCount = 10;                                                       //20s后系统进入睡眠
   
    UART1_Send_Byte(0x55);
    UART1_Send_Byte(0x55);
    Delayms(100);
    
    while (1)
    {
        IWDG_ReloadCounter();//喂狗
        switch(sysStatus)
        {
          case STATUS_RUN_MODE:                                                 //快速模式 
            Key_Resetbutton();
            UART1_ReceiveData();
            if(timeBackup != SleepDelayCount)
            {
                RTC_Get_Time();                                                 //获取RTC当前时间
                timeBackup = SleepDelayCount;
//                UART1_Send_String("闪灯");
                

                UART1_Send_Byte(clock.cYear);
                UART1_Send_Byte(clock.cMonth);
                UART1_Send_Byte(clock.cDay);
                //UART1_Send_Byte(clock.cWeek);
                
                UART1_Send_Byte(clock.cHour);
                UART1_Send_Byte(clock.cMinute);
                UART1_Send_Byte(clock.cSecond);
            }
            Delayms(100);
            LED_Toggle();
            if(!SleepDelayCount)
            {
                sysStatus = STATUS_STOP_MODE;
            }
            break;
            
          case STATUS_STOP_MODE:                                                //停止模式+
            EnterStop_Init();                                                   //配置进入停止模式
            WakeUp_Init();                                                      //rtc唤醒后PC指针在这里，从这里开始执行
            break;
        }
    }
}






/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
