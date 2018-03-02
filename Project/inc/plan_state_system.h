#ifndef PLAN_STATE_SYSTEM_H
#define PLAN_STATE_SYSTEM_H

#include "plan_include.h"

// EVENTS ----------------------------------------------------------------------
typedef enum
{
  EVENT_INITIALIZED,            // Occurs at the end of initialization seq
  EVENT_CALIBRATED,
  EVENT_SDCARD_IN,              // Occurs when SD Card is inserted
  EVENT_SDCARD_OUT,             // Occurs when SD Card is taken out
  EVENT_START,                  // Occurs when START command received from app
  EVENT_STOP,                   // Occurs when STOP command received from app
  EVENT_CALIB,
  EVENT_CALIB_ACK_SENT,
  EVENT_FLEXSEA_ERROR = 0xFF,   // Occurs upon error in FlexSEA communication
} event;

// SYSTEM STATES ---------------------------------------------------------------
typedef enum
{
  STATE_ERROR,                  // Error state
  STATE_INITIALIZING,           // Initial state
  STATE_CALIBRATION,
  STATE_DONE_CALIB,
  STATE_ACTIVE,                 // Active control/logging/streaming
  STATE_INACTIVE,               // Idle state; waiting for command from app
} state;

// EXTERNAL MEMORY STATES ------------------------------------------------------
typedef enum
{
  MEM_OUT,                      // When SD Card is taken out
  MEM_IN,                       // When SD Card is inserted
} mstate;

// TRANSITION STRUCTURE --------------------------------------------------------
typedef struct 
{
  state source;                 // Original state
  event event;                  // Event upon which to transition
  state destination;            // Final state
} transition;

// FUNCTION PROTOTYPES =========================================================
void change_sys_state(state* curstate_p, event curevent);

#endif //PLAN_STATE_SYSTEM_H
