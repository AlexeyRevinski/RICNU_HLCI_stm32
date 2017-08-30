#ifndef PLAN_LED_H
#define PLAN_LED_H

// INCLUDES  ===================================================================

#include "plan_include.h"

// NICKNAMES: LED  =============================================================

#define LEDx_CLK                        RCC_AHBPeriph_GPIOB
#define LEDx_PORT                       GPIOB
#define LED1_PIN                        GPIO_Pin_6              //BLUE
#define LED2_PIN                        GPIO_Pin_7              //GREEN

// FUNCTION PROTOTYPES  ========================================================

void LED_config(void);

#endif //PLAN_LED_H
