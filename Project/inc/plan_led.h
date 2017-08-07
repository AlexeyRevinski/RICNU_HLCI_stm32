#ifndef PLAN_LED_H
#define PLAN_LED_H

// INCLUDES  ===================================================================

#include "plan_include.h"

// NICKNAMES: LED  =============================================================

#define LEDx_CLK                        RCC_AHBPeriph_GPIOC
#define LEDx_PORT                       GPIOC
#define LED1_PIN                        GPIO_Pin_8              //BLUE
#define LED2_PIN                        GPIO_Pin_9              //GREEN

// FUNCTION PROTOTYPES  ========================================================

void LED_config(void);

#endif //PLAN_LED_H