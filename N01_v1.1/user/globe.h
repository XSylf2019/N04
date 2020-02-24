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

extern void Delayms(uint16_t time);//�����ʱ���ܳ�1700,����λ
extern void TimingDelay_Decrement(void);

extern volatile uint8_t SleepDelayCount;//˯�ߵ���ʱ
extern uint8_t OpenType;//���ŷ�ʽ 0����1ָ��2��3����4��̬
extern uint8_t UserPower;//�û�Ȩ�� 0x20����Ա 0x00��ͨ 0x10���� 0x30в�� 0x40��ķ 0x50��̬���� 0x60����
extern uint8_t UserType;//�û����� 1ָ�� 2�� 3����
extern uint16_t IDuserCode;//�û����
extern volatile uint8_t gu_rtcwkflag;//rtc��ʱ����ι��
/******************************************************************************/
#endif
