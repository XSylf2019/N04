#include "sky1311_drv.h"
#include "sky1311t_reg.h"
/*******************************************************************************
uint8_t SPIByteCom(uint8_t ch)
{
    uint8_t res;
    while((SKY1311_SPI->SR & 0x02) == 0);   // wait for TX buffer empty
    SKY1311_SPI->DR = ch;                  // write to data register
    while((SKY1311_SPI->SR & 0x01) == 0); // wait for RX buffer not empty
    res = SKY1311_SPI->DR ;                // read RX buffer
    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
    return res;
}
0******************************************************************************/
uint8_t SPIByteCom(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SKY1311_SPI, SPI_FLAG_TXE) == RESET);
  /* Send byte through the SPI1 peripheral */
  SPI_SendData(SKY1311_SPI, byte);
  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SKY1311_SPI, SPI_FLAG_RXNE) == RESET);
  /* Return the byte read from the SPI bus */
  return SPI_ReceiveData(SKY1311_SPI);	 
}
 /******************************************************************************
 ** \简  述  通过SPI接口向1311写一个字节的命令
 **
 ** \参  数  命令字
 ** \返回值  none
 ******************************************************************************/
void sky1311WriteCmd(uint8_t cmd)
{
    SKY1311_CS_LOW();

    cmd = (cmd & 0x1F) | 0x80;	        // bit7,6,5 = 100b, mean command
    /*
    SKY1311_SPI->DR = cmd;              // write to data register

    while((SKY1311_SPI->SR & 0x01) == 0); // wait for RX buffer not empty

    uint8_t temp = SKY1311_SPI->DR ;    // flush RX buffer

    while((SKY1311_SPI->SR & 0x02) == 0);   // wait for TX buffer empty

    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
    temp = SKY1311_SPI->DR ;                // flush RX buffer
    */
    SPIByteCom(cmd);

    SKY1311_CS_HIGH();

}
/**
 ******************************************************************************
 ** \简  述  通过SPI接口向1311的寄存器写一个字节数据
 **
 ** \参  数  uint8_t regAdd: 寄存器地址， uint8_t data: 要写入的数据
 ** \返回值  none
 ******************************************************************************/
void sky1311WriteReg(uint8_t regAdd, uint8_t data)
{

    SKY1311_CS_LOW();

    regAdd      =   (regAdd & 0x3F);        // bit7,6=00, config as addr/write mode
/*
    SKY1311_SPI->DR = regAdd;               // write to data register

    while((SKY1311_SPI->SR & 0x01) == 0);  // wait for RX buffer not empty

    uint8_t temp = SKY1311_SPI->DR ;        // flush RX buffer

    while((SKY1311_SPI->SR & 0x02) == 0);   // wait for TX buffer empty

    SKY1311_SPI->DR = data;                 // write "data"to data register

    while((SKY1311_SPI->SR & 0x02) == 0);   // wait for TX buffer empty

    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
    temp = SKY1311_SPI->DR ;                // flush RX buffer
 */
    SPIByteCom(regAdd);
    SPIByteCom(data);

    SKY1311_CS_HIGH();
}
/**
 ******************************************************************************
 ** \简  述  通过SPI接口读取1311的寄存器
 **
 ** \参  数  uint8_t regAdd: 寄存器地址
 ** \返回值  uint8_t 寄存器内容
 ******************************************************************************/
uint8_t sky1311ReadReg(uint8_t regAdd)
{
    uint8_t value;
    SKY1311_CS_LOW();

    regAdd      =   (regAdd & 0x3F) | 0x40;            // bit7,6=01, config as addr/read mode
/*
    value = SKY1311_SPI->DR;

    while((SKY1311_SPI->SR & 0x02) == 0);

    SKY1311_SPI->DR = regAdd;

    while((SKY1311_SPI->SR & 0x01) == 0);

    value = SKY1311_SPI->DR;

    while((SKY1311_SPI->SR & 0x02) == 0);

    SKY1311_SPI->DR = 0xFF;

    while((SKY1311_SPI->SR & 0x01) == 0);

    value = SKY1311_SPI->DR;
*/
    SPIByteCom(regAdd);
    value = SPIByteCom(0xFF);

    SKY1311_CS_HIGH();
    return value;
}
/**
 ******************************************************************************
 ** \简  述  通过SPI接口向1311的FIFO写指定数目的数据
 **
 ** \参  数  uint8_t* 数据内容头地址， uint8_t count: 要写入的数据数量
 ** \返回值  none
 ******************************************************************************/
void sky1311WriteFifo(uint8_t *data, uint8_t count)
{
    uint8_t add;
    SKY1311_CS_LOW();

    add      =   (ADDR_FIFO & 0x3F);               // bit7,6=00, config as addr/write mode
/*
    SKY1311_SPI->DR = add;

    while((SKY1311_SPI->SR & 0x01) == 0);

    uint8_t temp = SKY1311_SPI->DR ;

    while((SKY1311_SPI->SR & 0x02) == 0);

    while(count--)
    {
        SKY1311_SPI->DR = *data++;

        while((SKY1311_SPI->SR & 0x02) == 0);
    }
    //delay(2);
    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
    temp = SKY1311_SPI->DR ;                // flush RX buffer
*/
    SPIByteCom(add);
    while(count--)
    {
        SPIByteCom(*data++);
    }

    SKY1311_CS_HIGH();

}
/**
 ******************************************************************************
 ** \简  述  通过SPI接口向1311的FIFO读取指定数目的内容
 **
 ** \参  数  uint8_t* data 保存读取内容的缓冲区首地址， uint8_t count 读取的字节数
 ** \返回值  none
 ******************************************************************************/
void sky1311ReadFifo(uint8_t *data, uint8_t count)
{
    uint8_t add;

    SKY1311_CS_LOW();

    add   =   (ADDR_FIFO & 0x3F) | 0x40;            // bit7,6=01, config as addr/read mode
/*
    uint8_t temp = SKY1311_SPI->DR;

    while((SKY1311_SPI->SR & 0x02) == 0);

    SKY1311_SPI->DR = add;

    while((SKY1311_SPI->SR & 0x01) == 0);

    temp = SKY1311_SPI->DR;

    while(count--)
    {
      while((SKY1311_SPI->SR & 0x02) == 0);

      SKY1311_SPI->DR = 0xFF;

      while((SKY1311_SPI->SR & 0x01) == 0);

      *data++ = SKY1311_SPI->DR;
    }

    while((SKY1311_SPI->SR & 0x80) == 0x80); // wait for not Busy
*/
    SPIByteCom(add);
    while(count--)
    {
        *data++ = SPIByteCom(0xFF);
    }

    SKY1311_CS_HIGH();
}

