#include "plan_utilities.h"

//==============================================================================
// FUNCTION GPIO_ToggleBits()
//      - Negates GPIO output data register
//==============================================================================
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->ODR ^= GPIO_Pin;
}

//==============================================================================
// FUNCTION set_tick()
//      - Sets SysTick to set an interrupt request after specified
//        number of microseconds
//==============================================================================
void set_tick(uint32_t us)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config((RCC_Clocks.HCLK_Frequency/FACTOR_us_PER_s)*us);
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}


