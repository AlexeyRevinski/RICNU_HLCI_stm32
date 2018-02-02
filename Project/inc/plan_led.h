#ifndef PLAN_LED_H
#define PLAN_LED_H

// INCLUDES  ===================================================================

#include "plan_include.h"

#define LED_R_PORT                      GPIO_LED_1_PORT
#define LED_R_PIN                       GPIO_LED_1_PIN
#define LED_Y_PORT                      GPIO_LED_2_PORT
#define LED_Y_PIN                       GPIO_LED_2_PIN
#define LED_G_PORT                      GPIO_LED_3_PORT
#define LED_G_PIN                       GPIO_LED_3_PIN
#define LED_B_PORT                      GPIO_LED_4_PORT
#define LED_B_PIN                       GPIO_LED_4_PIN

#define LED_RED                         (uint8_t) 0x0001
#define LED_YEL                         (uint8_t) 0x0002
#define LED_GRN                         (uint8_t) 0x0004
#define LED_BLU                         (uint8_t) 0x0008
#define ON                              (uint8_t) 1
#define OFF                             (uint8_t) 0
#define TOG                             (uint8_t) 1
#define CON                             (uint8_t) 0

#define LED_TIME                0.25    //s
#define LED_TIM_PERIOD          46875
#define LED_TIM_PRESCALER       256

void LED_state(const uint8_t,const uint8_t,const uint8_t);
void LED_update(void);
void LED_rainbow(void);

#endif //PLAN_LED_H
