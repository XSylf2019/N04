#include "sky1311_drv.h"
#include "sky1311t.h"
#include "iso14443.h"
#include "userdef.h"
#include "string.h"
#if 0
/* 按照字符串输出10进制数 */
void Dec2Str(uint32_t DecNum, uint8_t *DecStr, uint8_t *size)
{
    uint8_t temp[22];
    uint8_t i=0;
    *size = 0;
    do{
        temp[i++] = (uint8_t)(DecNum%10)+0x30;
        DecNum/=10;

    }while(DecNum>0);
    *size = i;

    for(i=0;i<*size;i++)
        DecStr[i] = temp[(*size)-i-1];

}
#endif


/*****************************************************************************/
/*****************************************************************************/
/***************************************************************
    M1卡读取扇区操作示例
   读取扇区数据并通过串口打印出来
   寻卡等操作时，显示操作结果
   有错误时显示错误信息
****************************************************************/
sta_result_t mifare1_ReadTest(uint8_t *UID)
{
    sta_result_t sta;
    uint8_t tmpBuf[20];
    uint8_t default_key[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t sector_num=0, block_num=0;

#if DEBUG==1
    uart_puts("\r\nRead Mifare1 test\r\n");
#endif
    /* read test Mifare 1K card (0--63 block) */
    for(sector_num=0;sector_num<16;sector_num++)// read sector0--16
    //for(block_num=0;block_num<64;) // read block0--63
    {
        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key , UID, block_num);

        if(Ok == sta)
        {
            /*  read 4 block from 1 sector */
            for(uint8_t i=0;i<4;i++)
            {
                sta = M1_Read(block_num, tmpBuf);
                if(sta==Ok)
                {
#if DEBUG==1
                    uart_puts("Block ");
                    uart_putHex(block_num);
                    uart_puts(" : ");
                    uart_printBuffer(tmpBuf,16);
                    uart_newrow();
#endif
                    block_num++;

                }
                else{
#if DEBUG==1
                    uart_puts("Block 0x");
                    uart_putHex(block_num);
                    uart_puts("Reader Error\r\n");
#endif
                    //sky1311WriteReg(ADDR_M1_CTRL, 0x00);
                    //return sta;
                }
            }
        }
        else
        {
#if DEBUG==1
            uart_puts("Sector 0x");
            uart_putHex(sector_num);
            uart_puts("Authentication Error\r\n");
#endif
            sky1311WriteReg(ADDR_M1_CTRL, 0x00);
            return sta;
        }



#if DEBUG==1
        uart_newrow();
#endif
        sky1311WriteReg(ADDR_M1_CTRL, 0x81);    // next Authentication
    }
#if 0
    /* read test Mifare 1K card (64--127 block) */
    //for(sector_num=16;sector_num<32;sector_num++)// read sector16--31  (block64--127 )
    for(block_num=64;block_num<128;)
    {
        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, block_num);
        if(Ok == sta)
        {
            for(uint8_t i=0;i<4;i++)    /* 4 blocks/sector */
            {
                sta = M1_Read(block_num, tmpBuf);
                if(sta==Ok)
                {
#if DEBUG==1
                    uart_puts("Block ");
                    uart_putHex(block_num);
                    uart_puts(" : ");
                    uart_printBuffer(tmpBuf,16);
                    uart_newrow();
#endif
                    block_num++;
                }
                else
                {
                    sky1311WriteReg(ADDR_M1_CTRL, 0x00);
                    return sta;

                }
            }
        }
        else{
#if DEBUG==1
            uart_puts("Block 0x");
            uart_putHex(block_num);
            uart_puts("Authentication Error\r\n");
#endif

            piccHaltA();
            DelayMS(5);
            sta = piccWakeupA(tmpBuf);
            if(Ok == sta)
            {
                sta = piccSelectA(SEL1,UID, tmpBuf);
                if(Ok != sta){
#if DEBUG==1
                    uart_puts("PICC Select Error!\r\n");
#endif
                }
            }
            else
            {
#if DEBUG==1
                uart_puts("PICC Request Error! @ ");
                uart_puts("Block 0x");
                uart_putHex(block_num);
#endif
            }
            //sky1311WriteReg(ADDR_M1_CTRL, 0x00);
            //return sta;
        }
#if DEBUG==1
        uart_newrow();
#endif
        sky1311WriteReg(ADDR_M1_CTRL, 0x81);

    }

    /* read test Mifare 4K card (128--255 block) */
    //for(sector_num=32;sector_num<40;sector_num++)// read sector32--39  (block128--255 )
    for(block_num=128;block_num<255;)
    {
        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, block_num);
        if(Ok == sta)
        {
            for(uint8_t i=0;i<16;i++)       /* 16 blocks/sector */
            {
                sta = M1_Read(block_num, tmpBuf);
                if(sta==Ok)
                {
#if DEBUG==1
                    uart_puts("Block ");
                    uart_putHex(block_num);
                    uart_puts(" : ");
                    uart_printBuffer(tmpBuf,16);
                    uart_newrow();
#endif
                    if(block_num == 255)
                    {
                        sky1311WriteReg(ADDR_M1_CTRL, 0x00);
                        return Ok;
                    }
                    else
                    {
                        block_num++;
                    }
                }
                else
                {
                    sky1311WriteReg(ADDR_M1_CTRL, 0x00);
                    return sta;
                }
            }
        }
        else{
#if DEBUG==1
            uart_puts("Block 0x");
            uart_putHex(block_num);
            uart_puts("Authentication Error\r\n");
#endif

            piccHaltA();
            sky1311WriteReg(ADDR_M1_CTRL, 0x00);
            DelayMS(5);
            sta = piccWakeupA(tmpBuf);
            if(Ok == sta)
            {
                sta = piccSelectA(SEL1,UID, tmpBuf);
                if(Ok != sta){
#if DEBUG==1
                    uart_puts("PICC Select Error!\r\n");
                    return Ok
#endif
                }
            }
            else
            {
#if DEBUG==1
                uart_puts("PICC Request Error! @ ");
                uart_puts("Block 0x");
                uart_putHex(block_num);
                return Ok;
#endif
            }
            //sky1311WriteReg(ADDR_M1_CTRL, 0x00);
            //return Ok;
        }
#if DEBUG==1
        uart_newrow();
#endif
        sky1311WriteReg(ADDR_M1_CTRL, 0x81);
    }
#endif
    //sky1311Reset();
    return Ok;
}

/***************************************************************
    M1卡写入操作示例
    M1卡写入Block4 (Sector 1) 和Block8（Sector2）操作。
   寻卡等操作时，显示操作结果
   有错误时显示错误信息
****************************************************************/
sta_result_t mifare1_WriteTest(uint8_t *UID)
{
    sta_result_t sta;
    uint8_t default_key[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t i;
    uint8_t writeData[] = "ABCDEF1234567890";
    uint8_t keyb[]={1,2,3,4,5,6};
    uint8_t keya[]={0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF };

#if DEBUG==1
    uart_puts("\r\nWrite Mifare1 test\r\n");
#endif

    sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, 7);
    if(Ok==sta)
    {
        sta = M1_PwdChange(7,keya,keyb);
        if(Ok == sta)
        {
            sky1311WriteReg(ADDR_M1_CTRL, 0x81);
        }
    }
    else
    {
        piccHaltA();
        sky1311WriteReg(ADDR_M1_CTRL, 0x00);
        return Error;

    }
    for(i=0;i<3;i++)
    {
        irq_sta = sky1311ReadReg(ADDR_M1_CTRL);

        uint8_t block_num = 4+i;
        sta = M1_Write(block_num, writeData);
#if DEBUG==1
        uart_puts("Block write ");
        uart_putHex(block_num);
#endif
        if(Ok == sta)
        {
#if DEBUG==1
            uart_puts("Success\r\n");
#endif
        }
        else{
#if DEBUG==1
            uart_puts("Failed\r\n");
#endif
        }

    }
    return Ok;
}

/***************************************************************
    CPU卡操作示例
   读取CPU卡的UID等，并使用部分APDU做测试
   寻卡等操作时，显示操作结果
   有错误时显示错误信息
****************************************************************/
sta_result_t smartTypeA_test(void)
{
    sta_result_t sta;
    uint8_t tmpBuf[100];
    uint16_t tmpSize;

#if DEBUG==1
    uart_puts("\r\nSmart Type A test\r\n");
#endif
    sta = piccATS(tmpBuf);
    if(sta == NoResponse){
        sky1311Reset();
#if DEBUG==1
        uart_puts("\r\nRATS error!\r\n");
#endif
        return ErrorAts;
    }
    else{
#if DEBUG==1
    	uart_puts("RATS : ");
    	uart_printBuffer(tmpBuf,32);
        uart_newrow();
#endif
    }
#if 0
    /* 00 a4 04 00 00 */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;   // command: select file
    tmpBuf[3] = 0x04;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x00;

    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("Select SD:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
#endif
    }
#endif

     /* select MF file */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;   // command: select file
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x02;
    tmpBuf[6] = 0x3F;
    tmpBuf[7] = 0x00;   // MF
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,8,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("Select MF:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
#endif
    }
#if 0
    /*获取随机数*/
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0x84;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x08;
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("RM:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
#endif
    }
#endif

#if 0
    /* 读一卡通有关的信息 */
    tmpBuf[0] = 0x03;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x02;
    tmpBuf[6] = 0x10;
    tmpBuf[7] = 0x01;

    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,8,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("Read Card Info:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
        uart_puts("Card No: ");
        DecNum = (uint32_t) tmpBuf[40]<<24;
        DecNum += (uint32_t) tmpBuf[39]<<16;
        DecNum += (uint32_t) tmpBuf[38]<<8;
        DecNum += (uint32_t) tmpBuf[37];

        Dec2Str(DecNum,tmpBuf,&DecLength);
        uart_sendData(tmpBuf,DecLength);
        uart_newrow();

#endif
    }
    /* 读余额 */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x80;
    tmpBuf[2] = 0x5C;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x02;
    tmpBuf[5] = 0x04;

    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("Read Balance:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
        uart_puts("Balance: ￥");
        DecNum = (uint32_t) (tmpBuf[1]&0x0f)<<24;
        DecNum += (uint32_t) tmpBuf[2]<<16;
        DecNum += (uint32_t) tmpBuf[3]<<8;
        DecNum += (uint32_t) tmpBuf[4];
        uint8_t decimal = DecNum%100;
        DecNum = DecNum/100;
        Dec2Str(DecNum,tmpBuf,&DecLength);
        uart_sendData(tmpBuf,DecLength);
        uart_putChar('.');
        Dec2Str(decimal,tmpBuf,&DecLength);
        uart_sendData(tmpBuf,DecLength);
        uart_newrow();

#endif
    }

    for(i=1;i<=10;i++)
    {
        /* 读取交易记录 */
        tmpBuf[0] = 0x02;
        tmpBuf[1] = 0x00;   // CLA Class
        tmpBuf[2] = 0xB2;   // INS Instruction
        tmpBuf[3] = i;//0x01;   // P1 Parameter 1
        tmpBuf[4] = 0xC4;//0xC5;   // P2 Parameter 2
        tmpBuf[5] = 0x00;   // Le

        sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
        if(sta == NoResponse)
        {
            return NoResponse;
        }
        else{
    #if DEBUG==1
            uart_puts("Read Record:");
            uart_printBuffer(tmpBuf,tmpSize-2);
            uart_newrow();
    #endif
        }
    }
#endif //if 0
    sky1311Reset();
    //LED_OFF();
    return Ok;
}
/******************************************************************************/
/*  Type A PICC test */
sta_result_t TypeA_test(uint8_t *TypeA_data, uint8_t *TypeA_uid_len)
{
    sta_result_t sta;
    uint8_t SAK=0;
    uint8_t ATQA[2]={0};
    uint8_t UID[12]={0};
    uint8_t *uid;
    uint8_t sel=SEL1;
    uint8_t uidSize=0;
    uint8_t tmpBuf[10];
    uint8_t i;
    SKY1311_ENABLE(); 	    // chip enable PD2
    sky1311Init();			// init sky1311s
    //resetPicc();            // 复位卡片()
    delay_ms(10);             // 等待卡片工作
    typeAOperate();
    //sta = piccRequestA(tmpBuf);     // send REQA command to card
    sta = piccWakeupA(tmpBuf);        // send WUPA command to card
    if(Ok == sta)
    {
//        LED_ON();
        ATQA[0] = tmpBuf[1];
        ATQA[1] = tmpBuf[0];
        uidSize = tmpBuf[0]>>6;
    }
    else
    {
        sky1311Reset();
        return ErrorRequest;            // 这里返回，有可能是没卡或者没读到
    }
    uid = &UID[0];
    sel = SEL1;
    do{
        sta = piccAntiA(sel, 1, uid);   // 防碰撞循环，测试获得一张卡的UID，参数'1'表示遇到冲突时选择1
        if(Ok == sta)
        {
            uint8_t checkSum = uid[0] ^ uid[1] ^ uid[2] ^ uid[3];
            if(checkSum != uid[4])
            {
                sky1311Reset();
                return ErrorAnticollision;
            }
            memcpy(TypeA_data, uid, 4);
            *TypeA_uid_len = 4;
#if DEBUG==1
            //uart_puts("UID:");
            //uart_printBuffer(UID,4);
            //uart_newrow();
#endif
        }
        else
        {
            sky1311Reset();

            return ErrorAnticollision;
        }
        sta = piccSelectA(sel,uid, tmpBuf);        // Select PICC
        if(Ok == sta)
        {
            SAK = tmpBuf[0];
#if DEBUG==1
            //uart_puts("SAK:");
            //uart_printBuffer(tmpBuf,1);
#endif
        }
        else
        {
            return ErrorSelect;
        }


        if(uidSize!=0 && uid[0] == 0x88)
        {
            for(i=0;i<3;i++){
                uid[i] = uid[i+1];
            }
            uid += 3;
            sel += 2;

        }
    }while( (SAK&0x04)!=0 );

#if CARDTYPE==AUTO

    if((SAK & 0x24) == 0x20){           //UID complete, PICC compliant with ISO/IEC 14443-4
        sta = smartTypeA_test();
    }
    else if((SAK==0x08 || SAK == 0x18) && ((ATQA[1]&0xB4) == 0x04 || (ATQA[1]&0xB2) == 0x02)){// M1 card
        //sta = mifare1_WriteTest(UID);
//        sta = mifare1_ReadTest(UID);//不需要对卡操作，需要屏蔽该函数，否则显示读卡错误
    }
#elif CARDTYPE==ONLYM1
    sta = mifare1_ReadTest(UID);
#elif CARDTYPE==ONLYSMART
    sta = smartTypeA_test();
#elif CARDTYPE==SKY1311TEST
    sta = sky1311_ReadTest();
#endif
    sky1311Reset();
    return sta;
}

/***************************************************************
    TYPE B卡操作示例 （以读身份证串号为例）
   读取TYPE B卡的UID等，并使用部分APDU做测试
   寻卡等操作时，显示操作结果
   有错误时显示错误信息
****************************************************************/
sta_result_t TypeB_test(uint8_t *TypeB_data, uint8_t *TypeB_uid_len)
{
    sta_result_t sta;

    nAQTB_t ATQB;
    uint8_t tmpBuf[100];
    uint16_t tmpSize;
    uint8_t  attSize;

    SKY1311_ENABLE(); 	    // chip enable PD2
    //DelayMS(5);
    sky1311Init();			// init sky1311s
    //resetPicc();            // 复位卡片
    delay_ms(10);             // 等待卡片工作
    typeBOperate();

    sta = piccRequestB(REQALL,0, 0, &ATQB);
    if(Ok != sta)
    {
        sky1311Reset();
        return NoResponse;
    }
//    uart_puts("\r\nType-B Card test\r\n");
//    LED_ON();
    sta = piccAttrib(ATQB.PUPI, 0, 8, 0, 1, tmpBuf, 0, tmpBuf, &attSize);
    if(Ok != sta)
    {
        sky1311Reset();
        return NoResponse;
    }

    /* 读身份证ID  ISO14443-4 */
    tmpBuf[0] = 0x00;
    tmpBuf[1] = 0x36;
    tmpBuf[2] = 0x00;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x08;
    //sky1311_fifo_tx(TYPE_B, tmpBuf, 5);
    //sta = sky1311_fifo_rx(TYPE_B, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_B,RATE_OFF,tmpBuf,5,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
			if(tmpBuf[tmpSize-4] == 0x90 && tmpBuf[tmpSize-3] == 0x00)
			{
				memcpy(TypeB_data, tmpBuf, tmpSize-4);
				*TypeB_uid_len = tmpSize-4;
			}
//        uart_puts("ID Card:");
//        uart_printBuffer(tmpBuf,tmpSize-2);
//        uart_newrow();
    }
    sky1311Reset();
    return Ok;
}
