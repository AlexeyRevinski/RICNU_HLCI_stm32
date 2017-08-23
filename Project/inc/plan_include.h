#ifndef PLAN_INCLUDE_H
#define PLAN_INCLUDE_H

// INCLUDES  ===================================================================

// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External code
#include "flexsea.h"            // FlexSEA protocol definitions 
#include "diskio.h"             // FatFs low level I/O
#include "ff.h"                 // FatFs
#include "jsmn.h"               // JSMN library
#include "flexsea_system.h"     // FlexSEA system definitions
#include "cmd-RICNU_Knee_v1.h"  // RICNU Knee protocol
#include "stm32f0xx_conf.h"     // STM32 library from STMicroelectronics

// FRICNU Plan
#include "plan_comm.h"          // Communication code
#include "plan_config.h"        // Hardware configuration functions
#include "plan_led.h"           // LED add-on
#include "plan_utilities.h"     // Utility code
#include "plan_state_system.h"  // System state machine
#include "plan_sd.h"

#endif //PLAN_INCLUDE_H