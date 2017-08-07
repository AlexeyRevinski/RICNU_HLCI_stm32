#include "plan_led.h"

//==============================================================================
// FUNCTION LED_Config()
//      - Configures GPIO pins for: LEDs
//==============================================================================
void LED_config(void)
{
  // Declare GPIO initialization structure
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  // Enable GPIO C port clock
  RCC_AHBPeriphClockCmd(LEDx_CLK, ENABLE);
  
  // Configure LED GPIOs using initialization structure
  
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin   = LED1_PIN|LED2_PIN;
  
  // Implement configurations (LED1 and LED2 are on the same port C)
  GPIO_Init(LEDx_PORT, &GPIO_InitStructure);
}

