#include "plan_include.h"

// GLOBAL VARIABLES  ===========================================================
uint16_t        state_time_us = 0;      // Time in microseconds since last cycle
uint8_t         systick_update = 0;     // Did SysTick tick? 0=no, 1=yes
state           sys_state = STATE_INITIALIZING;
mstate          mem_state = MEM_OUT;

// FUNCTION PROTOTYPES  ========================================================
void initialize(void);

//==============================================================================
// FUNCTION main()
//      - initializes the system
//      - implements system state machine
//==============================================================================
int main(void)
{
  initialize(); // System initialization
  while(1)
  {
    if(systick_update)  // If system update flag is set
    {
      systick_update = 0;       // Reset update flag      
      switch (sys_state)        // Take an action based on system state
      {
      // INACTIVE STATE ========================================================
      case STATE_INACTIVE:
        break;
        
      // ERROR STATE ===========================================================
      case STATE_ERROR:
        break;
        
      // ACTIVE STATE ==========================================================
      case STATE_ACTIVE:                                                        // Time all these
        switch(state_time_us)   // Do one of the following (time based):
        {
        case SYS_TICK_US*0:     // At t = 0us:
          update_Manage();              // - Communicate with Manage
          break;
          
        case SYS_TICK_US*7:     // At t = 350us
          pack_P2U();                   // Pack data to send upstream
          break;
          
        case SYS_TICK_US*10:    // At t = 500us
          //update_fsm();                 // Update state machine
          break;
          
        case SYS_TICK_US*12:    // At t = 600us
          //calculate_gains();            // Calculate gains and pack 
          break;                        //   them into a FlexSEA packet
          
        case SYS_TICK_US*14:    // At t = 700us:
          update_User();                // Communicate with user
          break;
        }
        break;
      }
    }
  }
}

//==============================================================================
// FUNCTION initialize()
//      - initializes hardware peripherals on STM32F103
//      - initializes FlexSEA stack
//      - sends an initial SPI packet to FlexSEA Manage (no control)
//      - builds an FSM structure based on fsm.json file on the SD Card
//    This is a blocking function. To continue back to main:
//      - Peripherals must configure correctly AND
//      - FlexSEA Manage and Execute must be responsive AND
//      - SD Card must be inserted and containing a properly-written FSM file
//    Otherwise, program waits within this function until all three occur.
//==============================================================================
void initialize(void)
{
  // PERIPHERAL CONFIGURATION --------------------------------------------------
  hardware_config();    // Configures all on-chip hardware
  
  // FLEXSEA STACK AND SYSTEM INITIALIZATION -----------------------------------
  //init_flexsea_payload_ptr();                   // Initialize FlexSEA stack
  prep_packet(0,CTRL_NONE,0,0,0,0,0);           // Prepare NO CONTROL packet
  update_Manage();
                                               // Make sure FlexSEA accepts    // Develop fsm_build-like function
                                                //   and responds               // Check if accepted!!!
                                                                                // If not, keep trying or maybe go into STOP mode
  // FINITE STATE MACHINE BUILDING ---------------------------------------------
  while(!(fsm_build()|fsm_build()|fsm_build())) // Try 3 times. If fail - STOP
  {                                             //   mode. Interrupt - try again
    LED_state(LED_RED,ON,CON);LED_update();     // Turn on RED LED
    PWR_EnterSTOPMode(PWR_Regulator_LowPower,   // Go into STOP mode
                      PWR_STOPEntry_WFI);
    RCC_config();       // Once awake, configure clocks anew and try again
  }
  
  // INITIALIZATION COMPLETE LED SEQUENCE --------------------------------------
  LED_rainbow();                                    // LED sequence (blocking)
  change_sys_state(&sys_state,EVENT_INITIALIZED); // Initialized state
}