#ifndef PLAN_UTILITIES_H
#define PLAN_UTILITIES_H

// INCLUDES  ===================================================================

#include "plan_include.h"



// DEFINES  ====================================================================
#define TIMEBASE_100_HZ                 100     //Hz
#define TIMEBASE_250_HZ                 250     //Hz
#define TIMEBASE_500_HZ                 500     //Hz
#define FACTOR_us_PER_s                 1000000 //us per 1s


#define TEXT_LENGTH                     200     //bytes
#define TOKEN_NUMBR                     30      //tokens


// FUNCTION PROTOTYPES  ========================================================

void GPIO_ToggleBits(GPIO_TypeDef*, uint16_t);
void update_fsm(void);
void calculate_gains(void);
int u2s(int data, int size);


#endif //PLAN_UTILITIES_H
