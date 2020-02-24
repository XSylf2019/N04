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

uint8_t sysStatus;//ģʽ
volatile uint8_t SleepDelayCount;//˯�ߵ���ʱ
volatile uint16_t Time1ms_count;
volatile uint8_t gu_rtcwkflag = 0;//rtc��ʱ����ι��
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
void Delayms(uint16_t time)//�����ʱ���ܳ�1700,����λ
{
  uint16_t i;
  
  IWDG_ReloadCounter();//ι��
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
    
    PWR_FastWakeUpCmd(DISABLE);//�رտ��ٻ��ѹ���
    PWR_UltraLowPowerCmd(ENABLE);//ʹ�ܵ�Դ�ĵ͹���ģʽ����仰�����úܴ�
    CLK_HSICmd(DISABLE);//�ر��ڲ�����ʱ�ӣ�֮ǰ�ں��ǹ�����HSI�µģ�
    
ENTERSTOPMODE:
  
    RTCWakeUp_Config(2047);//2047��������1s
    halt();//����һ��ִ������ں�ʱ�Ӿ�ֹͣ�ˣ�����ͣ��ģʽ

    if(gu_rtcwkflag)
    {
        gu_rtcwkflag = 0;
        LED_Toggle();
        IWDG_ReloadCounter();//ι��
        goto ENTERSTOPMODE;//ι�깷����ֹͣģʽ
    }
}
/******************************************************************************/
void WakeUp_Init(void)
{
    SystemClock_Init();                                                         //ϵͳʱ�ӳ�ʼ��
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
    
    SystemClock_Init();                                                         //ϵͳʱ�ӳ�ʼ��
    
    Key_io_init();                                                              //IO��ʼ��
    
    UART1_Init(9600);                                                           //����1��ʼ��
    
    RTC_Setting_Init();                                                         //RTC��ʼ��
    
    watchdog_init();                                                            //���Ź���ʼ��
    
    TIM4_init();                                                                //1 ms��ʱ������
    
    ADC_init();                                                                 //adc��ʼ��

    enableInterrupts();                                                         //ʹ���ж�
    
    sysStatus = STATUS_RUN_MODE;
    SleepDelayCount = 10;                                                       //20s��ϵͳ����˯��
   
    UART1_Send_Byte(0x55);
    UART1_Send_Byte(0x55);
    Delayms(100);
    
    while (1)
    {
        IWDG_ReloadCounter();//ι��
        switch(sysStatus)
        {
          case STATUS_RUN_MODE:                                                 //����ģʽ 
            Key_Resetbutton();
            UART1_ReceiveData();
            if(timeBackup != SleepDelayCount)
            {
                RTC_Get_Time();                                                 //��ȡRTC��ǰʱ��
                timeBackup = SleepDelayCount;
//                UART1_Send_String("����");
                

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
            
          case STATUS_STOP_MODE:                                                //ֹͣģʽ+
            EnterStop_Init();                                                   //���ý���ֹͣģʽ
            WakeUp_Init();                                                      //rtc���Ѻ�PCָ������������￪ʼִ��
            break;
        }
    }
}






/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
