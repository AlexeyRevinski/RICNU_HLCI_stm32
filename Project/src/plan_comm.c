#include "plan_comm.h"

uint8_t         spi_rx_buffer[DATA_SIZE_FLEXSEA];       // Used for SPI MN rx   // ALL OF THIS SHOULD GO TO COMM
extern uint8_t  comm_str_1[COMM_STR_BUF_LEN];           // Used for SPI MN tx
uint8_t         usart_tx_buffer[DATA_SIZE_P2U];         // Used for UART tx
uint8_t         usart_rx_buffer[DATA_SIZE_U2P];         // Used for UART rx
uint8_t         spi_payload[DATA_SIZE_FLEXSEA];
uint8_t         spi_rx_tmp[DATA_SIZE_FLEXSEA];

extern uint8_t		g_offset;
extern 			state sys_state;
extern			lstate log_state;
ricnu_data 		rndata;
flexsea_ctrl 	fc;

extern int32_t straincal[];
extern int32_t imucal[];


int64_t loadCellM[6][6] = {{ 9008,	101,	-9026,	-60,	121,	41},		// Fx, x10
						  { 5328,	89,		5472,	72,		-10849,	32},		// Fy, x10
						  {-52,		4990,	-67,	4979,	126,	4976},		// Fz, x10
						  { 261,	9833,	421,	-9659,	-680,	263},		// Mx, x1000
						  { 246,	5923,	-504,	5766,	-294,	-11289},	// My, x1000
						  {-19219,	-176,   -17030,	-134,   -18937,	-101}};		// Mz, x1000



//==============================================================================
// FUNCTION update()
//      - starts DMA-assisted communication to a specified device
//==============================================================================
void comm_start(device dev)
{
  switch (dev)
  {
  case MANAGE:
    while(SPI_I2S_GetFlagStatus(						// While DR not empty, read
    		SPI_MN, SPI_I2S_FLAG_RXNE) == SET)
    {SPI_I2S_ReceiveData(SPI_MN);}
    GPIO_ResetBits(GPIO_MN_NSS_PORT,GPIO_MN_NSS_PIN);	// Select Manage
    DMA_Cmd(SPI_MN_DMA_RX_CHAN, ENABLE);				// Start memory transfer
    DMA_Cmd(SPI_MN_DMA_TX_CHAN, ENABLE);				// Start memory transfer
    break;
  case USER:
    DMA_Cmd(USART_BT_DMA_TX_CHAN, ENABLE);				// Start memory transfer
    break;
  }
}


void comm_send_user_cmd(uint8_t cmd)
{
	switch(cmd){
	case 0xA5:
		comm_prep_user(3);
	}
	comm_start(USER);
}

void comm_send_manage(void)
{
	comm_prep_manage();
	comm_start(MANAGE);
}


void comm_prep_manage(void)
{
  tx_cmd_ricnu_rw(TX_N_DEFAULT, fc.offset, fc.control, fc.setpoint,
                  CHANGE, fc.g0, fc.g1, fc.g2, fc.g3);
  pack(P_AND_S_DEFAULT, FLEXSEA_EXECUTE_1, 0, 0, comm_str_1);
}


void comm_prep_user(uint8_t offset)
{
	usart_tx_buffer[0] = offset;
	switch(offset){
	case 0:		// Send IMU data
		usart_tx_buffer[1 ] = (uint8_t)((rndata.gx>>8 ) & 0xFF);
		usart_tx_buffer[2 ] = (uint8_t)((rndata.gx    ) & 0xFF);
		usart_tx_buffer[3 ] = (uint8_t)((rndata.gy>>8 ) & 0xFF);
		usart_tx_buffer[4 ] = (uint8_t)((rndata.gy    ) & 0xFF);
		usart_tx_buffer[5 ] = (uint8_t)((rndata.gz>>8 ) & 0xFF);
		usart_tx_buffer[6 ] = (uint8_t)((rndata.gz    ) & 0xFF);
		usart_tx_buffer[7 ] = (uint8_t)((rndata.ax>>8 ) & 0xFF);
		usart_tx_buffer[8 ] = (uint8_t)((rndata.ax    ) & 0xFF);
		usart_tx_buffer[9 ] = (uint8_t)((rndata.ay>>8 ) & 0xFF);
		usart_tx_buffer[10] = (uint8_t)((rndata.ay    ) & 0xFF);
		usart_tx_buffer[11] = (uint8_t)((rndata.az>>8 ) & 0xFF);
		usart_tx_buffer[12] = (uint8_t)((rndata.az    ) & 0xFF);
		usart_tx_buffer[13] = (uint8_t)((rndata.em>>24) & 0xFF);
		usart_tx_buffer[14] = (uint8_t)((rndata.em>>16) & 0xFF);
		usart_tx_buffer[15] = (uint8_t)((rndata.em>>8 ) & 0xFF);
		usart_tx_buffer[16] = (uint8_t)((rndata.em    ) & 0xFF);
		usart_tx_buffer[17] = (uint8_t)((rndata.ej>>24) & 0xFF);
		usart_tx_buffer[18] = (uint8_t)((rndata.ej>>16) & 0xFF);
		usart_tx_buffer[19] = (uint8_t)((rndata.ej>>8 ) & 0xFF);
		usart_tx_buffer[20] = (uint8_t)((rndata.ej    ) & 0xFF);
		usart_tx_buffer[21] = (uint8_t)((rndata.cu>>8 ) & 0xFF);
		usart_tx_buffer[22] = (uint8_t)((rndata.cu    ) & 0xFF);
		usart_tx_buffer[23] = (uint8_t)((0            ) & 0xFF);
		usart_tx_buffer[24] = (uint8_t)((0            ) & 0xFF);
		break;
	case 1:		// Send strain data
		for(int i=1,j=0;i<=24;)										// Put the 6 32 bit values into 24 bytes
		{
			usart_tx_buffer[i++]=(uint8_t)(rndata.st[j]>>24);
			usart_tx_buffer[i++]=(uint8_t)(rndata.st[j]>>16);
			usart_tx_buffer[i++]=(uint8_t)(rndata.st[j]>>8);
			usart_tx_buffer[i++]=(uint8_t)(rndata.st[j++]>>0);
		}
		break;
	case 2:		// Send calibration stop
		for(int i=1;i<=24;)
		{
			usart_tx_buffer[i++]=0;
		}
		break;
	}
}



uint8_t comm_unpack_manage(void)
{
	int16_t rawstrain[6] = {0,0,0,0,0,0};
	if(unpack_payload(spi_rx_buffer,spi_rx_tmp,spi_payload)>0)
	{
		uint16_t index = 0;
		uint8_t offset = 0;
		index = P_DATA1;
		offset = spi_payload[index++];

		if(offset == 0)
		{
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
				fc.offset = 1;
				rndata.gx-=imucal[0];
				rndata.gy-=imucal[1];
				rndata.gz-=imucal[2];
				rndata.ax-=imucal[3];
				rndata.ay-=imucal[4];
				rndata.az-=imucal[5];
				rndata.em-=imucal[6];
				rndata.ej-=imucal[7];
				rndata.cu-=imucal[8];
			}
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
					fc.offset = 0;
					rndata.st[i]-=straincal[i];
					rndata.st[i]>>=9;
				}
			}
		}
		return offset;
	}
	return 5;
}

void comm_unpack_user(void)
{
  if(((usart_rx_buffer[0]>>4)&0x03)==0x03)	// Calibrate
  {
	  change_sys_state(&sys_state,EVENT_CALIB);
  }
  else if(((usart_rx_buffer[0]>>4)&0x03)==0x02)// Relax
  {
	  change_sys_state(&sys_state,EVENT_STOP);
  }
  else
  {
	  if(((usart_rx_buffer[0]>>(7-0))&1))
	    {
	  	  change_sys_state(&sys_state,EVENT_START);
	    }
	    else if (!((usart_rx_buffer[0]>>(7-0))&1))
	    {
	  	  change_sys_state(&sys_state,EVENT_STOP);
	    }
	    if(((usart_rx_buffer[0]>>(7-1))&1))		// Log on and off
	    {
	    	log_state = LOG_ON;
	    }
	    else if (!((usart_rx_buffer[0]>>(7-1))&1))
	    {
	    	log_state = LOG_OFF;
	    }
  }
}


