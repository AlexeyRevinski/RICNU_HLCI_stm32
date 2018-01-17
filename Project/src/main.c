#include "plan_include.h"

// GLOBAL VARIABLES  ===========================================================
uint16_t        state_time_us = 0;      // Time in microseconds since last cycle
uint8_t         systick_update = 0;     // Did SysTick tick? 0=no, 1=yes
state           sys_state = ENTRY_STATE;
mstate          mem_state = MEM_OUT;

int LED_err_count = 0;
int LED_mem_count = 0;

fsm             FSM_s   = NULL;
fsm             *FSM    = &FSM_s; // Initialize pointer to fsm structure
fsm_tracker     TR;

uint8_t         spi_rx_buffer[DATA_SIZE_FLEXSEA];       // Used for SPI rx
extern uint8_t  comm_str_1[COMM_STR_BUF_LEN];           // Used for SPI tx
uint8_t         usart_tx_buffer[DATA_SIZE_P2U];         // Used for UART tx
uint8_t         usart_rx_buffer[DATA_SIZE_U2P];         // Used for UART rx

int32_t setpoint = -1;
int16_t g0=-1,g1=-1,g2=-1,g3=-1;

// FUNCTION PROTOTYPES  ========================================================
void    initialize(void);       // System initialization function

//==============================================================================
// FUNCTION main()
//      - initializes the system
//      - implements system state machine
//==============================================================================
int main(void)
{
  while(1)
  {
    if(systick_update||sys_state==STATE_INITIALIZING)
    {
      systick_update = 0;             // Reset systick_update flag
      
      // Updating memory states
      switch (mem_state)
      {
      case MEM_OUT:
        GPIO_SetBits(LEDx_PORT,LED1_PIN);
        break;
      case MEM_IN:
        GPIO_ResetBits(LEDx_PORT,LED1_PIN);
        break;
      }
      
      // Updating system state
      switch (sys_state)
      {
      case STATE_INITIALIZING:
        initialize();
        break;
      case STATE_WAITING_FOR_MEMORY:
      case STATE_ERROR:
        LED_err_count++;
        if(LED_err_count>=ERROR_LED_TIME*FACTOR_us_PER_s/SYSTICK)
        {
          GPIO_ToggleBits(LEDx_PORT,LED2_PIN);
          LED_err_count = 0;
        }
        break;
      case STATE_BUILDING_FSM:
        if(!fsm_build()) if(!fsm_build()) if(!fsm_build())
        {
          change_sys_state(&sys_state,EVENT_FSM_BUILD_FAIL); break;
        }
        change_sys_state(&sys_state,EVENT_FSM_BUILD_SUCCESS);
        break;
      case STATE_WAIT_FOR_USER:
        break;
      case STATE_ACTIVE_CONTROL:
        switch(state_time_us)   // Assume one of the following states
        {
        case SYSTICK*0:         // At t = 0us:
          update_Manage();      // - Communicate with Manage
          break;
        case SYSTICK*7:         // At t = 350us
          pack_P2U();           // Pack data to send upstream
          break;
        case SYSTICK*10:        // At t = 500us
          update_fsm();         // Update state machine
          break;
        case SYSTICK*12:        // At t = 600us
          calculate_gains();    // Calculate gains and pack them into a FlexSEA packet
          break;
        case SYSTICK*14:        // At t = 700us:
          update_User();        // - Communicate with user
          break;
          /*
        case SYSTICK*40:        // At t = 2ms
          pack_U2P();           // Pack data to send downstream
          break;
          */
        }
        break;
      }
    }
  }
}

//==============================================================================
// FUNCTION initialize()
//      - initializes FlexSEA stack and makes an initial SPI packet
//      - configures system peripheral functionality
//      - sets system state machine update frequency (for use in main)
//==============================================================================
void initialize(void)
{
  // Initialize FlexSEA pointer stack and initial message to Manage
  init_flexsea_payload_ptr();
  uint16_t numb = 0;
  
  // Configure Plan peripherals
  RTC_config();         // Real Time Clock configuration
  LED_config();         // GPIOs - LEDs on the eval board (deprecate later)     //TODO
  GPIO_config();        // GPIOs - for SPI, USART, etc.
  NVIC_config();        // Nested Vector Interrupt Controller
  EXTI_config();        // External Interrupt Controller
  DMA_config();         // Direct Memory Access Controller
  SPI_config();         // Serial Peripheral Interface Controller
  USART_config();       // Universal Synch/Asynch Receiver/Transmitter

  // Put motor in no control mode
  tx_cmd_ricnu_rw(TX_N_DEFAULT, 0, CTRL_NONE, 0, CHANGE, 0, 0, 0, 0);
  pack(P_AND_S_DEFAULT, FLEXSEA_EXECUTE_1, 0, &numb, comm_str_1);    
  
  // Configure system state machine update rate
  set_tick(SYSTICK);
  
  // Proceed to next state
  if(SD_Detect())
  {
    change_sys_state(&sys_state,EVENT_EXTERNAL_MEMORY_DETECTED);
  }
  else
  {
    //change_sys_state(&sys_state,EVENT_EXTERNAL_MEMORY_DETECTED);
    change_sys_state(&sys_state,EVENT_EXTERNAL_MEMORY_DISCONNECTED);
  }
}




