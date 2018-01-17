#ifndef PLAN_STATE_SYSTEM_H
#define PLAN_STATE_SYSTEM_H

#include "plan_include.h"
#include <stddef.h>

typedef enum
{
  EVENT_EXTERNAL_MEMORY_DETECTED,
  EVENT_EXTERNAL_MEMORY_DISCONNECTED,
  EVENT_FSM_BUILD_SUCCESS,
  EVENT_FSM_BUILD_FAIL,
  EVENT_USER_INPUT_RECEIVED,
  EVENT_RESET_REQUEST,
  EVENT_FLEXSEA_ERROR = 0xFF,
} event;

typedef enum
{
  STATE_ERROR,
  STATE_INITIALIZING,
  STATE_WAITING_FOR_MEMORY,
  STATE_BUILDING_FSM,
  STATE_WAIT_FOR_USER,
  STATE_ACTIVE_CONTROL,
} state;


typedef enum
{
  MEM_OUT,
  MEM_IN,
} mstate;

typedef struct 
{
  state source;
  event event;
  state destination;
} transition;

#define ENTRY_STATE     STATE_INITIALIZING

void change_sys_state(state* curstate_p, event curevent);

#endif //PLAN_STATE_SYSTEM_H
