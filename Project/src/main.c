#include "plan_include.h"

// GLOBAL VARIABLES  ===========================================================
uint16_t        state_time_us = 0;      // Time in microseconds since last cycle
uint8_t         systick_update = 0;     // Did SysTick tick? 0=no, 1=yes
state           sys_state = ENTRY_STATE;
int LED_err_count = 0;

uint8_t         spi_rx_buffer[DATA_SIZE_FLEXSEA];       // Used for SPI rx
extern uint8_t  comm_str_1[COMM_STR_BUF_LEN];           // Used for SPI tx
uint8_t         usart_tx_buffer[DATA_SIZE_P2U];         // Used for UART tx
uint8_t         usart_rx_buffer[DATA_SIZE_U2P];         // Used for UART rx

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
    switch (sys_state)
    {
    case STATE_INITIALIZING:
      initialize();
      change_sys_state(&sys_state,EVENT_INITIALIZATION_SUCCESS);
      break;
    case STATE_WAITING_FOR_MANIFEST:
      // If card is detected in the socket
      if ((GPIO_EXTI_CD_PORT->IDR & GPIO_EXTI_CD_PIN))
      {
        // Move up to STATE_READING_MANIFEST
        
        change_sys_state(&sys_state,EVENT_EXTERNAL_MEMORY_DETECTED);
      }
      else //Do nothing; wait for the card to be put in
      {
        ;
      }
      change_sys_state(&sys_state,EVENT_EXTERNAL_MEMORY_DETECTED);              //REMOVE
      break;
    case STATE_INITIALIZING_MEMORY:
      break;
    case STATE_READING_MANIFEST:
      //change_sys_state(&sys_state,EVENT_MANIFEST_READ_SUCCESS);
      break;
    case STATE_INCORRECT_MANIFEST:
      break;
    case STATE_BUILDING_FSM:
      change_sys_state(&sys_state,EVENT_FSM_BUILD_SUCCESS);
      break;
    case STATE_WAIT_FOR_USER:
      change_sys_state(&sys_state,EVENT_USER_FSM_INITIALIZE);
      break;
    case STATE_INITIALIZING_FSM:
      change_sys_state(&sys_state,EVENT_FSM_INITIALIZED);
      break;
    case STATE_ACTIVE_CONTROL:
      change_spi_mode(SPI_MODE_MANAGE);
      while(sys_state==STATE_ACTIVE_CONTROL)
      {
        if (systick_update == 1)    // On SysTick update:
        {
          systick_update = 0;       // Reset systick_update flag
          switch(state_time_us)     // Assume one of the following states:
          {
            case SYSTICK*0:         // At t = 0us:
              update_Manage();      // - Communicate with Manage
              break;
            case SYSTICK*7:         // At t = 350us
              pack_P2U();           // Pack data to send upstream
              break;
            case SYSTICK*14:         // At t = 700us microseconds:
              update_User();        // - Communicate with user
              break;
            case SYSTICK*40:
              pack_U2P();           // Pack data to send downstream
              break;
          }
        }
      }
      break;
    case STATE_ERROR:
      while(sys_state==STATE_ERROR)
      {
        if (systick_update == 1)    // On SysTick update:
        {
          systick_update = 0;       // Reset systick_update flag
          LED_err_count++;
          if(LED_err_count>=ERROR_LED_TIME*FACTOR_us_PER_s/SYSTICK)
          {
            GPIO_ToggleBits(LEDx_PORT,LED2_PIN);
            LED_err_count = 0;
          }
        }
      }
      break;
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
  tx_cmd_ricnu_rw(TX_N_DEFAULT, 0, CTRL_NONE, 0, CHANGE, 0, 0, 0, 0);
  pack(P_AND_S_DEFAULT, FLEXSEA_EXECUTE_1, 0, &numb, comm_str_1);
  
  // Configure Plan peripherals
  RTC_config();         // Real Time Clock configuration
  LED_config();         // GPIOs - LEDs on the eval board (deprecate later)     //TODO
  GPIO_config();        // GPIOs - for SPI, USART, etc.
  NVIC_config();        // Nested Vector Interrupt Controller
  EXTI_config();        // External Interrupt Controller
  DMA_config();         // Direct Memory Access Controller
  SPI_config();         // Serial Peripheral Interface Controller
  USART_config();       // Universal Synch/Asynch Receiver/Transmitter
  
  // Prepare to initialize the SD Card
  change_spi_mode(SPI_MODE_SD_INIT);
  
  // Configure system state machine update rate
  set_tick(SYSTICK);
}




