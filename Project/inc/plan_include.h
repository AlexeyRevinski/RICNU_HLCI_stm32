#ifndef PLAN_INCLUDE_H
#define PLAN_INCLUDE_H

// INCLUDES  ===================================================================

// Standard C libraries
#include <stdlib.h>             // For dynamic memory allocation functions          
#include <string.h>             // For string manipulation functions

// External code
#include "stdio.h"
#include "flexsea.h"            // FlexSEA protocol definitions 
#include "ff.h"                 // FatFs - FAT system library
#include "diskio.h"             // FatFs low level I/O apapter
#include "jsmn.h"               // JSMN - JSON parsing library
#include "flexsea.h"
#include "flexsea_system.h"     // FlexSEA system definitions                   // Maybe get rid of flexsea code!!!
#include "flexsea_comm.h"
#include "cmd-RICNU_Knee_v1.h"  // RICNU Knee protocol
#include "stm32f10x.h"          // STM32F10x Standard Peripheral library

// RICNU Plan
#include "plan_comm.h"          // Communication module
#include "plan_config.h"        // Hardware configuration module
#include "plan_fsm.h"           // Device control finite state machine module
#include "plan_led.h"           // LED module
#include "plan_log.h"			// Data logging module
#include "plan_sd.h"            // SD Card low level communication module
#include "plan_spi.h"			// SPI communication functions module
#include "plan_state_system.h"  // System finite state machine module

#endif //PLAN_INCLUDE_H
