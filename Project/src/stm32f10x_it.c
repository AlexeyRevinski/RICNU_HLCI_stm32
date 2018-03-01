/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f10x_it.h"
#include "plan_include.h"

extern uint16_t state_time_us;
extern uint8_t systick_update;

extern int delay;

extern state sys_state;
#define CALIBTIME 2000

int64_t straincalib[] = {0,0,0,0,0,0};
int64_t imucalsum[] = {0,0,0,0,0,0,0,0,0};
int32_t imucal[] = {0,0,0,0,0,0,0,0,0};
int32_t straincal[] = {0,0,0,0,0,0};
uint32_t calibtimer = CALIBTIME;

extern ricnu_data rndata;



//extern time tm;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
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
  GPIO_SetBits(GPIO_LED_1_PORT,GPIO_LED_1_PIN);
  while (1)
  {
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
  GPIO_SetBits(GPIO_LED_1_PORT,GPIO_LED_1_PIN);
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  GPIO_SetBits(GPIO_LED_1_PORT,GPIO_LED_1_PIN);
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  GPIO_SetBits(GPIO_LED_1_PORT,GPIO_LED_1_PIN);
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
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
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
	// Set update flag
	systick_update = 1;

    // Increment time by SYS_TICK_US microseconds
    if(state_time_us!=(SYS_PERIOD_US-SYS_TICK_US)){state_time_us+=SYS_TICK_US;}
    else{state_time_us=0;}

    // Update software timers
    if(!(state_time_us%1000)){SD_TimeUpdate();}	//1kHz time base
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */

void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_SEC);			// Clear interrupt pending bit
    if (RTC_GetCounter() == 0x0001517F)			// If reached 23:59:59, reset
	{
    	RTC_WaitForLastTask();					// Wait until last write operation is finished
		RTC_SetCounter(0);							// Reset counter
	}
    RTC_WaitForLastTask();						// Wait for last write operation
  }
  TIM_SetCounter(TIM3,0);						// Reset ms counter
}

void TIM2_IRQHandler(void)
{
  if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    LED_update();
  }
}

void EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_SD_CD_LINE) != RESET)
  {
    
    // See if the card is in or not
    //if(SD_DETECT);
    // Clear the interrupt pending bit
    EXTI_ClearITPendingBit(EXTI_SD_CD_LINE);
  }
}

// SD Card:             SPI RX Handler
void DMA1_Channel2_IRQHandler(void)
{
  // Stop packet transmission sequence - drive chip select high
  DMA_Cmd(SPI_SD_DMA_RX_CHAN, DISABLE);
  //GPIO_SetBits(GPIO_SD_NSS_PORT,GPIO_SD_NSS_PIN);
  // Clear all channel 2 IT requests
  DMA_ClearITPendingBit(DMA1_IT_GL2);
}
// SD Card:             SPI TX Handler
void DMA1_Channel3_IRQHandler(void)
{
  // DMA is in circular mode - will keep writing to SPI->DR unless stopped
  //  by disabling the DMA channel
  DMA_Cmd(SPI_SD_DMA_TX_CHAN, DISABLE);
  // Clear all channel 3 IT requests
  DMA_ClearITPendingBit(DMA1_IT_GL3);
}

// FlexSEA Manage:      SPI RX Handler
void DMA1_Channel4_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_GL4);
  // Stop packet transmission sequence - drive chip select high
  DMA_Cmd(SPI_MN_DMA_RX_CHAN, DISABLE);
  GPIO_SetBits(GPIO_MN_NSS_PORT,GPIO_MN_NSS_PIN);

  unpack(MANAGE);

  if(sys_state==STATE_CALIBRATION)
  {

		if(calibtimer)
		{


		  if (calibtimer<=CALIBTIME)	// Discard first few packets
		  {

			  if(calibtimer>CALIBTIME/2)
			  {
				  prep_packet(1,CTRL_NONE,0,0,0,0,0);
				  //update(MANAGE);
				  //unpack(MANAGE);
				  for (int i=0;i<6;i++)
				  {
					  straincalib[i]+=rndata.st[i];
				  }
			  }
			  else
			  {
				  prep_packet(0,CTRL_NONE,0,0,0,0,0);
				  //update(MANAGE);
				  //unpack(MANAGE);
				  imucalsum[0]+=rndata.gx;
				  imucalsum[1]+=rndata.gy;
				  imucalsum[2]+=rndata.gz;
				  imucalsum[3]+=rndata.ax;
				  imucalsum[4]+=rndata.ay;
				  imucalsum[5]+=rndata.az;
				  imucalsum[6]+=rndata.em;
				  imucalsum[7]+=rndata.ej;
				  imucalsum[8]+=rndata.cu;
			  }
		  }
		  calibtimer--;
		}
		else
		{
		for (int i=0;i<6;i++)
		{
			straincalib[i]/=(CALIBTIME/2);
			straincal[i] = (int32_t) straincalib[i];
		}
		for (int i=0;i<9;i++)
		{
			imucalsum[i]/=(CALIBTIME/2);
			imucal[i] = (int32_t) imucalsum[i];
		}
		change_sys_state(&sys_state,EVENT_CALIBRATED);       // Initialized state
		}
  }
  else if (sys_state==STATE_ACTIVE)
  {
	  update(USER);
	  fsm_update();
  }





  // Clear all channel 4 IT requests
}

// FlexSEA Manage:      SPI TX Handler
void DMA1_Channel5_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_GL5);
  // DMA is in circular mode - will keep writing to SPI->DR unless stopped
  //  by disabling the DMA channel
  DMA_Cmd(SPI_MN_DMA_TX_CHAN, DISABLE);
  // Clear all channel 5 IT requests
}

// Bluetooth Module:    UART RX Handler
void DMA1_Channel6_IRQHandler(void)
{
  DMA_Cmd(USART_BT_DMA_RX_CHAN, DISABLE);
  // Clear all channel 6 IT requests
  DMA_ClearITPendingBit(DMA1_IT_GL6);

  unpack(USER);
}

// Bluetooth Module:    UART TX Handler
void DMA1_Channel7_IRQHandler(void)
{
  // DMA is in circular mode - will keep writing to USART->TDR unless stopped
  //  by disabling the DMA channel
  DMA_Cmd(USART_BT_DMA_TX_CHAN, DISABLE);
  // Clear all channel 7 IT requests
  DMA_ClearITPendingBit(DMA1_IT_GL7);
}

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
