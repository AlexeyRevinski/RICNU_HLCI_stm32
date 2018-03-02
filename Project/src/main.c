#include "plan_include.h"

// GLOBAL VARIABLES  ===========================================================
FATFS 			fs;
state sys_state = STATE_INITIALIZING;         // State variable
uint8_t offsettimer = 0;
extern flexsea_ctrl fc;
extern lstate log_state;

//==============================================================================
// FUNCTION main()
//      - initializes the system
//      - implements system state machine
//==============================================================================
int main(void)
{
  // PERIPHERAL CONFIGURATION --------------------------------------------------
  hardware_config();    // Configures all on-chip hardware
  f_mount(&fs,"", 0);                           // Mount the default drive
  log_init();

  // FLEXSEA STACK AND SYSTEM INITIALIZATION -----------------------------------
  fc.offset = 1;
  fc.control = CTRL_NONE;
  fc.setpoint = 0;
  fc.g0 = 0;
  fc.g1 = 0;
  fc.g2 = 0;
  fc.g3 = 0;
  comm_send_manage();
  //check that Execute changed control to none


  // DEVICE CONTROL FINITE STATE MACHINE BUILD ---------------------------------
  while(1)
  {
    if(fsm_build()) break;        // If successfully read and built fsm, break
    if(fsm_build()) break;        // Otherwise, try again
    if(fsm_build()) break;
    
    // If unsuccessful - SD Card absent or file error. Go into stop mode
    LED_state(LED_RED,ON,CON);LED_update();     // Turn on RED LED
    PWR_EnterSTOPMode(PWR_Regulator_LowPower,   // Go into STOP mode
                      PWR_STOPEntry_WFI);
    RCC_config();                       // Once awake, configure clocks anew
    while(SD_Init()!=UNKNOWN);          // Go through init until success
  }
  
  // INITIALIZATION COMPLETE LED SEQUENCE --------------------------------------
  LED_rainbow();                                        // LED sequence BLOCK
  log_time_set(0,0,0);
  change_sys_state(&sys_state,EVENT_INITIALIZED);       // Initialized state

  // INFINITE WHILE LOOP -----------------------------------------
  while(1)
  {
	  if(sys_state==STATE_ACTIVE&&log_state==LOG_ON)
	  {
		  log_file_append();
	  }
	  else
	  {
		  __WFI();
	  }
  }
}
