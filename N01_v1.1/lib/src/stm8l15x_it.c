/**
  ******************************************************************************
  * @file    EXTI/EXTI_IOControl/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    13-May-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "globe.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_IOControl
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t PressedButton;
//volatile uint8_t SleepDelayCount;//睡眠倒计时
//volatile unsigned int Time1ms_count;//切记，中断中调用的变量一律要volatile声明，要不怎么死都不知道。
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  RTC_WakeUpCmd(DISABLE);
  gu_rtcwkflag = 1;//rtc定时唤醒喂狗
  RTC_ClearITPendingBit(RTC_IT_WUT);
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1528_EVAL
  /* Joystick UP button is pressed */
  PressedButton = BUTTON_UP;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin0);
#endif /* USE_STM8L1528_EVAL */
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler, 9)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
#ifdef USE_STM8L1526_EVAL
  /* Check if Joystick left button is pressed */
  if (STM_EVAL_PBGetState(BUTTON_LEFT) == RESET)
  {
    PressedButton = BUTTON_LEFT;
  }

  /* Check if Key button is pressed */
  if (STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
    PressedButton = BUTTON_KEY;
  }

  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);

#elif defined USE_STM8L1528_EVAL

  /* Check if Joystick down button is pressed */
  if (STM_EVAL_PBGetState(BUTTON_DOWN) == RESET)
  {
    PressedButton = BUTTON_DOWN;
  }

  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);

#endif /* USE_STM8L152X_EVAL */
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL

  /* Joystick SEL button is pressed */
  PressedButton = BUTTON_SEL;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);

#elif defined USE_STM8L1528_EVAL

  /* Joystick LEFT button is pressed */
  PressedButton = BUTTON_LEFT;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);

#endif /* USE_STM8L152X_EVAL */
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1528_EVAL
  /* Joystick RIGHT button is pressed */
  PressedButton = BUTTON_RIGHT;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin3);
#endif /* USE_STM8L1528_EVAL */
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL
  /* Joystick DOWN button is pressed */
  PressedButton = BUTTON_DOWN;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin4);

#elif defined USE_STM8L1528_EVAL

  /* Joystick SEL button is pressed */
  PressedButton = BUTTON_SEL;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin4);

#endif /* USE_STM8L152X_EVAL */
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL
  /* Joystick RIGHT button is pressed */
  PressedButton = BUTTON_RIGHT;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin5);
#endif /* USE_STM8L1526_EVAL */
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1528_EVAL
  /* Key button is pressed */
  PressedButton = BUTTON_KEY;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin6);
#endif /* USE_STM8L1526_EVAL */
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  /* Joystick UP button is pressed */

  /* Cleat Interrupt pending bit */

}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler, 20)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  USART_ClearITPendingBit (USART2,USART_IT_RXNE);//清中断标志
  
//  USART_SendData8 (USART1,USART_ReceiveData8 (USART1));//将接收到的数据发送出去
  
  UART2.data = USART_ReceiveData8 (USART2);
  if(UART2.data == 0x7F && UART2.startflag == 0)
  {
    UART2.completeflag = 0;
    UART2.startflag = 1;
    UART2.index = 1;
    UART2.buff[0] = UART2.data; 
  }
  else if(UART2.startflag)
  {
    UART2.buff[UART2.index] = UART2.data; 
    if(UART2.index == UART2.buff[1] + 2)
    {
      UART2.length = UART2.index;
      UART2.completeflag = 1;
      UART2.startflag = 0;
      UART2.index = 0;
    }
    if(++UART2.index == 20)
    {
      UART2.startflag = 0;
      UART2.index = 0;
    }
  }
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler, 21)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler, 22)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  USART_ClearITPendingBit (USART3,USART_IT_RXNE);//清中断标志
  
//  USART_SendData8 (USART1,USART_ReceiveData8 (USART1));//将接收到的数据发送出去
  
  UART3.data = USART_ReceiveData8 (USART3);
  if(UART3.data == 0x7F && UART3.startflag == 0)
  {
    UART3.completeflag = 0;
    UART3.startflag = 1;
    UART3.index = 1;
    UART3.buff[0] = UART3.data; 
  }
  else if(UART3.startflag)
  {
    UART3.buff[UART3.index] = UART3.data; 
    if(UART3.index == UART3.buff[1] + 2)
    {
      UART3.length = UART3.index;
      UART3.completeflag = 1;
      UART3.startflag = 0;
      UART3.index = 0;
    }
    if(++UART3.index == 20)
    {
      UART3.startflag = 0;
      UART3.index = 0;
    }
  }
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  TimingDelay_Decrement();
  /* Cleat Interrupt Pending bit */
  TIM4_ClearITPendingBit(TIM4_IT_Update);
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler, 26)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler, 27)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler, 28)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  USART_ClearITPendingBit (USART1,USART_IT_RXNE);//清中断标志
  
//  USART_SendData8 (USART1,USART_ReceiveData8 (USART1));//将接收到的数据发送出去
  
  UART1.data = USART_ReceiveData8 (USART1);
  if(UART1.data == 0x7F && UART1.startflag == 0)
  {
    UART1.completeflag = 0;
    UART1.startflag = 1;
    UART1.index = 1;
    UART1.buff[0] = UART1.data; 
  }
  else if(UART1.startflag)
  {
    UART1.buff[UART1.index] = UART1.data; 
    if(UART1.index == UART1.buff[1] + 2)
    {
      UART1.length = UART1.index;
      UART1.completeflag = 1;
      UART1.startflag = 0;
      UART1.index = 0;
    }
    if(++UART1.index == 20)
    {
      UART1.startflag = 0;
      UART1.index = 0;
    }
  }
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler, 29)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */

/**
  * @}
  */
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/