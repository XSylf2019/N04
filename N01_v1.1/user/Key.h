#ifndef __KEY_H_
#define __KEY_H_
/******************************************************************************/
#include "stm8l15x.h"
/******************************************************************************/
#define   DEVICE_WRITE_ADDR 0xd0
#define   DEVICE_READ_ADDR  0xd1
#define   ERROR   0

#define CH_SENS(a,b)  0x##a##b

#define SENS1  0x02 //Sensitivity, Channel 2 and 1     
#define SENS2  0x03 //Channel 4 and 3     
#define SENS3  0x04 //Channel 6 and 5     
#define SENS4  0x05 //Channel 8 and 7     
#define SENS5  0x06 //Channel 10 and 9     
#define SENS6  0x07 //Channel 12 and 11
#define CTRL1  0x08
#define CTRL2  0x09
#define REF_RST1   0x0A
#define REF_RST2   0x0B
#define CH_HOLD1   0x0C
#define CH_HOLD2   0x0D
#define CAL_HOLD1  0x0E
#define CAL_HOLD2  0x0F
#define OUTPUT1  0x10
#define OUTPUT2  0x11
#define OUTPUT3  0x12

typedef struct
{
    uint8_t ContinuePress;
    uint16_t CopyUser;
    uint8_t CopyCount;
    uint8_t keyInputCount;
    uint8_t okPressTime;
}KeyPads;
extern KeyPads Key;

#define BEEP_POINT      0
#define BEEP_OK         1
#define BEEP_DEFAIT     2
#define BEEP_ALARM      3
#define BEEP_set        4
#define BEEP_clear      5
/******************************************************************************/
void Key_io_init(void);
void SoundBeep(uint8_t type);//·äÃùÆ÷ÉùÒô
void TSM12_Init(void);
void TSM12_SleepInit(void);
void TSM12_Into_Sleep_Mode(void);
void TSM12_Exit_Sleep_Mode(void);
void TSM12_close(void);
void TSM12_open(void);
void Key_Resetbutton(void);
/******************************************************************************/	
#endif 
