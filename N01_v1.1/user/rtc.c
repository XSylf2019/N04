#include "rtc.h"
/******************************************************************************/
StruCLOCK clock;

const uint8_t MonthLength[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/******************************************************************************/
//void RTC_Setting_Init(void)
//{
//    RTC_InitTypeDef RTC_InitStr;
//    RTC_TimeTypeDef RTC_TimeStr;
//    RTC_DateTypeDef RTC_DateStr;
//
//    //RTC Clock Init
//    CLK_LSEConfig(CLK_LSE_ON); //Open LSE Clock 32.768K
//    while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET); //wait is ready
//    
////    RTC_DeInit(); //此句可以不加，视情况而定，加了之后，功耗会提高很多
//    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1); //select LSE no div 32.768K
//    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTC时钟门控使能
//    RTC_WakeUpCmd(DISABLE);
//
//
//    RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;//24小时格式
//    RTC_InitStr.RTC_AsynchPrediv = 0x7f; //32.768K/(127+1)=256
//    RTC_InitStr.RTC_SynchPrediv = 0x0ff; //256/(255+1)=1
//    RTC_Init(&RTC_InitStr);
//
//    //Write default time 20.1.1 00:00:00
//    RTC_DateStructInit(&RTC_DateStr);
//    RTC_DateStr.RTC_Date = 0x01;
//    RTC_DateStr.RTC_Month = RTC_Month_January;
//    RTC_DateStr.RTC_Year = 0x20;
//    RTC_SetDate(RTC_Format_BCD, &RTC_DateStr);//BCD格式
//
//    RTC_TimeStructInit(&RTC_TimeStr);
//    RTC_TimeStr.RTC_Hours = 0x00;
//    RTC_TimeStr.RTC_Minutes = 0x00;
//    RTC_TimeStr.RTC_Seconds = 0x00;
//    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStr);//BCD格式
//
//}
void RTC_Setting_Init(void)
{
    RTC_InitTypeDef RTC_InitStr;
    RTC_TimeTypeDef RTC_TimeStr;
    RTC_DateTypeDef RTC_DateStr;

    //RTC Clock Init
    CLK_LSEConfig(CLK_LSE_ON); //Open LSE Clock 32.768K
    while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET); //wait is ready
    
//    RTC_DeInit(); //此句可以不加，视情况而定，加了之后，功耗会提高很多
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1); //select LSE no div 32.768K
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTC时钟门控使能

    RTC_WakeUpCmd(DISABLE);
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//32.768k/16=2.048KHz t=0.488ms
    RTC_ITConfig(RTC_IT_WUT, ENABLE);//开启中断 


    RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;//24小时格式
    RTC_InitStr.RTC_AsynchPrediv = 0x7f; //32.768K/(127+1)=256
    RTC_InitStr.RTC_SynchPrediv = 0x0ff; //256/(255+1)=1
    RTC_Init(&RTC_InitStr);

    //Write default time 20.1.1 00:00:00
    RTC_DateStructInit(&RTC_DateStr);
    RTC_DateStr.RTC_Date = 0x01;
    RTC_DateStr.RTC_Month = RTC_Month_January;
    RTC_DateStr.RTC_Year = 0x20;
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStr);//BCD格式

    RTC_TimeStructInit(&RTC_TimeStr);
    RTC_TimeStr.RTC_Hours = 0x00;
    RTC_TimeStr.RTC_Minutes = 0x00;
    RTC_TimeStr.RTC_Seconds = 0x00;
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStr);//BCD格式

}
/******************************************************************************/
void RTC_Get_Time(void)
{
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;

    RTC_GetTime(RTC_Format_BCD, &time); //get the time
    RTC_GetDate(RTC_Format_BCD, &date); //get the date

    clock.cHour = time.RTC_Hours;
    clock.cMinute = time.RTC_Minutes;
    clock.cSecond = time.RTC_Seconds;

    clock.cYear = date.RTC_Year;
    clock.cMonth = date.RTC_Month;
    clock.cDay = date.RTC_Date;
    clock.cWeek = date.RTC_WeekDay;
}
/******************************************************************************/
//void RTCWakeUp_Config(uint16_t time)
//{
//    RTC_WakeUpCmd(DISABLE);
//    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//32.768k/16=2.048KHz t=0.488ms 
//    RTC_ITConfig(RTC_IT_WUT, ENABLE);//开启中断 
//    RTC_SetWakeUpCounter(time);//设置RTC Weakup计算器初值 t=time*0.488
//    enableInterrupts();
//    RTC_WakeUpCmd(ENABLE);//使能自动唤醒
//}
void RTCWakeUp_Config(uint16_t time)
{
    RTC_SetWakeUpCounter(time);//设置RTC Weakup计算器初值 t=time*0.488
    RTC_WakeUpCmd(ENABLE);//使能自动唤醒
}
/******************************************************************************/
void Set_RTC_Time(uint8_t *TimeBuff)
{
    RTC_InitTypeDef RTC_InitStr;
    RTC_TimeTypeDef RTC_TimeStr;
    RTC_DateTypeDef RTC_DateStr;
  
    RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;//24小时格式
    RTC_InitStr.RTC_AsynchPrediv = 0x7f; //32.768K/(127+1)=256
    RTC_InitStr.RTC_SynchPrediv = 0x0ff; //256/(255+1)=1
    RTC_Init(&RTC_InitStr);

    //Write default time 20.1.1 00:00:00
    RTC_DateStructInit(&RTC_DateStr);
    RTC_DateStr.RTC_Date = TimeBuff[2];
    RTC_DateStr.RTC_Month = TimeBuff[1];
    RTC_DateStr.RTC_Year = TimeBuff[0];
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStr);//BCD格式

    RTC_TimeStructInit(&RTC_TimeStr);
    RTC_TimeStr.RTC_Hours = TimeBuff[3];
    RTC_TimeStr.RTC_Minutes = TimeBuff[4];
    RTC_TimeStr.RTC_Seconds = TimeBuff[5];
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStr);//BCD格式
}
/******************************************************************************/