#ifndef PLAN_INCLUDE_H
#define PLAN_INCLUDE_H

// INCLUDES  ===================================================================

#define CMD_RICNU					101


// External code
#include "flexsea.h"            // FlexSEA protocol definitions 
#include "flexsea_system.h"     // FlexSEA system definitions
#include "cmd-RICNU_Knee_v1.h"  // RICNU Knee protocol
#include "stm32f0xx_conf.h"     // STM32 library from STMicroelectronics

// FlexSEA Plan
#include "plan_comm.h"
#include "plan_config.h"
#include "plan_led.h"
#include "plan_utilities.h"

#endif //PLAN_INCLUDE_H