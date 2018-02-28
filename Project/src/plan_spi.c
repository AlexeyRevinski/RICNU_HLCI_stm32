#include "plan_spi.h"

char temptxbuff[1000];
char temprxbuff[1000];

//==============================================================================
// FUNCTION SPI_ChangeSpeed()
//      - writes and reads one data byte via SPI
//==============================================================================
uint8_t	SPI_ChangeSpeed(SPI_TypeDef * SPIx, uint16_t SPI_BaudRatePrescaler)
{
	if(!IS_SPI_ALL_PERIPH(SPIx)||
		!IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)) return 0;
	SPI_Cmd(SPIx, DISABLE);
	SPIx->CR1 &= ~SPI_CR1_BR;
	SPIx->CR1 |= SPI_BaudRatePrescaler;
	SPI_Cmd(SPIx, ENABLE);
	return 1;
}


//==============================================================================
// FUNCTION SPI_WriteByte()
//      - writes and reads one data byte via SPI
//==============================================================================
uint8_t	SPI_WriteByte(SPI_TypeDef * SPIx, uint8_t data)
{
  uint8_t result = 0;
  while(!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE));  	// Wait until TX empty
  SPI_I2S_SendData(SPIx, data);								// Send data
  while(!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)); 	// Wait until RX not empty
  result = SPI_I2S_ReceiveData(SPIx);						// Receive data
  return result;
}


//==============================================================================
// FUNCTION SPI_ReadMultByte()
//      - Receives multiple bytes over SPI (sends 0xFF)
//==============================================================================
void	SPI_ReadMultByte(SPI_TypeDef * SPIx,uint8_t* buffer,uint32_t numbytes)
{
	/*
	SPI_SD_DMA_RX_CHAN->CNDTR = numbytes;				// Buffer size
	SPI_SD_DMA_RX_CHAN->CMAR = (uint32_t)buffer;		// Memory base addr

	// Clear data register by reading data until none left
	while(SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_RXNE) == SET)
	{
		SPI_I2S_ReceiveData(SPI_SD);
	}

	// Start DMA transfer from memory to SPI->DR
	DMA_Cmd(SPI_SD_DMA_TX_CHAN, ENABLE);
	DMA_Cmd(SPI_SD_DMA_RX_CHAN, ENABLE);

	while(!DMA_GetFlagStatus(DMA1_FLAG_TC3));	// Wait until TX transfer is complete
	//DMA_Cmd(SPI_SD_DMA_TX_CHAN, DISABLE);
	while(SPI_I2S_GetFlagStatus(SPI_SD,SPI_I2S_FLAG_BSY));	// Wait while SPI is busy
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));	// Wait until RX transfer is complete
	//DMA_Cmd(SPI_SD_DMA_RX_CHAN, DISABLE);
*/

	while(numbytes--)
	{
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_BSY));// Wait until not busy
		*(buffer++) = SPI_WriteByte(SPIx,0xFF);				// Get byte
	}

}


//==============================================================================
// FUNCTION SPI_ReadMultByte()
//      - Sends multiple bytes over SPI (discards received data)
//==============================================================================
void	SPI_SendMultByte(SPI_TypeDef * SPIx,const uint8_t* buffer, uint32_t numbytes)
{
	/*
	SPI_SD_DMA_TX_CHAN->CNDTR = numbytes;				// Buffer size
	SPI_SD_DMA_TX_CHAN->CMAR = (uint32_t)buffer;		// Memory base addr
	//SPI_SD_DMA_RX_CHAN->CNDTR = numbytes;				// Buffer size
	//SPI_SD_DMA_RX_CHAN->CMAR = (uint32_t)temprxbuff;		// Memory base addr

	// Start DMA transfer from memory to SPI->DR
	SPI_I2S_DMACmd(SPI_SD, SPI_I2S_DMAReq_Tx, ENABLE);
	//SPI_I2S_DMACmd(SPI_SD, SPI_I2S_DMAReq_Rx, ENABLE);
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC3));	// Wait until TX transfer is complete
	SPI_I2S_DMACmd(SPI_SD, SPI_I2S_DMAReq_Tx, DISABLE);
	//while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));
	//SPI_I2S_DMACmd(SPI_SD, SPI_I2S_DMAReq_Rx, DISABLE);
	while(SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_RXNE) == SET)
	{
		SPI_I2S_ReceiveData(SPI_SD);
	}
	*/




	while(numbytes--)
	{
		//while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_BSY));// Wait until not busy
				//SPI_WriteByte(SPIx,*(buffer++));					// Send byte
		//while(SPI_SD->SR & SPI_I2S_FLAG_BSY);
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_BSY));
		SPI_WriteByte(SPIx,*(buffer++));					// Send byte
		//while(!(SPI_SD->SR & SPI_I2S_FLAG_TXE));  	// Wait until TX empty
		//SPIx->DR = *(buffer++);								// Send data
		//while(!(SPI_SD->SR & SPI_I2S_FLAG_RXNE)); 	// Wait until RX not empty
		//SPIx->DR;
	}


}
