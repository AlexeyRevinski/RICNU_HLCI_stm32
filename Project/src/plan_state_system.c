#include "plan_state_system.h"

static const transition transitionTable[] = {
  {STATE_INITIALIZING,          EVENT_EXTERNAL_MEMORY_DISCONNECTED,     STATE_WAITING_FOR_MEMORY},
  {STATE_INITIALIZING,          EVENT_EXTERNAL_MEMORY_DETECTED,         STATE_BUILDING_FSM},
  {STATE_WAITING_FOR_MEMORY,    EVENT_EXTERNAL_MEMORY_DETECTED,         STATE_BUILDING_FSM},
  {STATE_BUILDING_FSM,          EVENT_FSM_BUILD_SUCCESS,                STATE_WAIT_FOR_USER},
  {STATE_BUILDING_FSM,          EVENT_FSM_BUILD_FAIL,                   STATE_ERROR},
  {STATE_WAIT_FOR_USER,         EVENT_USER_INPUT_RECEIVED,              STATE_ACTIVE_CONTROL},
  {STATE_ACTIVE_CONTROL,        EVENT_FLEXSEA_ERROR,                    STATE_ERROR},
  {STATE_ERROR,                 EVENT_RESET_REQUEST,                    STATE_INITIALIZING},
};

static int table_size = (int) sizeof(transitionTable)/sizeof(transition);

void change_sys_state(state* curstate_p, event curevent)
{
  static int i=0;
  // Cycle through table entries
  for(i=0;i<table_size;i++)
  {
    // If state and event match
    if(*curstate_p==transitionTable[i].source && curevent==transitionTable[i].event)
    {
      // Transition to defined destination state
      *curstate_p = transitionTable[i].destination;
      // Exit the loop
      break;
    }
  }
}
