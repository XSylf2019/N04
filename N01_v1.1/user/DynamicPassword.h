#ifndef __DYNAMICPASSWORD_H_
#define __DYNAMICPASSWORD_H_
/******************************************************************************/
#include "stm8l15x.h"
/******************************************************************************/
uint8_t DynamicPwdCertification(uint8_t *buff);                                 //8λ��ʱ����
uint8_t PeriodPwdCertification(uint8_t *buff);                                  //9λʱ������
uint8_t ValidityPwdCertification(uint8_t *buff);                                //10λʱЧ����
uint32_t GetRealSecondTime(void);
void GetDateTimeFromSecond(uint32_t lSec);
/******************************************************************************/
#endif
