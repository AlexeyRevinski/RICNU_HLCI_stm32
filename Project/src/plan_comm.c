#include "plan_comm.h"

uint8_t         spi_rx_buffer[DATA_SIZE_FLEXSEA];       // Used for SPI MN rx   // ALL OF THIS SHOULD GO TO COMM
extern uint8_t  comm_str_1[COMM_STR_BUF_LEN];           // Used for SPI MN tx
uint8_t         usart_tx_buffer[DATA_SIZE_P2U];         // Used for UART tx
uint8_t         usart_rx_buffer[DATA_SIZE_U2P];         // Used for UART rx
uint8_t         spi_payload[DATA_SIZE_FLEXSEA];
uint8_t         spi_rx_tmp[DATA_SIZE_FLEXSEA];

ricnu_data rndata;



void prep_packet(uint8_t offset, uint8_t ctrl, int32_t sp, int16_t gain0, \
                 int16_t gain1, int16_t gain2, int16_t gain3)
{
  tx_cmd_ricnu_rw(TX_N_DEFAULT, offset, ctrl, sp,
                  CHANGE, gain0, gain1, gain2, gain3);
  pack(P_AND_S_DEFAULT, FLEXSEA_EXECUTE_1, 0, 0, comm_str_1);
}



//==============================================================================
// FUNCTION update()
//      - starts DMA-assisted communication to a specified device
//==============================================================================
void update(device dev)
{
  switch (dev)
  {
  case MANAGE:
    // Clear data register by reading data until none left
    while(SPI_I2S_GetFlagStatus(SPI_MN, SPI_I2S_FLAG_RXNE) == SET)
    {
      SPI_I2S_ReceiveData(SPI_MN);
    }
    
    // Select Manage Board
    GPIO_ResetBits(GPIO_MN_NSS_PORT,GPIO_MN_NSS_PIN);
    
    // Start DMA transfer from memory to SPI->DR
    DMA_Cmd(SPI_MN_DMA_RX_CHAN, ENABLE);
    DMA_Cmd(SPI_MN_DMA_TX_CHAN, ENABLE);
    break;
  case USER:
    // Start DMA transfer from memory to USART->DR
    DMA_Cmd(USART_BT_DMA_RX_CHAN, ENABLE);
    DMA_Cmd(USART_BT_DMA_TX_CHAN, ENABLE);
    break;
  }
}


//==============================================================================
// FUNCTION pack_P2U()
//      - packs data to send to User over UART
//==============================================================================
int unpack(device dev)
{
  switch(dev)
  {
  case MANAGE:
    if(unpack_payload(spi_rx_buffer,spi_rx_tmp,spi_payload)>0)
    {
    	{
    		uint16_t index = 0;
    		uint8_t offset = 0;
			index = P_DATA1;
			offset = spi_payload[index++];

			if(offset == 0)
			{
				//Typical Execute variables, + new encoders:
				rndata.gx = (int16_t) REBUILD_UINT16(spi_payload, &index);
				rndata.gy = (int16_t) REBUILD_UINT16(spi_payload, &index);
				rndata.gz = (int16_t) REBUILD_UINT16(spi_payload, &index);
				rndata.ax = (int16_t) REBUILD_UINT16(spi_payload, &index);
				rndata.ay = (int16_t) REBUILD_UINT16(spi_payload, &index);
				rndata.az = (int16_t) REBUILD_UINT16(spi_payload, &index);
				rndata.em = (int32_t) REBUILD_UINT32(spi_payload, &index);
				rndata.ej = (int32_t) REBUILD_UINT32(spi_payload, &index);
				rndata.cu = (int16_t) REBUILD_UINT16(spi_payload, &index);
			}
			else if(offset == 1)
			{
				;
			}
    	}
      //struct execute_s ex2; ricnu_1.ex = &ex2;    // point rn->ex to a declared structure
      //struct strain_s  st2; ricnu_1.st = &st2;    // point rn->st to a declared structure
      //rx_cmd_ricnu_rr(spi_payload,0);             // unpack payload using RICNU function
      for(int i=0;i<=24;i++){usart_tx_buffer[i]=spi_payload[i+4];}    
      return COMM_OK;
    }
  case USER:
    break;
  }
  return COMM_ERR;
}

/*
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
*/

