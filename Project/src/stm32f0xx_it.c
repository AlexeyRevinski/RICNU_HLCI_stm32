/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "stm32f0xx_conf.h"

extern uint16_t state_time_us;
extern uint8_t systick_update;

extern int delay;

extern state sys_state;

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  systick_update = 1;
  if(state_time_us!=(SYSTEM_PERIOD_US-SYSTICK)){state_time_us+=SYSTICK;}
  else{state_time_us=0;}
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

// Handles Button press response (press and release)
void EXTI0_1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_UB_LINE) != RESET)
  {
    // If button is pressed
    if ((GPIO_EXTI_UB_PORT->IDR & GPIO_EXTI_UB_PIN))
    {
      change_sys_state(&sys_state,EVENT_MANIFEST_READ_SUCCESS);
      GPIO_ResetBits(LEDx_PORT,LED1_PIN);
      
      if(sys_state==STATE_INITIALIZING_MEMORY)
      {
        /* Set chip select high */
        GPIO_ResetBits(GPIO_SPIx_SS_SD_PORT,GPIO_SPIx_SS_SD_PIN);
        GPIO_SetBits(GPIO_SPIx_SS_MN_PORT,GPIO_SPIx_SS_MN_PIN);
        /* Send dummy byte 0xFF, 10 times with CS high */
        /* Rise CS and MOSI for 80 clocks cycles */
        for (int i = 0; i <= 9; i++)
        {
          /* Wait until the transmit buffer is empty */
          while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){;}
          /* Send the byte */
          SPI_SendData8(SPIx, 0xFF);
          /* Wait to receive a byte*/
          while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){;}
          SPI_ReceiveData8(SPIx);
        }
        while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)== SET){;}
        GPIO_SetBits(GPIO_SPIx_SS_SD_PORT,GPIO_SPIx_SS_SD_PIN);
        change_sys_state(&sys_state,EVENT_MEMORY_INITIALIZATION_SUCCESS);
      }
    }
    else // If button is not pressed
    {
      GPIO_SetBits(LEDx_PORT,LED1_PIN);
    }
    
    // Clear the interrupt pending bit
    EXTI_ClearITPendingBit(EXTI_UB_LINE);
  }
}

// Handles SD Card insertion/ejection response
void EXTI2_3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_CD_LINE) != RESET)
  {
    // If card is detected in the socket
    if ((GPIO_EXTI_CD_PORT->IDR & GPIO_EXTI_CD_PIN))
    {
      GPIO_ResetBits(LEDx_PORT,LED2_PIN);
      change_sys_state(&sys_state,EVENT_EXTERNAL_MEMORY_DETECTED);
    }
    else // If card was taken out of the socket
    {
      GPIO_SetBits(LEDx_PORT,LED2_PIN);
      change_sys_state(&sys_state,EVENT_EXTERNAL_MEMORY_DISCONNECTED);
    }
    // Clear the interrupt pending bit
    EXTI_ClearITPendingBit(EXTI_UB_LINE);
  }
}

// Handles SPI DMA transfer events
void DMA1_Channel2_3_IRQHandler(void)
{
  // If finished transmitting an SPI packet (48 byte limit reached)
  if (DMA_GetITStatus(DMA1_IT_TC3))
  {
    // DMA is in circular mode - will keep writing to SPI->DR unless stopped
    //  by disabling the DMA channel
    DMA_Cmd(SPIx_DMA_TX_CHANNEL, DISABLE);
    // Clear all channel 3 IT requests
    DMA_ClearITPendingBit(DMA1_IT_GL3);
  }
           
  // If finished receiving an SPI packet (48 byte limit reached)
  else if(DMA_GetITStatus(DMA1_IT_TC2))
  {
    // Stop packet transmission sequence - drive chip select high
    DMA_Cmd(SPIx_DMA_RX_CHANNEL, DISABLE);
    GPIO_SetBits(GPIO_SPIx_SS_MN_PORT,GPIO_SPIx_SS_MN_PIN);
    // Clear all channel 2 IT requests
    DMA_ClearITPendingBit(DMA1_IT_GL2);
  }
}

// Handles UART DMA transfer events
void DMA1_Channel4_5_IRQHandler(void)
{
  // If finished transmitting an SPI packet (48 byte limit reached)
  if (DMA_GetITStatus(DMA1_IT_TC4))
  {
    // DMA is in circular mode - will keep writing to USART->TDR unless stopped
    //  by disabling the DMA channel
    DMA_Cmd(USARTx_DMA_TX_CHANNEL, DISABLE);
    // Clear all channel 4 IT requests
    DMA_ClearITPendingBit(DMA1_IT_GL4);
  }
           
  // If finished receiving an SPI packet (48 byte limit reached)
  else if(DMA_GetITStatus(DMA1_IT_TC5))
  {
    DMA_Cmd(USARTx_DMA_RX_CHANNEL, DISABLE);
    // Clear all channel 5 IT requests
    DMA_ClearITPendingBit(DMA1_IT_GL5);
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
