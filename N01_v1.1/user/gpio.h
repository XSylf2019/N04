#ifndef __GPIO_H_
#define __GPIO_H_
/******************************************************************************/
/* GPIO for SKY1311T*/
#define SKY1311_SPI                           SPI1
#define SKY1311_SPI_CLK                       CLK_Peripheral_SPI1

#define SKY1311_SPI_GPIO_PORT                 GPIOB

#define SKY1311_SPI_SCK_PIN                   GPIO_Pin_5                        /* PB.05 */
#define SKY1311_SPI_SCK_GPIO_PORT             GPIOB                             /* GPIOB */

#define SKY1311_SPI_MISO_PIN                  GPIO_Pin_7                        /* PB.07 */
#define SKY1311_SPI_MISO_GPIO_PORT            GPIOB                             /* GPIOB */

#define SKY1311_SPI_MOSI_PIN                  GPIO_Pin_6                        /* PB.06 */
#define SKY1311_SPI_MOSI_GPIO_PORT            GPIOB                             /* GPIOB */

#define SKY1311_SPI_CS_PIN                    GPIO_Pin_4                        /* PB.04 */
#define SKY1311_SPI_CS_GPIO_PORT              GPIOB                             /* GPIOB */

#define SKY1311_IRQ_PIN                       GPIO_Pin_3                        /* PB.02 */
#define SKY1311_IRQ_GPIO_PORT                 GPIOB                             /* GPIOB */

#define SKY1311_PD_PIN                        GPIO_Pin_2                        /* PB.00 */
#define SKY1311_PD_GPIO_PORT                  GPIOB                             /* GPIOB */

#define SKY1311_CS_LOW()        GPIO_ResetBits(SKY1311_SPI_CS_GPIO_PORT, SKY1311_SPI_CS_PIN)
#define SKY1311_CS_HIGH()       GPIO_SetBits(SKY1311_SPI_CS_GPIO_PORT, SKY1311_SPI_CS_PIN)

#define SKY1311_IRQ_READ()      GPIO_ReadInputDataBit(SKY1311_IRQ_GPIO_PORT,SKY1311_IRQ_PIN)

#define SKY1311_DISABLE()       GPIO_ResetBits(SKY1311_PD_GPIO_PORT, SKY1311_PD_PIN)
#define SKY1311_ENABLE()        GPIO_SetBits(SKY1311_PD_GPIO_PORT, SKY1311_PD_PIN)
#define sky1311Disable()        SKY1311_DISABLE()
#define sky1311Enable()         SKY1311_ENABLE()

/* GPIO for oled*/
#define OLED_PWR_PIN            GPIO_Pin_0                                      /* PG.00 */
#define OLED_PWR_PORT           GPIOG                                           /* GPIOG */

#define OLED_CS_PIN             GPIO_Pin_2                                      /* PA.02 */
#define OLED_CS_PORT            GPIOA                                           /* GPIOA */

#define OLED_RES_PIN            GPIO_Pin_3                                      /* PA.03 */
#define OLED_RES_PORT           GPIOA                                           /* GPIOA */

#define OLED_SCLK_PIN           GPIO_Pin_4                                      /* PA.04 */
#define OLED_SCLK_PORT          GPIOA                                           /* GPIOA */

#define OLED_SDIN_PIN           GPIO_Pin_5                                      /* PA.05 */
#define OLED_SDIN_PORT          GPIOA                                           /* GPIOA */

#define oledPowerOn() 		GPIO_ResetBits(OLED_PWR_PORT, OLED_PWR_PIN)
#define oledPowerOff() 		GPIO_SetBits(OLED_PWR_PORT, OLED_PWR_PIN)

#define oledResetValid() 	GPIO_ResetBits(OLED_RES_PORT, OLED_RES_PIN)
#define oledResetDisable() 	GPIO_SetBits(OLED_RES_PORT, OLED_RES_PIN)

#define spiSckHigh()		GPIO_SetBits(OLED_SCLK_PORT, OLED_SCLK_PIN)
#define spiSckLow()		GPIO_ResetBits(OLED_SCLK_PORT, OLED_SCLK_PIN)

#define spiSdoHigh()		GPIO_SetBits(OLED_SDIN_PORT, OLED_SDIN_PIN)
#define spiSdoLow()		GPIO_ResetBits(OLED_SDIN_PORT, OLED_SDIN_PIN)

#define oledCsDisable()		GPIO_SetBits(OLED_CS_PORT, OLED_CS_PIN)
#define oledCsValid()		GPIO_ResetBits(OLED_CS_PORT, OLED_CS_PIN)

/* GPIO for TSM12*/
#define TSM12_SCL_PIN           GPIO_Pin_4                                      /* PF.04 */
#define TSM12_SCL_PORT          GPIOF                                           /* GPIOF */

#define TSM12_SDA_PIN           GPIO_Pin_5                                      /* PF.05 */
#define TSM12_SDA_PORT          GPIOF                                           /* GPIOF */

#define TSM12_I2CE_PIN          GPIO_Pin_0                                      /* PF.00 */
#define TSM12_I2CE_PORT         GPIOF                                           /* GPIOF */

#define TSM12_IRQ_PIN           GPIO_Pin_1                                      /* PB.01 */
#define TSM12_IRQ_PORT          GPIOF                                           /* GPIOF */

#define TSM_SCL_H               GPIO_SetBits(TSM12_SCL_PORT, TSM12_SCL_PIN)
#define TSM_SCL_L               GPIO_ResetBits(TSM12_SCL_PORT, TSM12_SCL_PIN)

#define TSM_SDA_H               GPIO_SetBits(TSM12_SDA_PORT, TSM12_SDA_PIN)
#define TSM_SDA_L               GPIO_ResetBits(TSM12_SDA_PORT, TSM12_SDA_PIN)

#define TSM_EN_H                GPIO_SetBits(TSM12_I2CE_PORT, TSM12_I2CE_PIN)
#define TSM_EN_L                GPIO_ResetBits(TSM12_I2CE_PORT, TSM12_I2CE_PIN)

#define TSM_INT_READ()	        GPIO_ReadInputDataBit(TSM12_IRQ_PORT,TSM12_IRQ_PIN)

#define TSM_SDA_READ()          GPIO_ReadInputDataBit(TSM12_SDA_PORT,TSM12_SDA_PIN)

#define TSM_SDA_OUT()           GPIO_Init(TSM12_SDA_PORT,TSM12_SDA_PIN,GPIO_Mode_Out_PP_High_Slow)//输出上拉 2MHz
#define TSM_SDA_IN()            GPIO_Init(TSM12_SDA_PORT,TSM12_SDA_PIN,GPIO_Mode_In_PU_No_IT)//输出上拉 2MHz

/* GPIO for UART */
#define UART_GPIO_PORT          GPIOC
#define UART_TX_PIN             GPIO_Pin_3
#define UART_TX_PORT            GPIOC
#define UART_RX_PIN             GPIO_Pin_2
#define UART_RX_PORT            GPIOC

/* GPIO for LED*/
#define LED_PIN                 GPIO_Pin_0
#define LED_PORT                GPIOB
#define LED_ON()                GPIO_SetBits(LED_PORT, LED_PIN)
#define LED_OFF()               GPIO_ResetBits(LED_PORT, LED_PIN)
#define LED_Toggle()            GPIO_ToggleBits(LED_PORT,LED_PIN);

/* GPIO for RESET*/
#define RESET_PIN               GPIO_Pin_1
#define RESET_PORT              GPIOF
#define Read_Akey()             GPIO_ReadInputDataBit(RESET_PORT,RESET_PIN)

/* GPIO for FINGER*/
#define FINGER_IRQ_PIN          GPIO_Pin_4
#define FINGER_IRQ_PORT         GPIOC
#define Read_finger()           GPIO_ReadInputDataBit(FINGER_IRQ_PORT,FINGER_IRQ_PIN)

#define FINGER_PWR_PIN          GPIO_Pin_0
#define FINGER_PWR_PORT         GPIOG
#define Finger_PWRON()          GPIO_SetBits(FINGER_PWR_PORT, FINGER_PWR_PIN)
#define Finger_PWROFF()         GPIO_ResetBits(FINGER_PWR_PORT, FINGER_PWR_PIN)

/* GPIO for MOTO*/
#define MOTOR_PIN               GPIO_Pin_2
#define MOTOR_PORT              GPIOE
#define MOTOR_H()               GPIO_SetBits(MOTOR_PORT, MOTOR_PIN)
#define MOTOR_L()               GPIO_ResetBits(MOTOR_PORT, MOTOR_PIN)

#define MOTOL_PIN               GPIO_Pin_1
#define MOTOL_PORT              GPIOE
#define MOTOL_H()               GPIO_SetBits(MOTOL_PORT, MOTOL_PIN)
#define MOTOL_L()               GPIO_ResetBits(MOTOL_PORT, MOTOL_PIN)

/* GPIO for SPEAK*/
#define SPEAK_PIN               GPIO_Pin_2
#define SPEAK_PORT              GPIOG
#define SPEAK_ON()              GPIO_SetBits(SPEAK_PORT, SPEAK_PIN)
#define SPEAK_OFF()             GPIO_ResetBits(SPEAK_PORT, SPEAK_PIN)
/******************************************************************************/
#endif
