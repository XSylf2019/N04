#ifndef __SAVE_H_
#define __SAVE_H_
/******************************************************************************/
#include "stm8l15x.h"
/******************************************************************************/
#define DATA_MEMORY_START_ADDR (FLASH_PROGRAM_START_PHYSICAL_ADDRESS + 0xE000)
#define DATA_MEMORY_STOP_ADDR  (FLASH_PROGRAM_END_PHYSICAL_ADDRESS)

#define ADJUST_DATA_STORE_ADDR          0x16000                                 //从56k开始存数据,8k用户存储区(0x16000 - 0x17FFF)
/******************************************************************************/
#define SAVE_Addr_FingerCounter  	8					//指纹数存放地址

#define	SAVE_Addr_Card_count		10				        //IC卡数存放地址

#define	SAVE_Addr_Password_count	12				        //密码数存放地址

#define SAVE_Addr_ManagerCounter        14				        //管理存放地址

#define SAVE_Addr_FamilyCounter  	16				        //家人存放地址

#define SAVE_Addr_GuestCounter  	18				        //客人存放地址

#define SAVE_Addr_doorcount	        20				        //开门条数

#define SAVE_ADDR_USER_OFFSE            30      	                        //用户数据存放地址
/******************************************************************************/
#define SAVE_FINGER_DATA_START		100	                                //指纹数据存储的起始位置   100+100*14=1500

#define SAVE_CARD_DATA_START		(SAVE_FINGER_DATA_START + MAXIMAL_FINGER_COUNTER * FINGER_USER_LENTH)      //密码数据存储的起始位置   1500+10*16=1660

#define SAVE_PASSWOD_DATA_START		(SAVE_CARD_DATA_START + MAX_USER_CARD_COUNT * CARD_USER_LENTH)      //卡数据存储的起始位置    1660+50*19=2610
/******************************************************************************/
#define SAVE_Addr_doordata              2650			                //开门数据
/* 定义最大 */ 
#define MAXIMAL_FINGER_COUNTER		100					//最大指纹数

#define	MAX_USER_CARD_COUNT             50					//最大卡数

#define	MAXIMAL_PASSWORD_COUNTER	10				        //最大密码数

#define MAXIMAL_MANAGER_COUNTER		10					//最大管理人数


/** 指纹用户长度 每个用户信息14bytes (2bytes用户编号 1byte用户类型 1byte用户权限 2bytes指纹头模板编号 4bytes起始时间 4bytes结束时间)
  * 密码用户长度 每个用户信息16bytes( 2bytes用户编号 1byte用户类型 1byte用户权限|密码长度 4bytes密码数据 4bytes起始时间 4bytes结束时间) 
  * IC卡用户长度 每个用户信息15bytes (2bytes用户编号 1byte用户类型 1byte用户权限 4bytes卡号 4bytes起始时间 4bytes结束时间) 
  * 身份证用户长度 每个用户信息19bytes (2bytes用户编号 1byte用户类型 1byte用户权限 8bytes卡号 4bytes起始时间 4bytes结束时间)
  */
#define FINGER_USER_LENTH               14                                      //指纹用户长度
#define CARD_USER_LENTH                 20                                      //卡用户长度
#define	PASSWOD_USER_LENTH		16                                      //密码用户长度

/* 用户类型 */
typedef enum
{
    USER_Type_Finger = 1,               //指纹用户
    USER_Type_Card,                     //卡用户
    USER_Type_Password,                 //密码用户
    USER_Type_Coercion,                 //胁迫用户
    USER_Type_Nanny                     //保姆用户
}USER_Type_TypeDef;
/* 用户权限 */
typedef enum
{
    USER_Power_Ordinary = 0x00,         //普通用户
    USER_Power_Guest = 0x10,            //宾客用户
    USER_Power_Manage = 0x20,           //管理用户
    USER_Power_Coercion = 0x30,         //胁迫用户
    USER_Power_Nanny = 0x40,            //保姆用户
    USER_Power_Dynamic = 0x50,          //动态密码用户
    USER_Power_Factory = 0x60           //出厂用户
}USER_Power_TypeDef;
/* 开门方式 */
typedef enum
{
    OPEN_Door_Factory = 0,              //出厂状态下开门
    OPEN_Door_Finger,                   //指纹开门
    OPEN_Door_Card,                     //卡开门
    OPEN_Door_Password,                 //密码开门
    OPEN_Door_Dynamicf,                 //动态密码开门
    OPEN_Door_remote                    //远程开门
}OPEN_Door_TypeDef;
/******************************************************************************/
void clearSaveAllUser(void);//清空所有数据
uint16_t searchNonCode(void);//自动生成code
uint16_t searchInnerCode(void);//自动生成指纹code
uint8_t saveUserInfo(uint8_t * DataBuff);//存储用户数据
uint8_t findFingerUser(uint16_t code);//指纹比对
uint8_t findCardUser(uint8_t *cardNo);//卡比对
uint8_t findPasswordUser(uint8_t *PWD,uint8_t length);//密码比对
uint8_t getTotalFingerCounter(void);//获取指纹用户数
uint8_t getTotalCardCounter(void);//获取卡用户数
uint8_t getTotalPasswordCounter(void);//获取密码用户数
uint8_t ChangeDesignatedAddress(uint8_t address,uint8_t data);//指定地址数据更改
uint8_t ChangeIntDesignatedAddress(uint8_t address,uint16_t data);////指定地址数据更改
uint8_t getDesignatedAddress(uint8_t address);//获取指定地址数据
void saveDoordata(uint8_t openType);
void readDoordata(uint16_t count,uint8_t *buff);
void clearSaveAllpwd(void);//清空密码
/*******************************************************************************/	
#endif 
