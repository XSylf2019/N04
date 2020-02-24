#include "gpio.h"
/******************************************************************************/
void TSM12_io_init(void)
{   
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
    EXTI_ClearITPendingBit(EXTI_IT_Pin2);
    GPIO_Init(TSM12_IRQ_PORT, TSM12_IRQ_PIN, GPIO_Mode_In_PU_IT);               //输入上拉,有中断
    EXTI_SelectPort(EXTI_Port_B);
    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising);
      
    //Aey
    EXTI_ClearITPendingBit(EXTI_IT_Pin2);
    GPIO_Init(TSM12_IRQ_PORT, TSM12_IRQ_PIN, GPIO_Mode_In_PU_IT);               //输入上拉,有中断
    EXTI_SelectPort(EXTI_Port_B);
    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising);
}
/******************************************************************************/

/******************************************************************************/