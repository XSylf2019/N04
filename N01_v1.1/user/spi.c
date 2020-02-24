#include "spi.h"
#include "sky1311_drv.h"
/******************************************************************************/
void SKY1311_SPI_Init(void)
{
  /* Set the MOSI,MISO and SCK at high level */
  //GPIO_ExternalPullUpConfig(SKY1311_SPI_SCK_GPIO_PORT, SKY1311_SPI_SCK_PIN | \
                            //SKY1311_SPI_MISO_PIN | SKY1311_SPI_MOSI_PIN, ENABLE);

    GPIO_Init(SKY1311_SPI_SCK_GPIO_PORT, SKY1311_SPI_SCK_PIN, GPIO_Mode_Out_PP_High_Fast);  // PB.05
    GPIO_Init(SKY1311_SPI_MISO_GPIO_PORT, SKY1311_SPI_MISO_PIN, GPIO_Mode_In_FL_No_IT);     // PB.07
    GPIO_Init(SKY1311_SPI_MOSI_GPIO_PORT, SKY1311_SPI_MOSI_PIN, GPIO_Mode_Out_PP_High_Fast);// PB.06
    GPIO_Init(SKY1311_SPI_CS_GPIO_PORT, SKY1311_SPI_CS_PIN, GPIO_Mode_Out_PP_High_Fast);    // PB.04
    GPIO_Init(SKY1311_IRQ_GPIO_PORT, SKY1311_IRQ_PIN, GPIO_Mode_In_FL_No_IT);               // PB.03
    GPIO_Init(SKY1311_PD_GPIO_PORT, SKY1311_PD_PIN, GPIO_Mode_Out_PP_High_Fast);            // PB.02

    /*GPIOB->DDR = (GPIOB->DDR | 0x20 | 0x40 | 0x10);
    GPIOB->CR1 = (GPIOB->CR1 | 0x20 | 0x40 | 0x10);
    GPIOB->CR2 = (GPIOB->CR2 | 0x20 | 0x40 | 0x10);

    GPIOB->DDR = (GPIOB->DDR & 0x7F);
    GPIOB->CR1 = (GPIOB->CR1 | 0x80);
    GPIOB->CR2 = (GPIOB->CR2 & 0x7F);

    GPIOB->ODR = (GPIOB->ODR & 0xFD);
    GPIOB->DDR = (GPIOB->DDR & 0xFD);
    GPIOB->CR1 = (GPIOB->CR1 & 0xFD);
    GPIOB->CR2 = (GPIOB->CR2 & 0xFD);*/

  /* SKY1311_SPI Periph clock enable */
  CLK_PeripheralClockConfig(SKY1311_SPI_CLK, ENABLE);
  /* SPI configuration */
  SPI_Init(SKY1311_SPI, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4, SPI_Mode_Master,
           SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);
    /* Enable SPI  */
  SPI_Cmd(SKY1311_SPI, ENABLE);
    /* Deselect the FLASH: Chip Select high */
  SKY1311_CS_HIGH();
}
/******************************************************************************/
uint8_t SPI_FLASH_SendByte(uint8_t byte)
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
/******************************************************************************/