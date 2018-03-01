#include "plan_include.h"

// GLOBAL VARIABLES  ===========================================================
uint16_t        state_time_us = 0;      // Time in microseconds since last cycle
uint8_t         systick_update = 0;     // Did SysTick tick? 0=no, 1=yes
mstate          mem_state = MEM_OUT;
FATFS 			fs;


uint32_t		testtimer = 0;
extern	FIL		logfile;

extern uint8_t		g_offset;

#define CALIBTIME 500

extern ricnu_data rndata;
int64_t straincalib[] = {0,0,0,0,0,0};
int32_t straincal[] = {0,0,0,0,0,0};
uint32_t calibtimer = CALIBTIME;
state sys_state = STATE_INITIALIZING;         // State variable
uint8_t offsettimer = 0;

//==============================================================================
// FUNCTION main()
//      - initializes the system
//      - implements system state machine
//==============================================================================
int main(void)
{
	//FRESULT fr;



  // PERIPHERAL CONFIGURATION --------------------------------------------------
  hardware_config();    // Configures all on-chip hardware
  f_mount(&fs,"", 0);                           // Mount the default drive
  log_init();

  // FLEXSEA STACK AND SYSTEM INITIALIZATION -----------------------------------
  //prep_packet(0,CTRL_NONE,0,0,0,0,0);           // Prepare NO CONTROL packet
  prep_packet(1,CTRL_NONE,0,0,0,0,0);           // Prepare NO CONTROL packet
  update(MANAGE);                               // Make this blocking!
  //check that Execute changed control to none

  // Mount file system


  testtimer = 4000;

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
      case STATE_CALIBRATION:

    	  switch(state_time_us)
    	  {
    	  case SYS_TICK_US*0:
		  	  if(calibtimer)
		  	  {
				  update(MANAGE);
				  unpack(MANAGE);

				  if (calibtimer<(CALIBTIME-10))	// Discard first few packets
				  {
					  for (int i=0;i<6;i++)
					  {
						  straincalib[i]+=rndata.st[i];
					  }
				  }
				  calibtimer--;
		  	  }
		  	  else
		  	  {
		  		for (int i=0;i<6;i++)
				  {
					  straincalib[i]/=(CALIBTIME-10);
					  straincal[i] = (int32_t) straincalib[i];
				  }
		  		change_sys_state(&sys_state,EVENT_CALIBRATED);       // Initialized state
		  	  }
			  break;
    	  }


    	  break;
        
      // ACTIVE STATE
      case STATE_ACTIVE:                                                        // Time all these

    	  /*
    	  GPIO_ResetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN);
    	  update(MANAGE);
    	  unpack(MANAGE);
    	  fsm_update();
    	  update(USER);
    	  if(testtimer)
		  {
			  log_generate();
			  log_file_append();
			  if(testtimer==1)
			  {
				  f_close(&logfile);
			  }
			  testtimer--;
		  }
    	  GPIO_SetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN);
    	  //LED_state(LED_BLU,OFF,CON);
*/

			switch(state_time_us)   // Do one of the following (time based):
			{
			case SYS_TICK_US*0:     // At t = 0us:

			/*
			if(!g_offset)
			{
			GPIO_ResetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN);
				LED_state(LED_BLU,ON,CON);
			}
			else
			{
				GPIO_ResetBits(GPIO_LED_2_PORT,GPIO_LED_2_PIN);
				LED_state(LED_YEL,ON,CON);
			}
			*/
				prep_packet(g_offset,CTRL_NONE,0,0,0,0,0);           // Prepare NO CONTROL packet
				//g_offset=!g_offset;
			  update(MANAGE);               // - Communicate with Manage
			  unpack(MANAGE);
			  update(USER);
			  fsm_update();
			  /*
			  offsettimer--;
			  GPIO_SetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN);
			  GPIO_SetBits(GPIO_LED_2_PORT,GPIO_LED_2_PIN);
			  LED_state(LED_BLU,OFF,CON);
			  LED_state(LED_YEL,OFF,CON);
			  */
			  break;

			case SYS_TICK_US*7:     // At t = 350us
			                 // Pack data to send upstream
			  break;

			  /*
			case SYS_TICK_US*10:	// At t = 500us
				if(testtimer)
				  {
					  log_generate();
					  log_file_append();
					  if(testtimer==1)
					  {
						  f_close(&logfile);
					  }
					  testtimer--;
				  }
				break;
*/
			case SYS_TICK_US*20:    // At t = 1ms
			                 // Update state machine
			  break;

			case SYS_TICK_US*14:    // At t = 700us:
			                 // Communicate with user
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
