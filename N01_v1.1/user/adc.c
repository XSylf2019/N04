#include "adc.h"
/******************************************************************************/
//uint8_t guBattSampleTime;
//uint32_t guBattLevelSum;
//uint16_t guBattLevelResult;
/******************************************************************************/
void ADC_init(void)
{
/* Enable ADC clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

/* de-initialize ADC */
  ADC_DeInit(ADC1);

/*ADC configuration
  ADC configured as follow:
  - Channel VREF
  - Mode = Single ConversionMode(ContinuousConvMode disabled)
  - Resolution = 12Bit
  - Prescaler = /2
  - sampling time 384 */
  
  ADC_VrefintCmd(DISABLE);//启用或禁用内部电压参考
  //delay_10us(3);
  ADC_Cmd(ADC1, ENABLE);	 
  ADC_Init(ADC1, ADC_ConversionMode_Single,ADC_Resolution_12Bit, ADC_Prescaler_2);//单次转换，12位，ADC转换时钟1分频
  
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
  ADC_ChannelCmd(ADC1, ADC_Channel_0, ENABLE);//使能通道0------PA6
}
/******************************************************************************/
uint16_t adcGetUint(void)
{
    /* start ADC convertion by software */
    ADC_SoftwareStartConv(ADC1);
    /* wait until end-of-covertion */
    while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0 );
    /* Clears the ADC's pending flags */
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//清除对应标志
    /* read ADC convertion result */
    return ADC_GetConversionValue(ADC1);                                        /* 输出结果*/
}
/******************************************************************************/
uint16_t getBattVotatile(void)
{
    uint8_t i;
    uint32_t guBattLevelSum = 0;
    uint16_t guBattLevelResult = 0;
    
    for(i = 0; i < 10; i ++)                                                    //测量10次
    {
       guBattLevelSum += adcGetUint();
    }
    guBattLevelResult = guBattLevelSum / 10;
    
    return guBattLevelResult;
}
/******************************************************************************/