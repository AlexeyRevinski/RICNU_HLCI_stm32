#include "plan_include.h"

// GLOBAL VARIABLES  ===========================================================
uint16_t        state_time_us = 0;      // Time in microseconds since last cycle
uint8_t         systick_update = 0;     // Did SysTick tick? 0=no, 1=yes
mstate          mem_state = MEM_OUT;
FATFS 			fs;


uint32_t		testtimer = 0;
extern	FIL		logfile;

//==============================================================================
// FUNCTION main()
//      - initializes the system
//      - implements system state machine
//==============================================================================
int main(void)
{
  state sys_state = STATE_INITIALIZING;         // State variable

  // PERIPHERAL CONFIGURATION --------------------------------------------------
  hardware_config();    // Configures all on-chip hardware
  f_mount(&fs,"", 0);                           // Mount the default drive
  log_init();

  // FLEXSEA STACK AND SYSTEM INITIALIZATION -----------------------------------
  prep_packet(0,CTRL_NONE,0,0,0,0,0);           // Prepare NO CONTROL packet
  update(MANAGE);                               // Make this blocking!
  //check that Execute changed control to none

  // Mount file system


  testtimer = 32;

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
  change_sys_state(&sys_state,EVENT_INITIALIZED);       // Initialized state
  log_time_set(0,0,0);
  // POST-INITIALIZATION STATE MACHINE -----------------------------------------
  while(1)
  {
    if(systick_update)  		// On SysTick update - every 50us
    {
      systick_update = 0;       // Reset update flag
      switch (sys_state)        // Take an action based on system state
      {
      // INACTIVE STATE
      case STATE_INACTIVE:
        break;

      // ERROR STATE
      case STATE_ERROR:
        break;
        
      // ACTIVE STATE
      case STATE_ACTIVE:                                                        // Time all these
			switch(state_time_us)   // Do one of the following (time based):
			{
			case SYS_TICK_US*0:     // At t = 0us:
			  //update(MANAGE);               // - Communicate with Manage
			  break;

			case SYS_TICK_US*7:     // At t = 350us
			  //unpack(MANAGE);               // Pack data to send upstream
			  break;

			case SYS_TICK_US*10:	// At t = 500us
				if(testtimer)
				  {
					  log_generate();
					  log_file_append();
					  if(testtimer==1) f_close(&logfile);
					  testtimer--;
				  }
				break;

			case SYS_TICK_US*20:    // At t = 1ms
			  //fsm_update();               // Update state machine
			  break;

			case SYS_TICK_US*14:    // At t = 700us:
			  //update(USER);                // Communicate with user
			  break;

			default:
				break;
			}
        break;
        default:
        	break;
      }
    }
  }
}
