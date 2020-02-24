#include "lock.h"
#include "delay.h"
#include "gpio.h"
#include "Key.h"
#include "adc.h"
#include "globe.h"
/******************************************************************************/
void openMoto(void)
{	
    MOTOR_H(); 
    delay_ms(700);
    MOTOR_L(); 
    MOTOL_L();
}
/******************************************************************************/     
void closeMoto(void)
{
    MOTOL_H(); 
    delay_ms(700);
    MOTOR_L(); 
    MOTOL_L();
}
/******************************************************************************/
void Open_Lock(uint16_t code)
{
    uint8_t i;
    
    openMoto();
    if(getBattVotatile()<680)SoundBeep(BEEP_ALARM);                             //µÎ.µÎ.µÎ.µÎ.µÎ. <4.8vµÍÑ¹±¨¾¯
    for(i = 0;i < 50;i ++)
    {
        delay_ms(100);
    }
    closeMoto();
    SleepDelayCount = 2;
}
/******************************************************************************/