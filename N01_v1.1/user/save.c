#include "save.h"
#include "stm8l15x_flash.h"
#include "string.h"
#include "DynamicPassword.h"
/******************************************************************************/
uint8_t OpenType;//���ŷ�ʽ 0����1ָ��2��3����4��̬
uint8_t UserPower;//�û�Ȩ�� 0x20����Ա 0x00��ͨ 0x10���� 0x30в�� 0x40��ķ 0x50��̬���� 0x60����
uint8_t UserType;//�û����� 1ָ�� 2�� 3����
uint16_t IDuserCode;//�û����
/******************************************************************************/
void Flash_WriteDataBlock(uint8_t block_count, uint8_t *Buffer)
{
    /* Unlocks the program memory */
    FLASH_Unlock(FLASH_MemType_Program);
    /* Flash Program memory unlocked flag */
    while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET);
    /* Programs a memory block */
    FLASH_ProgramBlock(block_count, FLASH_MemType_Program, FLASH_ProgramMode_Standard, Buffer);
    /* End of programming (write or erase operation) flag */
    while (FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET);
    /* Locks the program memory */
    FLASH_Lock(FLASH_MemType_Program);
}
/******************************************************************************/
//���⺯����u16��Ϊu32�����򵱵�ַ����10000Hʱ����
void FLASH_ProgramByte_User(uint32_t Address, uint8_t Data)		
{
    /* Check parameters */
    assert_param(IS_FLASH_ADDRESS_OK(Address));
    *(PointerAttr uint8_t*) (uint32_t)Address = Data;
}
/******************************************************************************/
//���⺯����u16��Ϊu32�����򵱵�ַ����10000Hʱ����
uint8_t FLASH_ReadByte_User(uint32_t Address)
{
  /* Read byte */
  return(*(PointerAttr uint8_t *) (uint32_t)Address);
}
/******************************************************************************/
void FLASH_EraseByte_User(uint32_t Address)
{
  /* Check parameter */
  assert_param(IS_FLASH_ADDRESS(Address));

  *(PointerAttr uint8_t*) (uint32_t)Address = 0x00; /* Erase byte */
}
/******************************************************************************/
uint8_t  Flash_Erase_bytes(uint32_t Address , uint16_t length)
{
    uint16_t i=0;
    
    // Խ���ж�
    if((Address < DATA_MEMORY_START_ADDR)||(Address+ length > DATA_MEMORY_STOP_ADDR))
    {
        return FALSE;
    }
    
    /* Unlocks the program memory */
    FLASH_Unlock(FLASH_MemType_Program);
    /* Flash Program memory unlocked flag */
    while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET);
    /* Programs a memory block */
    for(i = 0;i < length;i ++)
    {
//        FLASH_EraseByte_User(Address+i);
        FLASH_ProgramByte_User(Address+i,0x00);
        
        FLASH_WaitForLastOperation(FLASH_MemType_Program);
    }
    /* Locks the program memory */
    FLASH_Lock(FLASH_MemType_Program);
    
    return length;
}
/******************************************************************************/
uint8_t  Flash_Write_bytes(uint32_t Address , uint8_t * DataBuff,uint8_t length)
{
    uint8_t i=0;
    
    // Խ���ж�
    if((Address < DATA_MEMORY_START_ADDR)||(Address+ length > DATA_MEMORY_STOP_ADDR))
    {
        return FALSE;
    }
    
    /* Unlocks the program memory */
    FLASH_Unlock(FLASH_MemType_Program);
    /* Flash Program memory unlocked flag */
    while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET);
    /* Programs a memory block */
    for(i = 0;i < length;i ++)
    {
        FLASH_ProgramByte_User(Address+i,DataBuff[i]);	
        
        FLASH_WaitForLastOperation(FLASH_MemType_Program);
    }
    /* Locks the program memory */
    FLASH_Lock(FLASH_MemType_Program);
    
    return length;
}
/******************************************************************************/
uint8_t  Flash_Read_bytes(uint32_t Address , uint8_t * DataBuff,uint8_t length)
{
    uint8_t i=0;
    
    // Խ���ж�
    if((Address < DATA_MEMORY_START_ADDR)||(Address+ length > DATA_MEMORY_STOP_ADDR))
    {
        return FALSE;
    }
    
    /* Unlocks the program memory */
    FLASH_Unlock(FLASH_MemType_Program);
    /* Flash Program memory unlocked flag */
    while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET);
    /* Programs a memory block */
    for(i = 0;i < length;i ++)
    {
        *(DataBuff+i)=FLASH_ReadByte_User(Address+i);	
        
        FLASH_WaitForLastOperation(FLASH_MemType_Program);
    }
    /* Locks the program memory */
    FLASH_Lock(FLASH_MemType_Program);
    
    return length;
}
/******************************************************************************/
uint8_t ChangeDesignatedAddress(uint8_t address,uint8_t data)
{
    uint8_t DataBuff[5] = {0};
    
    disableInterrupts();//�ر��ж�
    Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + address , DataBuff,1);
      
    DataBuff[0] = data;
    
    Flash_Write_bytes(ADJUST_DATA_STORE_ADDR + address , DataBuff , 1);
    enableInterrupts();//ʹ���ж�
    return 0;
}
/******************************************************************************/
uint8_t ChangeIntDesignatedAddress(uint8_t address,uint16_t data)
{
    uint8_t DataBuff[5] = {0};
    
    disableInterrupts();//�ر��ж�
    Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + address , DataBuff,2);
    
    DataBuff[0] = data;
    DataBuff[1] = data >> 8;
    
    Flash_Write_bytes(ADJUST_DATA_STORE_ADDR + address , DataBuff , 2);
    enableInterrupts();//ʹ���ж�
    return 0;
}
/******************************************************************************/
uint8_t ChangeDesignatedAddressplusOne(uint8_t address)//ָ����ַ���ݼ�1
{
    uint8_t DataBuff[5] = {0};
    
    Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + address , DataBuff,1);
    
    DataBuff[0] += 1;
    
    Flash_Write_bytes(ADJUST_DATA_STORE_ADDR + address , DataBuff , 1);
    return 0;
}
/******************************************************************************/
void clearSaveAllUser(void)//�����������
{
    disableInterrupts();//�ر��ж�
    
    Flash_Erase_bytes((ADJUST_DATA_STORE_ADDR) , 512);
    enableInterrupts();//ʹ���ж�
}
/******************************************************************************/
uint8_t saveUserInfo(uint8_t * DataBuff)//�洢�û�����
{
    uint8_t i;
    uint8_t buff[5] = {0};
    uint16_t p;

    disableInterrupts();//�ر��ж�
    switch(DataBuff[2])
    {
        case USER_Type_Finger:
          for(i = 0;i < MAXIMAL_FINGER_COUNTER;i ++)
          {
              p = SAVE_FINGER_DATA_START + i * FINGER_USER_LENTH;
              Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , buff,4);
              if(buff[0] == 0xff)
              {
                  Flash_Write_bytes(ADJUST_DATA_STORE_ADDR + p , DataBuff , FINGER_USER_LENTH);
                  ChangeDesignatedAddressplusOne(SAVE_Addr_FingerCounter);//ָ������1
                  if((DataBuff[3] & 0xf0) == 0x20)//����
                  {
                      ChangeDesignatedAddressplusOne(SAVE_Addr_ManagerCounter);//��������1
                  }
              }
          }
          break;
          
        case USER_Type_Card:
          for(i = 0;i < MAX_USER_CARD_COUNT;i ++)
          {
              p = SAVE_CARD_DATA_START + i * CARD_USER_LENTH;
              Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , buff,4);
              if(buff[0] == 0xff)
              {
                  Flash_Write_bytes(ADJUST_DATA_STORE_ADDR + p , DataBuff , CARD_USER_LENTH);
                  ChangeDesignatedAddressplusOne(SAVE_Addr_Card_count);//������1
                  if((DataBuff[3] & 0xf0) == 0x20)//����
                  {
                      ChangeDesignatedAddressplusOne(SAVE_Addr_ManagerCounter);//��������1
                  }
              }
          }
          break;
          
        case USER_Type_Password:
          for(i = 0;i < MAXIMAL_PASSWORD_COUNTER;i ++)
          {
              p = SAVE_PASSWOD_DATA_START + i * PASSWOD_USER_LENTH;
              Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , buff,4);
              if(buff[0] == 0xff)
              {
                  Flash_Write_bytes(ADJUST_DATA_STORE_ADDR + p , DataBuff , PASSWOD_USER_LENTH);
                  ChangeDesignatedAddressplusOne(SAVE_Addr_Password_count);//��������1
                  if((DataBuff[3] & 0xf0) == 0x20)//����
                  {
                      ChangeDesignatedAddressplusOne(SAVE_Addr_ManagerCounter);//��������1
                  }
              }
          }
          break;
    }
    enableInterrupts();//ʹ���ж�
    return 0;
}
/******************************************************************************/
uint8_t verifySameUserCode(uint16_t code)//�ȶ��Ƿ�Ĳ�ͬ���û����
{
    uint8_t i;
    uint8_t buff[5] = {0};
    uint16_t p,uid;

    disableInterrupts();//�ر��ж�
    for(i = 0;i < MAXIMAL_FINGER_COUNTER;i ++)//�������û�����
    {
        p = SAVE_FINGER_DATA_START + i * FINGER_USER_LENTH;
        Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , buff,4);
        uid = buff[0];
        uid |= buff[1] << 8;
        if(code == uid)
        {
          enableInterrupts();//ʹ���ж�
          return 1;
        }
    }

    for(i = 0;i < MAX_USER_CARD_COUNT;i ++)//�������û�����
    {
        p = SAVE_CARD_DATA_START + i * CARD_USER_LENTH;
        Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , buff,4);
        uid = buff[0];
        uid |= buff[1] << 8;
        if(code == uid)
        {
          enableInterrupts();//ʹ���ж�
          return 1;
        }
    }

    for(i = 0;i < MAXIMAL_PASSWORD_COUNTER;i ++)//�������û�����
    {
        p = SAVE_PASSWOD_DATA_START + i * PASSWOD_USER_LENTH;
        Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , buff,4);
        uid = buff[0];
        uid |= buff[1] << 8;
        if(code == uid)
        {
          enableInterrupts();//ʹ���ж�
          return 1;
        }
    }
    enableInterrupts();//ʹ���ж�
    return 0;
}
/******************************************************************************/
uint8_t verifySameFingerCode(uint16_t code)//�ȶ�ָ���Ƿ�Ĳ�ͬ���ڲ����
{
    uint8_t i;
    uint8_t buff[FINGER_USER_LENTH] = {0};
    uint16_t p,uid;

    disableInterrupts();//�ر��ж�
    for(i = 0;i < MAXIMAL_FINGER_COUNTER;i ++)//�������û�����
    {
        p = SAVE_FINGER_DATA_START + i * FINGER_USER_LENTH;
        Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , buff,FINGER_USER_LENTH);
        uid = buff[4];
        uid |= buff[5] << 8;
        if(code == uid)
        {
          enableInterrupts();//ʹ���ж�
          return 1;
        }
    }
    enableInterrupts();//ʹ���ж�
    return 0;
}
/******************************************************************************/
uint16_t searchNonCode(void)//�Զ�����code
{
    uint16_t code,p;
    
    code = 1;
    for(p = 0;p < (MAXIMAL_FINGER_COUNTER + MAX_USER_CARD_COUNT + MAXIMAL_PASSWORD_COUNTER);p ++)//��˳��� ��0��ʼ
    {
        if(verifySameUserCode(code))//��ͬ��
        {
            code ++;
            p = 0xffff;
            continue;
        }
    }
    return code;
}
/******************************************************************************/
uint16_t searchInnerCode(void)//�Զ�����ָ���ڲ�code
{
    uint16_t p,code = 0;
    
    for(p = 0;p < MAXIMAL_FINGER_COUNTER;p ++)//����ָ��
    {
        if(verifySameFingerCode(code))//��ͬ��
        {
            code ++;
            p = 0xffff;
            continue;
        }
    }
    return code;
}
/******************************************************************************/
uint8_t findFingerUser(uint16_t code)//ָ�Ʊȶ�
{
    uint8_t i;
    uint8_t buff[FINGER_USER_LENTH] = {0};
    uint16_t p,uid;
    uint32_t lSecstart=0,lSecstop=0;

    disableInterrupts();//�ر��ж�
    for(i = 0;i < MAXIMAL_FINGER_COUNTER;i ++)//�������û�����
    {
        p = SAVE_FINGER_DATA_START + i * FINGER_USER_LENTH;
        Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , buff,FINGER_USER_LENTH);
        uid = buff[4];
        uid |= buff[5] << 8;
        if(code == uid)
        {
            lSecstart = (uint32_t)buff[6] << 24;
            lSecstart |= (uint32_t)buff[7] << 16;
            lSecstart |= (uint32_t)buff[8] << 8;
            lSecstart |= (uint32_t)buff[9];
            
            lSecstop = (uint32_t)buff[10] << 24;
            lSecstop |= (uint32_t)buff[11] << 16;
            lSecstop |= (uint32_t)buff[12] << 8;
            lSecstop |= (uint32_t)buff[13];
            if(lSecstart <= GetRealSecondTime() && lSecstop >= GetRealSecondTime())
            {
                memcpy(&IDuserCode,&buff[0],2);
                UserType = USER_Type_Finger;//�û�����
                UserPower = buff[3] & 0xf0;//�û�Ȩ��
                OpenType = OPEN_Door_Finger;//���ŷ�ʽ
                enableInterrupts();//ʹ���ж�
                return 1;
            }
        }
    }
    enableInterrupts();//ʹ���ж�
    return 0;
}
/******************************************************************************/
uint8_t findCardUser(uint8_t *cardNo)//���ȶ�
{
    uint8_t i;
    uint8_t buff[CARD_USER_LENTH] = {0};
    uint16_t p;
    uint32_t lSecstart=0,lSecstop=0;
    
    disableInterrupts();//�ر��ж�
    for(i = 0;i < MAX_USER_CARD_COUNT;i ++)//�������û�����
    {
        p = SAVE_CARD_DATA_START + i * CARD_USER_LENTH;
        Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , buff,CARD_USER_LENTH);

        if(buff[4] == cardNo[0] && buff[5] == cardNo[1] && buff[6] == cardNo[2] && buff[7] == cardNo[3] &&
           buff[8] == cardNo[4] && buff[9] == cardNo[5] && buff[10] == cardNo[6] && buff[11] == cardNo[7])
        {
            lSecstart = (uint32_t)buff[6] << 24;
            lSecstart |= (uint32_t)buff[7] << 16;
            lSecstart |= (uint32_t)buff[8] << 8;
            lSecstart |= (uint32_t)buff[9];
            
            lSecstop = (uint32_t)buff[10] << 24;
            lSecstop |= (uint32_t)buff[11] << 16;
            lSecstop |= (uint32_t)buff[12] << 8;
            lSecstop |= (uint32_t)buff[13];
            if(lSecstart <= GetRealSecondTime() && lSecstop >= GetRealSecondTime())
            {
                memcpy(&IDuserCode,&buff[0],2);
                UserType = USER_Type_Card;//�û�����
                UserPower = buff[3] & 0xf0;//�û�Ȩ��
                OpenType = OPEN_Door_Card;//���ŷ�ʽ
                enableInterrupts();//ʹ���ж�
                return 1;
            }
        }
    }
    enableInterrupts();//ʹ���ж�
    return 0;
}
/******************************************************************************/
uint8_t findPasswordUser(uint8_t *PWD,uint8_t length)//����ȶ�
{
    uint8_t i,flag,offset,n;
    uint8_t buff[8] = {0};
    uint8_t DataBuff[PASSWOD_USER_LENTH] = {0};
    uint16_t p;
    uint32_t lSecstart=0,lSecstop=0;

    if(getTotalFingerCounter() == 0 && getTotalPasswordCounter() == 0 && getTotalCardCounter() == 0)
    {            
        for(i = 0;i < 6;i ++)
        {
            if(PWD[i] == '1' && PWD[i + 1] == '2' && PWD[i + 2] == '3' && PWD[i + 3] == '4' && PWD[i + 4] == '5' && PWD[i + 5] == '6')
            {
                IDuserCode = 0;
                UserType = USER_Type_Password;//�û�����
                UserPower = USER_Power_Factory;//�û�Ȩ��
                OpenType = OPEN_Door_Password;//���ŷ�ʽ
                return 1;
            }              
        }
    }
    
    if(getTotalPasswordCounter() == 0)return 0;
    
    disableInterrupts();//�ر��ж�
    for(i = 0;i < MAXIMAL_PASSWORD_COUNTER;i ++)
    {
        p = SAVE_PASSWOD_DATA_START + i * PASSWOD_USER_LENTH;
        Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + p , DataBuff,PASSWOD_USER_LENTH);

        if(length == (DataBuff[3] & 0x0f))
        {                     
            for(i = 0;i < 8;i ++)PWD[i] &= 0x0f; 

            buff[0] = DataBuff[4] >> 4;
            buff[1] = DataBuff[4] & 0x0f;
            
            buff[2] = DataBuff[5] >> 4;
            buff[3] = DataBuff[5] & 0x0f;
            
            buff[4] = DataBuff[6] >> 4;
            buff[5] = DataBuff[6] & 0x0f;
            
            buff[6] = DataBuff[7] >> 4;
            buff[7] = DataBuff[7] & 0x0f;    
            
            flag = 0;
            for(i = 0;i < length;i ++)
            {                        
                if(PWD[i] != buff[i])flag = 1;//�������
            }
            if(flag == 0)
            {
                lSecstart = (uint32_t)DataBuff[8] << 24;
                lSecstart |= (uint32_t)DataBuff[9] << 16;
                lSecstart |= (uint32_t)DataBuff[10] << 8;
                lSecstart |= (uint32_t)DataBuff[11];
                
                lSecstop = (uint32_t)DataBuff[12] << 24;
                lSecstop |= (uint32_t)DataBuff[13] << 16;
                lSecstop |= (uint32_t)DataBuff[14] << 8;
                lSecstop |= (uint32_t)DataBuff[15];
                if(lSecstart <= GetRealSecondTime() && lSecstop >= GetRealSecondTime())
                {
                    memcpy(&IDuserCode,&DataBuff[0],2); 
                    UserType = USER_Type_Password;//�û�����
                    UserPower = DataBuff[3] & 0xf0;//�û�Ȩ��
                    OpenType = OPEN_Door_Password;//���ŷ�ʽ
                    enableInterrupts();//ʹ���ж�
                    return 1;
                }
            }
        }
        else
        {
            if(length > 4 &&(length >= (DataBuff[3] & 0x0f)))
            {
                if((DataBuff[3] & 0x0f) < 6)
                {
                  enableInterrupts();//ʹ���ж�
                  return 0;
                }
                for(i = 0;i < 50;i ++)PWD[i] &= 0x0f; 

                buff[0] = DataBuff[4] >> 4;
                buff[1] = DataBuff[4] & 0x0f;
                
                buff[2] = DataBuff[5] >> 4;
                buff[3] = DataBuff[5] & 0x0f;
                
                buff[4] = DataBuff[6] >> 4;
                buff[5] = DataBuff[6] & 0x0f;
                
                buff[6] = DataBuff[7] >> 4;
                buff[7] = DataBuff[7] & 0x0f;    
                
                offset = length - DataBuff[3] & 0x0f;
                for(n = 0;n <= offset;n ++)
                {
                    flag = 0;
                    for(i = 0;i < (DataBuff[3] & 0x0f);i ++)
                    {
                        if(PWD[i + n] != buff[i])flag = 1;//�������
                    }
                    if(flag == 0)
                    {
                        lSecstart = (uint32_t)DataBuff[8] << 24;
                        lSecstart |= (uint32_t)DataBuff[9] << 16;
                        lSecstart |= (uint32_t)DataBuff[10] << 8;
                        lSecstart |= (uint32_t)DataBuff[11];
                        
                        lSecstop = (uint32_t)DataBuff[12] << 24;
                        lSecstop |= (uint32_t)DataBuff[13] << 16;
                        lSecstop |= (uint32_t)DataBuff[14] << 8;
                        lSecstop |= (uint32_t)DataBuff[15];
                        if(lSecstart <= GetRealSecondTime() && lSecstop >= GetRealSecondTime())
                        {
                            memcpy(&IDuserCode,&DataBuff[0],2); 
                            UserType = USER_Type_Password;//�û�����
                            UserPower = DataBuff[3] & 0xf0;//�û�Ȩ��
                            OpenType = OPEN_Door_Password;//���ŷ�ʽ
                            enableInterrupts();//ʹ���ж�
                            return 1;
                        }
                        break;
                    }
                }
            }
        }
    }
    enableInterrupts();//ʹ���ж�
    return 0; 
}
/******************************************************************************/
uint8_t getTotalFingerCounter(void)//��ȡָ���û���
{
    uint8_t buff[5] = {0};
    
    disableInterrupts();//�ر��ж�
    Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + SAVE_Addr_FingerCounter , buff,1);
    enableInterrupts();//ʹ���ж�
    return buff[0];   
}
/******************************************************************************/
uint8_t getTotalCardCounter(void)//��ȡ���û���
{
    uint8_t buff[5] = {0};
    
    disableInterrupts();//�ر��ж�
    Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + SAVE_Addr_Card_count , buff,1);
    enableInterrupts();//ʹ���ж�
    return buff[0]; 
}
/******************************************************************************/
uint8_t getTotalPasswordCounter(void)//��ȡ�����û���
{
    uint8_t buff[5] = {0};
    
    disableInterrupts();//�ر��ж�
    Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + SAVE_Addr_Password_count , buff,1);
    enableInterrupts();//ʹ���ж�
    return buff[0];  
}
/******************************************************************************/
uint8_t getDesignatedAddress(uint8_t address)//��ȡָ����ַ����
{
    uint8_t buff[5] = {0};
    
    disableInterrupts();//�ر��ж�
    Flash_Read_bytes(ADJUST_DATA_STORE_ADDR + address , buff,1);
    enableInterrupts();//ʹ���ж�
    return buff[0];    
}
/******************************************************************************/