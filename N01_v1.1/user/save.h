#ifndef __SAVE_H_
#define __SAVE_H_
/******************************************************************************/
#include "stm8l15x.h"
/******************************************************************************/
#define DATA_MEMORY_START_ADDR (FLASH_PROGRAM_START_PHYSICAL_ADDRESS + 0xE000)
#define DATA_MEMORY_STOP_ADDR  (FLASH_PROGRAM_END_PHYSICAL_ADDRESS)

#define ADJUST_DATA_STORE_ADDR          0x16000                                 //��56k��ʼ������,8k�û��洢��(0x16000 - 0x17FFF)
/******************************************************************************/
#define SAVE_Addr_FingerCounter  	8					//ָ������ŵ�ַ

#define	SAVE_Addr_Card_count		10				        //IC������ŵ�ַ

#define	SAVE_Addr_Password_count	12				        //��������ŵ�ַ

#define SAVE_Addr_ManagerCounter        14				        //�����ŵ�ַ

#define SAVE_Addr_FamilyCounter  	16				        //���˴�ŵ�ַ

#define SAVE_Addr_GuestCounter  	18				        //���˴�ŵ�ַ

#define SAVE_Addr_doorcount	        20				        //��������

#define SAVE_ADDR_USER_OFFSE            30      	                        //�û����ݴ�ŵ�ַ
/******************************************************************************/
#define SAVE_FINGER_DATA_START		100	                                //ָ�����ݴ洢����ʼλ��   100+100*14=1500

#define SAVE_CARD_DATA_START		(SAVE_FINGER_DATA_START + MAXIMAL_FINGER_COUNTER * FINGER_USER_LENTH)      //�������ݴ洢����ʼλ��   1500+10*16=1660

#define SAVE_PASSWOD_DATA_START		(SAVE_CARD_DATA_START + MAX_USER_CARD_COUNT * CARD_USER_LENTH)      //�����ݴ洢����ʼλ��    1660+50*19=2610
/******************************************************************************/
#define SAVE_Addr_doordata              2650			                //��������
/* ������� */ 
#define MAXIMAL_FINGER_COUNTER		100					//���ָ����

#define	MAX_USER_CARD_COUNT             50					//�����

#define	MAXIMAL_PASSWORD_COUNTER	10				        //���������

#define MAXIMAL_MANAGER_COUNTER		10					//����������


/** ָ���û����� ÿ���û���Ϣ14bytes (2bytes�û���� 1byte�û����� 1byte�û�Ȩ�� 2bytesָ��ͷģ���� 4bytes��ʼʱ�� 4bytes����ʱ��)
  * �����û����� ÿ���û���Ϣ16bytes( 2bytes�û���� 1byte�û����� 1byte�û�Ȩ��|���볤�� 4bytes�������� 4bytes��ʼʱ�� 4bytes����ʱ��) 
  * IC���û����� ÿ���û���Ϣ15bytes (2bytes�û���� 1byte�û����� 1byte�û�Ȩ�� 4bytes���� 4bytes��ʼʱ�� 4bytes����ʱ��) 
  * ���֤�û����� ÿ���û���Ϣ19bytes (2bytes�û���� 1byte�û����� 1byte�û�Ȩ�� 8bytes���� 4bytes��ʼʱ�� 4bytes����ʱ��)
  */
#define FINGER_USER_LENTH               14                                      //ָ���û�����
#define CARD_USER_LENTH                 20                                      //���û�����
#define	PASSWOD_USER_LENTH		16                                      //�����û�����

/* �û����� */
typedef enum
{
    USER_Type_Finger = 1,               //ָ���û�
    USER_Type_Card,                     //���û�
    USER_Type_Password,                 //�����û�
    USER_Type_Coercion,                 //в���û�
    USER_Type_Nanny                     //��ķ�û�
}USER_Type_TypeDef;
/* �û�Ȩ�� */
typedef enum
{
    USER_Power_Ordinary = 0x00,         //��ͨ�û�
    USER_Power_Guest = 0x10,            //�����û�
    USER_Power_Manage = 0x20,           //�����û�
    USER_Power_Coercion = 0x30,         //в���û�
    USER_Power_Nanny = 0x40,            //��ķ�û�
    USER_Power_Dynamic = 0x50,          //��̬�����û�
    USER_Power_Factory = 0x60           //�����û�
}USER_Power_TypeDef;
/* ���ŷ�ʽ */
typedef enum
{
    OPEN_Door_Factory = 0,              //����״̬�¿���
    OPEN_Door_Finger,                   //ָ�ƿ���
    OPEN_Door_Card,                     //������
    OPEN_Door_Password,                 //���뿪��
    OPEN_Door_Dynamicf,                 //��̬���뿪��
    OPEN_Door_remote                    //Զ�̿���
}OPEN_Door_TypeDef;
/******************************************************************************/
void clearSaveAllUser(void);//�����������
uint16_t searchNonCode(void);//�Զ�����code
uint16_t searchInnerCode(void);//�Զ�����ָ��code
uint8_t saveUserInfo(uint8_t * DataBuff);//�洢�û�����
uint8_t findFingerUser(uint16_t code);//ָ�Ʊȶ�
uint8_t findCardUser(uint8_t *cardNo);//���ȶ�
uint8_t findPasswordUser(uint8_t *PWD,uint8_t length);//����ȶ�
uint8_t getTotalFingerCounter(void);//��ȡָ���û���
uint8_t getTotalCardCounter(void);//��ȡ���û���
uint8_t getTotalPasswordCounter(void);//��ȡ�����û���
uint8_t ChangeDesignatedAddress(uint8_t address,uint8_t data);//ָ����ַ���ݸ���
uint8_t ChangeIntDesignatedAddress(uint8_t address,uint16_t data);////ָ����ַ���ݸ���
uint8_t getDesignatedAddress(uint8_t address);//��ȡָ����ַ����
void saveDoordata(uint8_t openType);
void readDoordata(uint16_t count,uint8_t *buff);
void clearSaveAllpwd(void);//�������
/*******************************************************************************/	
#endif 
