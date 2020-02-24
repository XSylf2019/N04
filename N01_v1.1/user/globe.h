#ifndef __GLOBE_H_
#define __GLOBE_H_
/******************************************************************************/
#define VERSION         "N01_1.1"
#define DATA            "2020-02-17"
/******************************************************************************/
typedef struct   
{
    uint8_t data;
    uint8_t length;
    uint8_t index;
    uint8_t startflag;
    uint8_t completeflag;
    uint8_t buff[20];
    uint8_t comman;
}StruUSART1;
extern StruUSART1 UART1;

typedef struct   
{
    uint8_t data;
    uint8_t length;
    uint8_t index;
    uint8_t startflag;
    uint8_t completeflag;
    uint8_t buff[20];
    uint8_t comman;
}StruUSART2;
extern StruUSART2 UART2;

typedef struct   
{
    uint8_t data;
    uint8_t length;
    uint8_t index;
    uint8_t startflag;
    uint8_t completeflag;
    uint8_t buff[20];
    uint8_t comman;
}StruUSART3;
extern StruUSART3 UART3;

extern void Delayms(uint16_t time);//最大延时不能超1700,否则复位
extern void TimingDelay_Decrement(void);

extern volatile uint8_t SleepDelayCount;//睡眠倒计时
extern uint8_t OpenType;//开门方式 0出厂1指纹2卡3密码4动态
extern uint8_t UserPower;//用户权限 0x20管理员 0x00普通 0x10宾客 0x30胁迫 0x40保姆 0x50动态密码 0x60出厂
extern uint8_t UserType;//用户类型 1指纹 2卡 3密码
extern uint16_t IDuserCode;//用户编号
extern volatile uint8_t gu_rtcwkflag;//rtc定时唤醒喂狗
/******************************************************************************/
#endif
