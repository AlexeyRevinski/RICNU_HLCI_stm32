#include "plan_state_system.h"

static const transition transitionTable[] = {
  {STATE_INITIALIZING,          EVENT_INITIALIZATION_SUCCESS,           STATE_WAITING_FOR_MANIFEST},
  {STATE_INITIALIZING,          EVENT_INITIALIZATION_FAIL,              STATE_ERROR},
  {STATE_WAITING_FOR_MANIFEST,  EVENT_EXTERNAL_MEMORY_DETECTED,         STATE_INITIALIZING_MEMORY},     
  {STATE_INITIALIZING_MEMORY,   EVENT_MEMORY_INITIALIZATION_SUCCESS,    STATE_READING_MANIFEST},
  {STATE_READING_MANIFEST,      EVENT_MANIFEST_READ_SUCCESS,            STATE_BUILDING_FSM},
  {STATE_READING_MANIFEST,      EVENT_MANIFEST_READ_FAIL,               STATE_INCORRECT_MANIFEST},
  {STATE_INCORRECT_MANIFEST,    EVENT_EXTERNAL_MEMORY_DISCONNECTED,     STATE_WAITING_FOR_MANIFEST},
  {STATE_READING_MANIFEST,      EVENT_EXTERNAL_MEMORY_DISCONNECTED,     STATE_WAITING_FOR_MANIFEST},
  {STATE_BUILDING_FSM,          EVENT_FSM_BUILD_FAIL,                   STATE_INCORRECT_MANIFEST},
  {STATE_BUILDING_FSM,          EVENT_FSM_BUILD_SUCCESS,                STATE_WAIT_FOR_USER},
  {STATE_WAIT_FOR_USER,         EVENT_USER_FSM_INITIALIZE,              STATE_INITIALIZING_FSM},
  {STATE_INITIALIZING_FSM,      EVENT_FSM_INITIALIZED,                  STATE_ACTIVE_CONTROL},
  {STATE_ACTIVE_CONTROL,        EVENT_FLEXSEA_ERROR,                    STATE_ERROR},
  {STATE_ERROR,                 EVENT_MANIFEST_READ_SUCCESS,            STATE_ACTIVE_CONTROL},          // TODO delete this transition
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
