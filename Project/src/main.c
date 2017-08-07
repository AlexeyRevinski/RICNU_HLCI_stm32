#include "plan_include.h"


// FUNCTION PROTOTYPES  ========================================================
void    initialize(void);       // System initialization function
void initial_delay(void);

// GLOBAL VARIABLES  ===========================================================
uint16_t        state_time_us = 0;      // Time in microseconds since last cycle
uint8_t         systick_update = 0;     // Did SysTick tick? 0=no, 1=yes
uint8_t         spi_rx_buffer[DATA_SIZE_FLEXSEA]; // For tx, comm_str_1 is used
uint8_t         usart_tx_buffer[DATA_SIZE_P2U];
uint8_t         usart_rx_buffer[DATA_SIZE_U2P];

int delay = 0;

extern uint8_t comm_str_1[COMM_STR_BUF_LEN];

//==============================================================================
// FUNCTION main()
//      - initializes the system
//      - implements a time-based state machine
//==============================================================================
int main(void)
{
  initialize();         // Initialize software and hardware  
  while(1)
  {
    if (systick_update == 1)    // On SysTick update:
    {
      systick_update = 0;       // Reset systick_update flag
      switch(state_time_us)     // Assume one of the following states:
      {
        case SYSTICK*0:         // At t = 0us:
          GPIO_SetBits(LEDx_PORT,LED2_PIN);
          update_Manage();      // - Communicate with Manage
          GPIO_ResetBits(LEDx_PORT,LED2_PIN);
          break;
        case SYSTICK*7:         // At t = 350us
          GPIO_SetBits(LEDx_PORT,LED2_PIN);
          pack_P2U();
          GPIO_ResetBits(LEDx_PORT,LED2_PIN);
          break;
        case SYSTICK*14:         // At t = 700us microseconds:
          GPIO_SetBits(LEDx_PORT,LED2_PIN);
          update_User();        // - Communicate with user
          GPIO_ResetBits(LEDx_PORT,LED2_PIN);
          break;
        case SYSTICK*40:
          GPIO_SetBits(LEDx_PORT,LED2_PIN);
          pack_U2P();
          GPIO_ResetBits(LEDx_PORT,LED2_PIN);
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
  tx_cmd_ricnu_rw(TX_N_DEFAULT, 0, CTRL_NONE, 0, CHANGE, 0, 0, 0, 0);
  pack(P_AND_S_DEFAULT, FLEXSEA_EXECUTE_1, 0, &numb, comm_str_1);
  
  // Configure Plan peripherals
  RCC_confg();          // Reset and Clock Control
  SYSCFG_config();      // Pin remapping
  LED_config();         // GPIOs - LEDs on the eval board (deprecate later)           //TODO
  GPIO_config();        // GPIOs - for SPI, USART, etc.
  NVIC_config();        // Nested Vector Interrupt Controller
  DMA_config();         // Direct Memory Access Controller
  SPI_config();         // Serial Peripheral Interface Controller
  USART_config();       // Universal Synch/Asynch Receiver/Transmitter
  
  // Configure system state machine update rate
  set_tick(SYSTICK);
  //initial_delay();
}

void initial_delay(void)
{
  while(delay<=140000){;}
}




