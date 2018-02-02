#include "plan_state_system.h"

static const transition transitionTable[] = {
  //{STATE_INITIALIZING,  EVENT_INITIALIZED,              STATE_INACTIVE},
  {STATE_INITIALIZING,  EVENT_INITIALIZED,              STATE_ACTIVE},
  {STATE_INACTIVE,      EVENT_START,                    STATE_ACTIVE},
  {STATE_ACTIVE,        EVENT_STOP,                     STATE_INACTIVE},
  {STATE_ACTIVE,        EVENT_FLEXSEA_ERROR,            STATE_ERROR},
  // Other error conditions...
};

static int table_size = (int) sizeof(transitionTable)/sizeof(transition);

void change_sys_state(state* curstate_p, event curevent)
{
  static int i=0;
  for(i=0;i<table_size;i++)                     // Cycle through table entries
  {
    if(*curstate_p==transitionTable[i].source &&        
       curevent==transitionTable[i].event)      // If state and event match
    {
      *curstate_p = transitionTable[i].destination;     // Transition
      break;                                            // Exit the loop
    }
  }
  switch(*curstate_p)
  {
  case STATE_INACTIVE:
    LED_state(LED_RED,OFF,CON);         // No error             (RED off)
    LED_state(LED_YEL,OFF,CON);         // No data logging      (YEL off)
    LED_state(LED_GRN, ON,CON);         // Ready                (GRN on)
    break;
  case STATE_ACTIVE:
    LED_state(LED_RED,OFF,CON);         // No error             (RED off)
    if(1){LED_state(LED_YEL,OFF,CON);}   // Data logging         (YEL on)        // Implement data logging verification
    LED_state(LED_GRN, ON,TOG);         // Active               (GRN flash)
    break;
  case STATE_ERROR:
    LED_state(LED_RED, ON,TOG);         // Error                (RED on)
    LED_state(LED_YEL,OFF,CON);         // No data logging      (YEL off)
    LED_state(LED_GRN,OFF,CON);         // Not ready or active  (GRN off)
    break;
  }
}
