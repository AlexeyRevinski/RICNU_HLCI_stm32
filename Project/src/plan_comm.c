#include "plan_comm.h"

extern uint8_t spi_rx_buffer[DATA_SIZE_FLEXSEA];
extern uint8_t usart_tx_buffer[DATA_SIZE_P2U];
extern uint8_t usart_rx_buffer[DATA_SIZE_U2P];

extern uint8_t comm_str_1[COMM_STR_BUF_LEN];

//==============================================================================
// FUNCTION update_Manage()
//      - communicates with FlexSEA Manage board via SPI (one packet)
//==============================================================================
void update_Manage(void)
{
  // Clear data register
  while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == SET){SPI_ReceiveData8(SPIx);}
  // Start packet transmission sequence - drive chip select low
  GPIO_ResetBits(GPIO_SPIx_SS_MN_PORT,GPIO_SPIx_SS_MN_PIN);
  GPIO_SetBits(GPIO_SPIx_EN_MN_PORT,GPIO_SPIx_EN_MN_PIN);
  // Start DMA transfer from memory to SPI->DR
  DMA_Cmd(SPIx_DMA_RX_CHANNEL, ENABLE);
  DMA_Cmd(SPIx_DMA_TX_CHANNEL, ENABLE);
}


//==============================================================================
// FUNCTION update_User()
//      - communicates with user app: (UART to BT121)
//==============================================================================
void update_User(void)
{
  // Start DMA transfer from memory to USART->TDR
  DMA_Cmd(USARTx_DMA_RX_CHANNEL, ENABLE);
  DMA_Cmd(USARTx_DMA_TX_CHANNEL, ENABLE);
}


//==============================================================================
// FUNCTION pack_P2U()
//      - packs data to send to User over UART
//==============================================================================
void pack_P2U(void)
{
  int i=0;
  if(spi_rx_buffer[1]==0x1D)
  {  
    for(i=0;i<=22;i++)
    {
      usart_tx_buffer[i]=spi_rx_buffer[i+6];
    }
  }
  if(usart_tx_buffer[23]==0){usart_tx_buffer[23]=1;}
  else if(usart_tx_buffer[23]==1){usart_tx_buffer[23]=0;}
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
  uint16_t numb = 0;
  tx_cmd_ricnu_rw(TX_N_DEFAULT, 0, ctrl, pwm, CHANGE, 0, 0, 0, 0);
  pack(P_AND_S_DEFAULT, FLEXSEA_EXECUTE_1, 0, &numb, comm_str_1);
}

