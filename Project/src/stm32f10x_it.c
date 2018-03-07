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

uint16_t        state_time_us = 0;      // Time in microseconds since last cycle
uint8_t         systick_update = 0;     // Did SysTick tick? 0=no, 1=yes

extern int delay;

extern state sys_state;
#define CALIBTIME 2000
#define ACKTIME	10

extern flexsea_ctrl fc;
extern			lstate log_state;

int64_t straincalib[] = {0,0,0,0,0,0};
int64_t imucalsum[] = {0,0,0,0,0,0,0,0,0};
int32_t imucal[] = {0,0,0,0,0,0,0,0,0};
int32_t straincal[] = {0,0,0,0,0,0};
uint32_t calibtimer = CALIBTIME;
uint32_t acktimer = ACKTIME;

extern ricnu_data rndata;

uint8_t prev_offset;



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
    else{
    	state_time_us=0;
    	//GPIO_ResetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN); LED_state(LED_BLU,ON,CON);
    	if(sys_state!=STATE_INITIALIZING||sys_state!=STATE_DONE_CALIB)
		{
    		comm_start(MANAGE);		// Start communication with Manage
		}
    }
    // Update software timers
    if(!(state_time_us%1000)){SD_TimeUpdate();}	//1kHz time base
    //GPIO_SetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN); LED_state(LED_BLU,OFF,CON);

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
	//GPIO_ResetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN);
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
	TIM_SetCounter(TIM3,0);						// Reset ms counter
    if (RTC_GetCounter() == 0x0001517F)			// If reached 23:59:59, reset
	{
    	RTC_WaitForLastTask();					// Wait until last write operation is finished
		RTC_SetCounter(0);							// Reset counter
	}
    RTC_WaitForLastTask();						// Wait for last write operation
  }
  RTC_ClearITPendingBit(RTC_IT_SEC);			// Clear interrupt pending bit

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
	  EXTI_ClearITPendingBit(EXTI_SD_CD_LINE);
    // See if the card is in or not
    //if(SD_DETECT);
    // Clear the interrupt pending bit

  }
}



// FlexSEA Manage:      SPI RX Handler
void DMA1_Channel4_IRQHandler(void)
{

	//GPIO_ResetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN); LED_state(LED_BLU,ON,CON);
	DMA_ClearITPendingBit(DMA1_IT_GL4);		// Clear all channel 4 IT requests
  DMA_Cmd(SPI_MN_DMA_RX_CHAN, DISABLE);		// Disable RX channel
  GPIO_SetBits(GPIO_MN_NSS_PORT,			// Unselect Manage
		  GPIO_MN_NSS_PIN);

  prev_offset = comm_unpack_manage();	// Unpack data from Manage

  if(sys_state==STATE_CALIBRATION)
  {
	if(calibtimer)
	{
	  if (calibtimer<=CALIBTIME)	// Discard first few packets
	  {
		  if(calibtimer>CALIBTIME/2)
		  {
			  fc.offset=1;fc.control=CTRL_NONE;
			  for (int i=0;i<6;i++)
			  {
				  straincalib[i]+=rndata.st[i];
			  }
		  }
		  else
		  {
			  fc.offset=0;fc.control=CTRL_NONE;
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
	  return;
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
		calibtimer = CALIBTIME;
		acktimer = ACKTIME;
		change_sys_state(&sys_state,EVENT_CALIBRATED);       // Initialized state
	}
  }
  if(sys_state==STATE_ACTIVE)				// If actively controlling, update fsm
  {
	  fsm_update();
	  if(log_state==LOG_ON)
	  {

		  log_generate();
	  }
  }
  else if (sys_state!=STATE_ACTIVE)
  {
	  fc.control = CTRL_NONE;
  }
  if(sys_state!=STATE_INITIALIZING && sys_state!=STATE_DONE_CALIB)
	{
		comm_prep_manage();		// Prep next manage packet
		comm_prep_user(prev_offset);
	}
  if(sys_state==STATE_DONE_CALIB)
	{
		if(acktimer)
		{
			comm_prep_user(3);
			//comm_start(USER);
			acktimer--;
		}
		else
		{
			change_sys_state(&sys_state,EVENT_CALIB_ACK_SENT);
		}
	}
  if(sys_state!=STATE_CALIBRATION)
	{
		comm_start(USER);
	}
  //GPIO_SetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN); LED_state(LED_BLU,OFF,CON);
}

// FlexSEA Manage:      SPI TX Handler
void DMA1_Channel5_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_GL5);  	// Clear all channel 5 IT requests
	DMA_Cmd(SPI_MN_DMA_TX_CHAN, DISABLE);	// Disable TX channel
}

// Bluetooth Module:    UART RX Handler
void DMA1_Channel6_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_GL6);		// Clear all channel 6 IT requests
	comm_unpack_user();						// Unpack data from RICNU User
}

// Bluetooth Module:    UART TX Handler
void DMA1_Channel7_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_GL7);		// Clear all channel 7 IT requests
	DMA_Cmd(USART_BT_DMA_TX_CHAN, DISABLE);	// Disable TX channel
}

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
