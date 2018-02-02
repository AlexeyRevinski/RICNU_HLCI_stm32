#include "plan_comm.h"

uint8_t         spi_rx_buffer[DATA_SIZE_FLEXSEA];       // Used for SPI MN rx   // ALL OF THIS SHOULD GO TO COMM
extern uint8_t  comm_str_1[COMM_STR_BUF_LEN];           // Used for SPI MN tx
uint8_t         usart_tx_buffer[DATA_SIZE_P2U];         // Used for UART tx
uint8_t         usart_rx_buffer[DATA_SIZE_U2P];         // Used for UART rx
uint8_t         spi_payload[DATA_SIZE_FLEXSEA];
uint8_t         spi_rx_tmp[DATA_SIZE_FLEXSEA];

void prep_packet(uint8_t offset, uint8_t ctrl, int32_t sp, int16_t gain0, \
                 int16_t gain1, int16_t gain2, int16_t gain3)
{
  tx_cmd_ricnu_rw(TX_N_DEFAULT, offset, ctrl, sp,
                  CHANGE, gain0, gain1, gain2, gain3);
  pack(P_AND_S_DEFAULT, FLEXSEA_EXECUTE_1, 0, 0, comm_str_1);
}

//==============================================================================
// FUNCTION update_Manage()
//      - communicates with FlexSEA Manage board via SPI (one packet)
//==============================================================================
void update_Manage(void)
{
  // Clear data register by reading data until none left
  while(SPI_I2S_GetFlagStatus(SPI_MN, SPI_I2S_FLAG_RXNE) == SET)
  {
    SPI_I2S_ReceiveData(SPI_MN);
  }  
  GPIO_ResetBits(GPIO_MN_NSS_PORT,GPIO_MN_NSS_PIN);     // Select Manage Board
  
  // Start DMA transfer from memory to SPI->DR
  DMA_Cmd(SPI_MN_DMA_RX_CHAN, ENABLE);
  DMA_Cmd(SPI_MN_DMA_TX_CHAN, ENABLE);
}


//==============================================================================
// FUNCTION update_User()
//      - communicates with user app: (UART to BT121)
//==============================================================================
void update_User(void)
{
  // Start DMA transfer from memory to USART->TDR
  DMA_Cmd(USART_BT_DMA_RX_CHAN, ENABLE);
  DMA_Cmd(USART_BT_DMA_TX_CHAN, ENABLE);
}


//==============================================================================
// FUNCTION pack_P2U()
//      - packs data to send to User over UART
//==============================================================================
int pack_P2U(void)                                                             // BUILD IN A ROBUST CHECKING MECHANISM
{
  if(unpack_payload(spi_rx_buffer,spi_rx_tmp,spi_payload)>0)
  {
    struct execute_s ex2; ricnu_1.ex = &ex2; // set rn->ex to point to declared structure
    struct strain_s  st2; ricnu_1.st = &st2; // set rn->st to point to declared structure
    rx_cmd_ricnu_rr(spi_payload,0);
    int i=0;
    //if(spi_rx_tmp[1]==0x1D) // If SPI packet contains the right-size data
    //{
      // Isolate data and pack into USART buffer
      for(i=0;i<=24;i++){usart_tx_buffer[i]=spi_payload[i+4];}    
    //}
    return COMM_OK;
  }
  return COMM_ERR;
}


//==============================================================================
// FUNCTION pack_U2P()
//      - packs data received from User over UART
//==============================================================================
void pack_U2P(void)
{
  int ctrl = CTRL_NONE;
  int pwm = 0;
  if(((usart_rx_buffer[0]>>(7-POS_MOTOR_ON_OFF))&1))
  {
    ctrl = CTRL_OPEN;
  }
  else if (!((usart_rx_buffer[0]>>(7-POS_MOTOR_ON_OFF))&1))
  {
    ctrl = CTRL_NONE;
    pwm = 0;
  }
  if(((usart_rx_buffer[0]>>(7-POS_MOTOR_DIRECT))&1))
  {
    pwm = -800;
  }
  else if (!((usart_rx_buffer[0]>>(7-POS_MOTOR_DIRECT))&1))
  {
    pwm = 800;
  }
  
  //uint16_t numb = 0;
  //prep_packet(0,ctrl,pwm,0,0,0,0);
  //tx_cmd_ricnu_rw(TX_N_DEFAULT, 0, ctrl, pwm, CHANGE, 0, 0, 0, 0);
  //pack(P_AND_S_DEFAULT, FLEXSEA_EXECUTE_1, 0, &numb, comm_str_1);
  (void) ctrl;
  (void) pwm;
}

