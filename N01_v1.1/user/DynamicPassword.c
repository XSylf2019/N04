#include "DynamicPassword.h"
#include "rtc.h"
/******************************************************************************/
extern StruCLOCK clock;

void GetCRC(uint8_t message[],uint8_t CRC[]);

uint8_t guDynamicPwdSwitch = 0;
uint8_t guDynamicvalue;
uint8_t original_password[8]={0};
uint8_t DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

uint16_t gModify_uYear;
uint8_t gModify_uMonth;
uint8_t gModify_uData;
uint8_t gModify_uHoure;
uint8_t gModify_uMinute;
uint8_t gModify_uSecond;
uint8_t gModify_uWeek;

#define DAY_OF_SECOND   86400
#define HOURS_OF_SECOND	3600
/*******************************************************************************
 * 功能：
 *     根据时间计算出CountDay时间戳
 * 参数：
 *     currTime：给定的时间 
 *     daylightSaving：是否是夏令时
 *     1970/1/1/0/0/0
 * 返回值：
 *     CountDay时间戳
*******************************************************************************/
uint32_t GetRealSecondTime(void)
{
    uint16_t iYear;
    uint8_t iMon, iDay, iHour, iMin, iSec;

    iYear = (((clock.cYear>>12)&0x000f)*1000);
    iYear += (((clock.cYear>>8)&0x000f)*100);
    iYear += (((clock.cYear>>4)&0x000f)*10);
    iYear += clock.cYear&0x0f;

    iMon = ((clock.cMonth>>4)*10);
    iMon += clock.cMonth&0x0f;
    
    iDay = ((clock.cDay>>4)*10);
    iDay += clock.cDay&0x0f;
    
    iHour = ((clock.cHour>>4)*10);
    iHour += clock.cHour&0x0f;
    
    iMin = ((clock.cMinute>>4)*10);
    iMin += clock.cMinute&0x0f;
    
    iSec = ((clock.cSecond>>4)*10);
    iSec += clock.cSecond&0x0f;
    
    uint16_t i, Cyear=0;
    uint32_t CountDay=0;
    
    for(i=1970; i<iYear; i++)   /* 1970 */
    {
      if(((i%4==0) && (i%100!=0)) || (i%400==0)) Cyear++;
    }
    CountDay = Cyear * 366 + (iYear-1970-Cyear) * 365;
    for(i=1; i<iMon; i++)
    {
      if((i==2) && (((iYear%4==0)&&(iYear%100!=0)) || (iYear%400==0)))
        CountDay += 29;
      else
        CountDay += DayOfMon[i-1];
    }
    CountDay += (iDay-1);
    
    CountDay = CountDay*DAY_OF_SECOND + (uint32_t)iHour*3600 + (uint32_t)iMin*60 + iSec;
    CountDay -= 28800;//注意这里减掉8小时时差
    return CountDay;
}
/*******************************************************************************
 * 功能：
 *     根据UTC时间戳得到对应的日期
 * 参数：
 *     lSec：给定的UTC时间戳
 *     daylightSaving：是否是夏令时
 *     1970/1/1/0/0/0
 * 返回值：
 *     无
*******************************************************************************/
void GetDateTimeFromSecond(uint32_t lSec)
{
    uint16_t i,j,iDay;
    uint32_t lDay;
    
    lSec = lSec + 8*3600;//北京时间+8小时的时差
    
    lDay = lSec / DAY_OF_SECOND;    
    lSec = lSec % DAY_OF_SECOND;
    
    i = 1970;
    while(lDay > 365)
    {
       if(((i%4==0)&&(i%100!=0)) || (i%400==0))  
       {
        lDay -= 366;
       }
       else
       {
        lDay -= 365;
       }
       i++;
    }
    if((lDay == 365) && !(((i%4==0)&&(i%100!=0)) || (i%400==0))) 
    {
       lDay -= 365;
       i++;
    }
    gModify_uYear =i;
    for(j=0;j<12;j++)   
    {
       if((j==1) && (((i%4==0)&&(i%100!=0)) || (i%400==0)))
       {
        iDay = 29;
       }
       else
       {
        iDay = DayOfMon[j];
       }
       if(lDay >= iDay)lDay -= iDay;
       else break;
    }
    gModify_uMonth = j+1;
    gModify_uData = lDay+1;
    gModify_uHoure = ((lSec / 3600))%24;
    gModify_uMinute = (lSec % 3600) / 60;
    gModify_uSecond = (lSec % 3600) % 60;
}
/******************************************************************************/
void GetCRC(uint8_t message[],uint8_t CRC[])
{
	//Function expects a modbus message of any length as well as a 2 byte CRC array in which to
	//return the CRC values:
	unsigned short CRCFull = 0xFFFF;
	char CRCLSB;
	uint8_t crc_num;
	crc_num=15;
	for (int i = 0; i < crc_num; i++)
	{
		CRCFull = (unsigned short)(CRCFull ^message[i]);
		for (int j = 0; j < 8; j++)
		{
			CRCLSB = (char)(CRCFull & 0x0001);
			CRCFull = (unsigned short)((CRCFull >> 1) & 0x7FFF);
			if (CRCLSB == 1)
				CRCFull = (unsigned short)(CRCFull ^ 0xA001);
		}
	}
	CRC[1] = (uint8_t)((CRCFull >> 8) & 0xFF);
	CRC[0] = (uint8_t)(CRCFull & 0xFF);
}
/******************************************************************************/
uint8_t  Password_process(uint8_t *buff,uint8_t *date,uint8_t flag)
{    
	uint8_t new_date[15] = {0};
	uint8_t new_password[8] = {0};
	uint8_t CRC[2] = {0};
	uint8_t temp;  //用于比较大小
	uint8_t produce_data;
	uint8_t a;   //百位
	uint8_t b;   //十位
	uint8_t k=1;
	uint8_t t=0;
//	if(guDynamicPwdSwitch != 1)return 0;
//	ReadDynamicPwd(original_password);       
//	if(original_password[0] == 0 || original_password[0] == 0xff)return 0; 
	for(temp = 0; temp < 8;temp ++)original_password[temp] &= 0x0f;
	for(uint8_t i = 0;i < 7;i ++)
	{            
		new_date[k] = date[i];
		k=k+2;
	}
	for(uint8_t j = 0;j < 8;j ++)
	{           
		new_date[t]=original_password[j];
		t=t+2;
	}
	for(uint8_t m = 0;m < 8;m ++)
	{
		GetCRC(new_date,CRC);
		if(CRC[0]<CRC[1])
		{
				temp=CRC[0];
		}
		else
		{
				temp=CRC[1];
		}
		if(temp>=0x64)
		{
				a=temp/0x64;
				b=(temp-(a<<6)-(a<<5)-(a<<2));
				b=b/0x0A;
				produce_data=temp-(a<<6)-(a<<5)-(a<<2)-(b<<3)-(b<<1);
		}
		else if(temp<0x64&&temp>=0x0A)
		{
				b=temp/0x0A;
				produce_data=temp-(b<<3)-(b<<1);
		}
		else
		{
				produce_data=temp;
		}
		new_password[m]=produce_data;
		new_date[0]=CRC[0];
		new_date[1]=CRC[1];
	}
	if(flag==1)//临时密码
	{
		if(buff[0] == new_password[0] && buff[1] == new_password[1] && buff[2] == new_password[2] && 
			buff[3] == new_password[3] && buff[4] == new_password[4] && buff[5] == new_password[5] && 
			buff[6] == new_password[6] && buff[7] == new_password[7])
		{
			 return 1;
		}
	}
	else if(flag==2)//时段密码
	{
		if(buff[0] == new_password[0] && buff[1] == new_password[1] && buff[2] == new_password[2] && 
			buff[3] == new_password[3] && buff[4] == new_password[4]/* && buff[5] == new_password[5]*/)//5位密码
		{
			 return 1;
		}
	}
	else if(flag==3)//时效密码
	{
		if(buff[0] == new_password[0] && buff[1] == new_password[1] && buff[2] == new_password[2] && 
			buff[3] == new_password[3] && buff[4] == new_password[4] && buff[5] == new_password[5])//6位密码
		{
			 return 1;
		}
	}
	return 0;
}
/******************************************************************************/
uint8_t DynamicPwdCertification(uint8_t *buff)//8位临时密码
{
	uint8_t i;
	uint8_t date[7]={0};//调取的是当前的实时日期 2016/05/25 19/34/58 转换为16进制数即为6个字节
	uint32_t CountDay=0;

	CountDay = GetRealSecondTime();//转时间戳
	CountDay -= 60*5;//5分钟前
	for(i = 0;i < 10; i ++)//当前时间前后5分钟
	{
		CountDay += 60*i;
		GetDateTimeFromSecond(CountDay);
		CountDay -= 60*i;
		
		date[0] = (uint8_t)(gModify_uYear >> 8);//年
		date[1] = (uint8_t)(gModify_uYear & 0x00FF);//年
		date[2] = gModify_uMonth;//月
		date[3] = gModify_uData;//日
		date[4] = gModify_uHoure;//时
		date[5] = 0;
		date[6] = gModify_uMinute;//分
		
		if(Password_process(buff,date,1))return 1;   
	}
	return 0;
}
/******************************************************************************/
uint8_t PeriodPwdCertification(uint8_t *buff)//9位时段密码
{
	uint16_t i,p,j,k,index,count,m;
	uint8_t date[7]={0};//调取的是当前的实时日期 2016/05/25 19/34/58 转换为16进制数即为6个字节
	uint32_t CountDay=0;
	uint8_t InputPWD[10] = {0};
	uint8_t Renthousedays[3] = {0};
	uint16_t Renthousecount = 0,day=0;

	for(m = 0;m < 10; m ++)
	{
		InputPWD[m] = buff[m];//Copy输入的密码
	}
	index = buff[0];
	i = index%5;              //3
	j = (index+1+i)%6;        //4
	k = (index+1+j)%7;        //2
	for(m = 0;m < 8; m ++)
	{
		buff[m] = InputPWD[m+1];
	}
	count = 8;
	Renthousedays[0] = buff[k];       //2
	for(p = k;p < count-1; p ++)
	{
		buff[p] = buff[p+1];
	}
	Renthousedays[1] = buff[j];       //4
	for(p = j;p < count-2; p ++)
	{
		buff[p] = buff[p+1];
	}
	Renthousedays[2] = buff[i];       //3
	
	Renthousecount = Renthousedays[2]*100;
	Renthousecount = Renthousecount + Renthousedays[1]*10;
	Renthousecount = Renthousecount + Renthousedays[0];
	if(Renthousecount < 1 || Renthousecount > 720)return 0;//入住天数少于1或大于365天不成立
	for(m = 0;m < 8; m ++)
	{
		buff[m] = InputPWD[m+1];
	}
	count = 8;
	for(p = k;p < count; p ++)
	{
		buff[p] = buff[p+1];
	}
	for(p = j;p < count-1; p ++)
	{
		buff[p] = buff[p+1];
	}
	for(p = i;p < count-2; p ++)
	{
		buff[p] = buff[p+1];
	}
	day = Renthousecount - 1;
	CountDay = GetRealSecondTime();//实时时间转时间戳
	CountDay -= HOURS_OF_SECOND*day;//x小时有效,x小时前

	for(i = 0;i < Renthousecount; i ++)//x小时有效
	{
		CountDay += HOURS_OF_SECOND*i;
		GetDateTimeFromSecond(CountDay);
		CountDay -= HOURS_OF_SECOND*i;
		
		date[0] = (uint8_t)(gModify_uYear >> 8);//年
		date[1] = (uint8_t)(gModify_uYear & 0x00FF);//年
		date[2] = gModify_uMonth;//月
		date[3] = gModify_uData;//日
		date[4] = gModify_uHoure&0xf0;//时
		date[5] = 0;
		date[6] = gModify_uHoure&0x0f;//时
		
		if(Password_process(buff,date,2))return 1;
	}
	return 0;
}
/******************************************************************************/
uint8_t ValidityPwdCertification(uint8_t *buff)//10位时效密码
{
	uint16_t i,p,j,k,index,count,m;
	uint8_t date[7]={0};//调取的是当前的实时日期 2016/05/25 19/34/58 转换为16进制数即为6个字节
	uint32_t CountDay=0;
	uint8_t InputPWD[10] = {0};
	uint8_t Renthousedays[3] = {0};
	uint16_t Renthousecount=0,day=0;

	for(m = 0;m < 10; m ++)
	{
		InputPWD[m] = buff[m];//Copy输入的密码
	}
	index = buff[0];
	i = index%5;              //3
	j = (index+1+i)%6;        //4
	k = (index+1+j)%7;        //2
	for(m = 0;m < 9; m ++)
	{
		buff[m] = InputPWD[m+1];
	}
	count = 9;
	Renthousedays[0] = buff[k];       //2
	for(p = k;p < count-1; p ++)
	{
		buff[p] = buff[p+1];
	}
	Renthousedays[1] = buff[j];       //4
	for(p = j;p < count-2; p ++)
	{
		buff[p] = buff[p+1];
	}
	Renthousedays[2] = buff[i];       //3
	
	Renthousecount = Renthousedays[2]*100;
	Renthousecount = Renthousecount + Renthousedays[1]*10;
	Renthousecount = Renthousecount + Renthousedays[0];
	if(Renthousecount < 1 || Renthousecount > 365)return 0;//入住天数少于1或大于365天不成立
	for(m = 0;m < 9; m ++)
	{
		buff[m] = InputPWD[m+1];
	}
	count = 9;
	for(p = k;p < count; p ++)
	{
		buff[p] = buff[p+1];
	}
	for(p = j;p < count-1; p ++)
	{
		buff[p] = buff[p+1];
	}
	for(p = i;p < count-2; p ++)
	{
		buff[p] = buff[p+1];
	}
	day = Renthousecount - 1;
	CountDay = GetRealSecondTime();//实时时间转时间戳
	CountDay -= DAY_OF_SECOND*day;//x天有效,x天前

	for(i = 0;i < Renthousecount; i ++)//x天有效
	{
		CountDay += DAY_OF_SECOND*i;
		GetDateTimeFromSecond(CountDay);
		CountDay -= DAY_OF_SECOND*i;
		
		date[0] = (uint8_t)(gModify_uYear >> 8);//年
		date[1] = (uint8_t)(gModify_uYear & 0x00FF);//年
		date[2] = gModify_uMonth&0xf0;//月
		date[3] = gModify_uMonth&0x0f;//月
		date[4] = gModify_uData&0xf0;//日
		date[5] = 0;
		date[6] = gModify_uData&0x0f;//日
		
		if(Password_process(buff,date,3))return 1;   
	}
	return 0;
}
/******************************************************************************/
