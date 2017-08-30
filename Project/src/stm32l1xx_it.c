/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/DataExchangeDMA/stm32l1xx_it.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    20-April-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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
#include "stm32l1xx_it.h"
#include "stm32l1xx_conf.h"
#include "plan_include.h"

extern uint16_t state_time_us;
extern uint8_t systick_update;

extern int delay;

extern state sys_state;

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{}

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
    GPIO_SetBits(LEDx_PORT, LED2_PIN);
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

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
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_xx.s).                                            */
/******************************************************************************/

// User Button Handler
void EXTI0_IRQHandler(void)
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
        GPIO_SetBits(GPIO_SPIx_SS_MN_PORT,GPIO_SPIx_SS_MN_PIN); // De-select manage
        GPIO_ResetBits(GPIO_SPIx_EN_MN_PORT,GPIO_SPIx_EN_MN_PIN); // De-select manage
        
        build_fsm();
        
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

// SD Card Detect Handler
void EXTI1_IRQHandler(void)
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

// DMA SPI RX Handler
void DMA1_Channel4_IRQHandler(void)
{
  // Stop packet transmission sequence - drive chip select high
  DMA_Cmd(SPIx_DMA_RX_CHANNEL, DISABLE);
  GPIO_SetBits(GPIO_SPIx_SS_MN_PORT,GPIO_SPIx_SS_MN_PIN);
  GPIO_ResetBits(GPIO_SPIx_EN_MN_PORT,GPIO_SPIx_EN_MN_PIN);
  // Clear all channel 2 IT requests
  DMA_ClearITPendingBit(DMA1_IT_GL4);
}

// DMA SPI TX Handler
void DMA1_Channel5_IRQHandler(void)
{
  // DMA is in circular mode - will keep writing to SPI->DR unless stopped
  //  by disabling the DMA channel
  DMA_Cmd(SPIx_DMA_TX_CHANNEL, DISABLE);
  // Clear all channel 3 IT requests
  DMA_ClearITPendingBit(DMA1_IT_GL5);
}

// DMA USART RX Handler
void DMA1_Channel6_IRQHandler(void)
{
  DMA_Cmd(USARTx_DMA_RX_CHANNEL, DISABLE);
  // Clear all channel 5 IT requests
  DMA_ClearITPendingBit(DMA1_IT_GL6);
}

// DMA USART TX Handler
void DMA1_Channel7_IRQHandler(void)
{
  // DMA is in circular mode - will keep writing to USART->TDR unless stopped
  //  by disabling the DMA channel
  DMA_Cmd(USARTx_DMA_TX_CHANNEL, DISABLE);
  // Clear all channel 4 IT requests
  DMA_ClearITPendingBit(DMA1_IT_GL7);
}




/*
// Handles Button press response (press and release)
void EXTI0_1_IRQHandler(void)
{
  char line[TEXT_LENGTH];
  if(EXTI_GetITStatus(EXTI_UB_LINE) != RESET)
  {
    // If button is pressed
    if ((GPIO_EXTI_UB_PORT->IDR & GPIO_EXTI_UB_PIN))
    {
      change_sys_state(&sys_state,EVENT_MANIFEST_READ_SUCCESS);
      GPIO_ResetBits(LEDx_PORT,LED1_PIN);
      
      if(sys_state==STATE_INITIALIZING_MEMORY)
      {
        GPIO_SetBits(GPIO_SPIx_SS_MN_PORT,GPIO_SPIx_SS_MN_PIN); // De-select manage
        GPIO_ResetBits(GPIO_SPIx_EN_MN_PORT,GPIO_SPIx_EN_MN_PIN); // De-select manage
        
        read_line(line);
        jsmn_parser parser;
        jsmntok_t tokens[20];
        jsmn_init(&parser);
        jsmn_parse(&parser, line, strlen(line), tokens, 20);
        
        
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
    GPIO_ResetBits(GPIO_SPIx_EN_MN_PORT,GPIO_SPIx_EN_MN_PIN);
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
*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
