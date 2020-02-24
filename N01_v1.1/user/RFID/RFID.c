#include "RFID.h"
#include "sky1311t.h"
#include "sky1311_drv.h"
#include "cardoper.h"
#include "spi.h"
/*******************************************************************************/
uint16_t rcValue;
/*******************************************************************************/
uint8_t sky1311_init(void)
{
	SKY1311_SPI_Init();
	delay_ms(50);
	
	rcValue = sky1311RCFreqCali();
	
	if(rcValue == 0)return 0;
	
	rcValue -= 10;
	
	irqClearAll();              // ���SKY1311�����ж�
	checkCardInit(rcValue);     // ���ý����Զ��쿨ģʽ
	return 1;
}
/*******************************************************************************/
uint8_t sky1311sleep_init(void)
{
	rcValue = sky1311RCFreqCali();
	
	if(rcValue == 0)return 0;
	
	rcValue -= 10;
	
	irqClearAll();              // ���SKY1311�����ж�
	checkCardInit(rcValue);     // ���ý����Զ��쿨ģʽ
	return 1;
}
/*******************************************************************************/
uint8_t SKY1311_IRQ_Callback(uint8_t *uid_data, uint8_t *uid_len)
{
	uint8_t irq_sta;
	uint8_t sta = 0;
	uint8_t data_len = 0;

	irq_sta = sky1311ReadReg(ADDR_IRQ_STA); //���жϼĴ���ֵ
	if((0==irq_sta)) // IRQ pin high
	{
		checkCardDeinit();          // ֹͣ�Զ�Ѱ��
		
		sta = TypeA_test(uid_data, &data_len);
		if(sta!= Ok )              // read Type A card
		{
			sta = TypeB_test(uid_data, &data_len);     // read Type B card
		}
		
		*uid_len = data_len;
	}
	return sta;
}
/*******************************************************************************/
void sky1311_checkCardInit(void)
{
	sky1311Enable();
	
	irqClearAll();              // ���SKY1311�����ж�
	
	checkCardInit(rcValue);     // ���ý����Զ��쿨ģʽ	
	SKY1311_DISABLE();
}
/*******************************************************************************/
