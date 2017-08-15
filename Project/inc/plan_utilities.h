#ifndef PLAN_UTILITIES_H
#define PLAN_UTILITIES_H

// INCLUDES  ===================================================================

#include "plan_include.h"

#define CMD_RICNU					101

// DEFINES  ====================================================================
#define TIMEBASE_100_HZ                 100     //Hz
#define TIMEBASE_250_HZ                 250     //Hz
#define TIMEBASE_500_HZ                 500     //Hz
#define FACTOR_us_PER_s                 1000000 //us per 1s



// FUNCTION PROTOTYPES  ========================================================

void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void set_tick(uint32_t us);

#endif //PLAN_UTILITIES_H