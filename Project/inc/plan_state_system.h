#ifndef PLAN_STATE_SYSTEM_H
#define PLAN_STATE_SYSTEM_H

#include "plan_include.h"
#include <stddef.h>

typedef enum
{
  EVENT_INITIALIZATION_SUCCESS          =       0,
  EVENT_INITIALIZATION_FAIL             =       1,
  EVENT_EXTERNAL_MEMORY_DETECTED        =       2,
  EVENT_EXTERNAL_MEMORY_DISCONNECTED    =       3,
  EVENT_MANIFEST_READ_SUCCESS           =       4,
  EVENT_MANIFEST_READ_FAIL              =       5,
  EVENT_FSM_BUILD_SUCCESS               =       6,
  EVENT_FSM_BUILD_FAIL                  =       7,
  EVENT_USER_FSM_INITIALIZE             =       8,
  EVENT_FSM_INITIALIZED                 =       9,
  EVENT_FLEXSEA_ERROR                   =       10,
} event;

typedef enum
{
  STATE_GENERIC                         =       0,
  STATE_ERROR                           =       1,
  STATE_INITIALIZING                    =       2,
  STATE_WAITING_FOR_MANIFEST            =       3,
  STATE_READING_MANIFEST                =       4,
  STATE_INCORRECT_MANIFEST              =       5,
  STATE_BUILDING_FSM                    =       6,
  STATE_WAIT_FOR_USER                   =       7,
  STATE_INITIALIZING_FSM                =       8,
  STATE_ACTIVE_CONTROL                  =       9,
} state;

typedef struct 
{
  state source;
  event event;
  state destination;
} transition;

#define ENTRY_STATE     STATE_INITIALIZING

void change_sys_state(state* curstate_p, event curevent);

#endif //PLAN_STATE_SYSTEM_H