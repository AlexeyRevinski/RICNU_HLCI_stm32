#include "plan_led.h"

static const uint8_t LED[4]      = {LED_RED,   LED_YEL,   LED_GRN,   LED_BLU};
static GPIO_TypeDef* LED_PORT[4] = {LED_R_PORT,LED_Y_PORT,LED_G_PORT,LED_B_PORT};
static const uint8_t LED_PIN[4]  = {LED_R_PIN, LED_Y_PIN, LED_G_PIN, LED_B_PIN};

static uint8_t led_state = 0;

//==============================================================================
// FUNCTION LED_state()
//      - changes global LED state variable
//==============================================================================
void LED_state(const uint8_t led,const uint8_t en,const uint8_t mod)
{
  if(en)        led_state|=led;         //Set that LED's on bit
  else          led_state&=(~led);      //Reset that LED's on bit
  if(mod)       led_state|=(led<<4);    //Set that LED's mod bit
  else          led_state&=(~(led<<4)); //Reset that LED's mod bit
}

//==============================================================================
// FUNCTION LED_update()
//      - makes hardware changes based on led_state variable
//==============================================================================
void LED_update(void)
{
  static uint8_t toggle = 0;
  toggle = !toggle;
  for(int i = 0; i<4; i++)
  {
    if(!(led_state&LED[i]))     // If not supposed to be not active,
    {
      GPIO_SetBits(LED_PORT[i],LED_PIN[i]);     // Turn off
    }
    else                        // If supposed to be active
    {
      if(!(led_state&(LED[i]<<4))) // If supposed to be constant
      {
        GPIO_ResetBits(LED_PORT[i],LED_PIN[i]);
      }
      else
      {
        // Turn on or off depending on global flashing state
        if(toggle)      GPIO_ResetBits(LED_PORT[i],LED_PIN[i]);
        else            GPIO_SetBits(LED_PORT[i],LED_PIN[i]);
      }
    }
  }
}

//==============================================================================
// FUNCTION LED_update()
//      - performs a specific LED sequence to let user know that fsm is ready
//==============================================================================
void LED_rainbow(void)
{
  TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);     // Disable TIM2 interrupts
  for(int i = 0; i<7; i++)
  {
    while(TIM2->CNT<LED_TIM_PERIOD);    // Wait until TIM2 counts fully
    TIM_Cmd(TIM2,DISABLE);              // Stop counting
    switch(i)                           // Go through sequence step
    {
    case 1: LED_state(LED_BLU,ON,CON); break;    // Turn on BLU LED
    case 2: LED_state(LED_GRN,ON,CON); break;    // Turn on GRN LED
    case 3: LED_state(LED_YEL,ON,CON); break;    // Turn on YEL LED
    case 4: LED_state(LED_RED,ON,CON); break;    // Turn on RED LED
    case 0:
    case 5:     
      LED_state(LED_BLU,OFF,CON);   // Turn off all LEDs
      LED_state(LED_GRN,OFF,CON);
      LED_state(LED_YEL,OFF,CON);
      LED_state(LED_RED,OFF,CON);
      break;
    }
    LED_update();
    TIM2->CNT = 0;                      // Reset counter
    TIM_Cmd(TIM2,ENABLE);               // Start counting again
  }
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);      // Enable TIM2 interrupts
}

