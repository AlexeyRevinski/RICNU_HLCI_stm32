#include "plan_comm.h"

uint8_t         spi_rx_buffer[DATA_SIZE_FLEXSEA];       // Used for SPI MN rx   // ALL OF THIS SHOULD GO TO COMM
extern uint8_t  comm_str_1[COMM_STR_BUF_LEN];           // Used for SPI MN tx
uint8_t         usart_tx_buffer[DATA_SIZE_P2U];         // Used for UART tx
uint8_t         usart_rx_buffer[DATA_SIZE_U2P];         // Used for UART rx
uint8_t         spi_payload[DATA_SIZE_FLEXSEA];
uint8_t         spi_rx_tmp[DATA_SIZE_FLEXSEA];

extern uint8_t		g_offset;
extern 			state sys_state;
ricnu_data rndata;

extern int32_t straincal[];
extern int32_t imucal[];


int64_t loadCellM[6][6] = {{ 9008,	101,	-9026,	-60,	121,	41},		// Fx, x10
						  { 5328,	89,		5472,	72,		-10849,	32},		// Fy, x10
						  {-52,		4990,	-67,	4979,	126,	4976},		// Fz, x10
						  { 261,	9833,	421,	-9659,	-680,	263},		// Mx, x1000
						  { 246,	5923,	-504,	5766,	-294,	-11289},	// My, x1000
						  {-19219,	-176,   -17030,	-134,   -18937,	-101}};		// Mz, x1000


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
	int16_t rawstrain[6] = {0,0,0,0,0,0};
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
					if(sys_state!=STATE_CALIBRATION)
					{
						g_offset = 1;

						rndata.gx-=imucal[0];
						rndata.gy-=imucal[1];
						rndata.gz-=imucal[2];
						rndata.ax-=imucal[3];
						rndata.ay-=imucal[4];
						rndata.az-=imucal[5];
						rndata.em-=imucal[6];
						rndata.ej-=imucal[7];
						rndata.cu-=imucal[8];
						usart_tx_buffer[0] = offset;
						usart_tx_buffer[1] = (uint8_t)((rndata.gx>>8) & 0xFF);
						usart_tx_buffer[2] = (uint8_t)((rndata.gx) & 0xFF);
						usart_tx_buffer[3] = (uint8_t)((rndata.gy>>8) & 0xFF);
						usart_tx_buffer[4] = (uint8_t)((rndata.gy) & 0xFF);
						usart_tx_buffer[5] = (uint8_t)((rndata.gz>>8) & 0xFF);
						usart_tx_buffer[6] = (uint8_t)((rndata.gz) & 0xFF);
						usart_tx_buffer[7] = (uint8_t)((rndata.ax>>8) & 0xFF);
						usart_tx_buffer[8] = (uint8_t)((rndata.ax) & 0xFF);
						usart_tx_buffer[9] = (uint8_t)((rndata.ay>>8) & 0xFF);
						usart_tx_buffer[10] = (uint8_t)((rndata.ay) & 0xFF);
						usart_tx_buffer[11] = (uint8_t)((rndata.az>>8) & 0xFF);
						usart_tx_buffer[12] = (uint8_t)((rndata.az) & 0xFF);
						usart_tx_buffer[13] = (uint8_t)((rndata.em>>24) & 0xFF);
						usart_tx_buffer[14] = (uint8_t)((rndata.em>>16) & 0xFF);
						usart_tx_buffer[15] = (uint8_t)((rndata.em>>8) & 0xFF);
						usart_tx_buffer[16] = (uint8_t)((rndata.em) & 0xFF);
						usart_tx_buffer[17] = (uint8_t)((rndata.ej>>24) & 0xFF);
						usart_tx_buffer[18] = (uint8_t)((rndata.ej>>16) & 0xFF);
						usart_tx_buffer[19] = (uint8_t)((rndata.ej>>8) & 0xFF);
						usart_tx_buffer[20] = (uint8_t)((rndata.ej) & 0xFF);
						usart_tx_buffer[21] = (uint8_t)((rndata.cu>>8) & 0xFF);
						usart_tx_buffer[22] = (uint8_t)((rndata.cu) & 0xFF);
						usart_tx_buffer[23] = (uint8_t)((0) & 0xFF);
						usart_tx_buffer[24] = (uint8_t)((0) & 0xFF);

						/*
						for(int i=0;i<=24;i++){usart_tx_buffer[i]=spi_payload[i+4];}
						*/
					}
					//


				}
				else if(offset == 1)
				{

					rawstrain[0] = ((int16_t) ((REBUILD_UINT16(spi_payload, &index))>>4));index--;
					rawstrain[1] = ((int16_t) (((REBUILD_UINT16(spi_payload, &index)<<4))>>4));
					rawstrain[2] = ((int16_t) ((REBUILD_UINT16(spi_payload, &index))>>4));index--;
					rawstrain[3] = ((int16_t) (((REBUILD_UINT16(spi_payload, &index)<<4))>>4));
					rawstrain[4] = ((int16_t) ((REBUILD_UINT16(spi_payload, &index))>>4));index--;
					rawstrain[5] = ((int16_t) (((REBUILD_UINT16(spi_payload, &index)<<4))>>4));

					for (int i = 0; i<6;i++)
					{
						rawstrain[i]&=0x0FFF;
						rawstrain[i]-=0x7FF;
					}


					for (int i=0;i<6;i++)
					{
						rndata.st[i] =
						(rawstrain[0] * loadCellM[i][0]) +
						(rawstrain[1] * loadCellM[i][1]) +
						(rawstrain[2] * loadCellM[i][2]) +
						(rawstrain[3] * loadCellM[i][3]) +
						(rawstrain[4] * loadCellM[i][4]) +
						(rawstrain[5] * loadCellM[i][5]);
						if(sys_state!=STATE_CALIBRATION)
						{
							g_offset = 0;
							rndata.st[i]-=straincal[i];
							rndata.st[i]>>=9;
						}
					}

					usart_tx_buffer[0] = offset;
					for(int i=1,j=0;i<=24;)										// Put the 6 32 bit values into 24 bytes
					{
						usart_tx_buffer[i++]=(uint8_t)(rndata.st[j]>>24);
						usart_tx_buffer[i++]=(uint8_t)(rndata.st[j]>>16);
						usart_tx_buffer[i++]=(uint8_t)(rndata.st[j]>>8);
						usart_tx_buffer[i++]=(uint8_t)(rndata.st[j++]>>0);
					}
				}
			}
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

