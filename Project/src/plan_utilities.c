#include "plan_utilities.h"

extern uint8_t          spi_rx_buffer[DATA_SIZE_FLEXSEA];
extern uint8_t          comm_str_1[COMM_STR_BUF_LEN];
extern fsm              *FSM;
extern fsm_tracker      TR;

int32_t setpoint = -1;
int16_t g0=-1,g1=-1,g2=-1,g3=-1;




//==============================================================================        // Perhaps will move to fsm
// FUNCTION update_fsm()
//      - processes data from Manage
//      - catches events based on current state
//      - changes current state and mode
//==============================================================================
void update_fsm(void)
{
  int i = 0;
  uint8_t d[22];
  
  // Check data packet  ========================================================
  if(!(spi_rx_buffer[1]==(uint8_t)0x1D)) return; // Check if SPI packet contains the right-size data
  
  // Interpret data  ===========================================================
  for(i=0;i<=22;i++){d[i]=spi_rx_buffer[i+7];} // Isolate data and pack into a data buffer
  
  // For 16-bit (in example, : MSB = 11001100, LSB = 00011101)
  //  - take the 8-bit MSB                                                    //                            11001100
  //  - make it a 32-bit int but keep only bits from MSB                      // 00000000 00000000 00000000 11001100
  //  - left shift MSB by 8                                                   // 00000000 00000000 11001100 00000000
  //  - OR the MSB with LSB                                                   // 00000000 00000000 11001100 00011101
  //  - convert from two's complement to regular signed int
  //  - modify (for gyro, divide by 164) and save
  
  i = 0;
  float gx = (double)u2s((((((int)d[i+0 ]) &0xFF)<<8)|
                                 (((int)d[i+1 ]))&0xFF),16)/(double)164;
  float gy = (double)u2s((((((int)d[i+2 ]) &0xFF)<<8)|
                                 (((int)d[i+3 ]))&0xFF),16)/(double)164;
  float gz = (double)u2s((((((int)d[i+4 ]) &0xFF)<<8)|
                                 (((int)d[i+5 ]))&0xFF),16)/(double)164;
  float ax = (double)u2s((((((int)d[i+6 ]) &0xFF)<<8)|
                                 (((int)d[i+7 ]))&0xFF),16)/(double)8192;
  float ay = (double)u2s((((((int)d[i+8 ]) &0xFF)<<8)|
                                 (((int)d[i+9 ]))&0xFF),16)/(double)8192;
  float az = (double)u2s((((((int)d[i+10]) &0xFF)<<8)|
                                 (((int)d[i+11]))&0xFF),16)/(double)8192;
  float em = (double)u2s((((((int)d[i+12]) &0xFF)<<24)|
                                ((((int)d[i+13]) &0xFF)<<16)|
                                ((((int)d[i+14]) &0xFF)<<8)|
                                ((((int)d[i+15]) &0xFF))),32)*0.021973;
  float ej = (double)u2s((((((int)d[i+16]) &0xFF)<<24)|
                                ((((int)d[i+17]) &0xFF)<<16)|
                                ((((int)d[i+18]) &0xFF)<<8)|
                                ((((int)d[i+19]) &0xFF))),32)*0.021973;
  float cm = (double)u2s((((((int)d[i+20]) &0xFF)<<8)|
                                 (((int)d[i+21]))&0xFF),16);
  
  // Catch event  ==============================================================
  float data = 0, threshold = -1;
  int r = -1, condition = -1, prev_mode = -1, prev_state = -1;
  
  // Cycle through transitions of current state
  for(int tcnt=0;tcnt<(FSM->m[TR.cm].s[TR.cs].num_t);tcnt++)
  {
    // Get transition's channel value to analyze
    switch(FSM->m[TR.cm].s[TR.cs].t[tcnt].chan) 
      {
      case AX: data=ax; break;
      case AY: data=ay; break;
      case AZ: data=az; break;
      case GX: data=gx; break;
      case GY: data=gy; break;
      case GZ: data=gz; break;
      case EM: data=em; break;
      case EJ: data=ej; break;
      case CM: data=cm; break;
      }
      condition = FSM->m[TR.cm].s[TR.cs].t[tcnt].cond;
      threshold = FSM->m[TR.cm].s[TR.cs].t[tcnt].thres;
      
      // If relationship between data and transition's
      //        threshold matches condition ==> generate event
      if((condition==MR&&data>threshold)||
         (condition==LS&&data<threshold)||
         (condition==ME&&data>=threshold)||
         (condition==LE&&data<=threshold)||
         (condition==EQ&&data==threshold))
      {
        r = tcnt;               // Save this transition number
        break;
      }
  }
  if(r>=0) // If caught an event (transition number is not -1)
  {
    // Save previous mode and state
    prev_mode = TR.cm;
    prev_state = TR.cs;
    
    // Cycle through modes 
    for(int mcnt=0;mcnt<(FSM->num_m);mcnt++)
    {
      // Cycle through states to find the next state based on event id
      for(int scnt=0;scnt<(FSM->m[mcnt].num_s);scnt++)
      {
        // If this state matches the next state id in the caught event ==>
        if(FSM->m[mcnt].s[scnt].id_self==
           FSM->m[TR.cm].s[TR.cs].t[r].id_next)
        {
          TR.cm = mcnt; // Go to this state (save mode index and state index)
          TR.cs = scnt;
          break;        // Stop cycling through states
        }
      }
      // If went to next state, stop cycling through modes
      if(!(TR.cs==prev_state&&TR.cm==prev_mode)) break;
    }
  }
}

//==============================================================================
// FUNCTION calculate_gains()
//      - calculates new gains and packs them into a FlexSEA data packet
//==============================================================================
void calculate_gains(void)
{
  //uint16_t numb = 0;
  uint8_t control = 0;
  (void) control;
  
  if(TR.cs==1)
  {
    LED_state(LED_YEL,ON,CON);
    LED_state(LED_BLU,OFF,CON);
  }
  else if(TR.cs==2)
  {
    LED_state(LED_YEL,OFF,CON);
    LED_state(LED_BLU,ON,CON);
  }
  else
  {
    LED_state(LED_YEL,OFF,CON);
    LED_state(LED_BLU,OFF,CON);
  }
  
  
  switch(FSM->m[TR.cm].s[TR.cs].ctrl)
  {
  case CTRL_NON:
    control = CTRL_NONE;
    break;
  case CTRL_POS:
    control = CTRL_POSITION;
    break;
  case CTRL_CUR:
    control = CTRL_CURRENT;
    break;
  case CTRL_IMP:
    control = CTRL_IMPEDANCE;
    setpoint = (uint32_t)FSM->m[TR.cm].s[TR.cs].g[2]*45;    
    g0 = (uint16_t)FSM->m[TR.cm].s[TR.cs].g[0];
    g1 = (uint16_t)FSM->m[TR.cm].s[TR.cs].g[1];
    g2 = (uint16_t)FSM->m[TR.cm].s[TR.cs].g[3];
    g3 = (uint16_t)FSM->m[TR.cm].s[TR.cs].g[4];
    break; 
  }
  //prep_packet(0,control,setpoint,g0,g1,g2,g3);
  //tx_cmd_ricnu_rw(TX_N_DEFAULT,0,control,setpoint,CHANGE,g0,g1,g2,g3);
  //pack(P_AND_S_DEFAULT, FLEXSEA_EXECUTE_1, 0, &numb, comm_str_1);
}


//==============================================================================
// FUNCTION u2s()
//      - changes an unsigned integer (with specified size) to signed
//==============================================================================
int u2s(int data, int size)
{
  int thres = (1<<(size-1));
  if(data>thres) return (data-((thres<<1)-1));
  return data;
}


//==============================================================================
// FUNCTION GPIO_ToggleBits()
//      - toggles a GPIO output
//==============================================================================
void GPIO_ToggleBits(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  if((GPIOx->ODR&GPIO_Pin)!=Bit_RESET) //If output register bit is set
  {
    GPIOx->BRR = GPIO_Pin;      //Reset output pin bit
  }
  else
  {
    GPIOx->BSRR = GPIO_Pin;     //Set output pin bit
  }
  
}








