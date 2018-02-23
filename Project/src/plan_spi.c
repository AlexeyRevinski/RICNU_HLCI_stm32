#include "plan_spi.h"


//==============================================================================
// FUNCTION SPI_ChangeSpeed()
//      - writes and reads one data byte via SPI
//==============================================================================
uint8_t	SPI_ChangeSpeed(SPI_TypeDef * SPIx, uint16_t SPI_BaudRatePrescaler)
{
	if(!IS_SPI_ALL_PERIPH(SPIx)||
		IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)) return 0;
	SPI_Cmd(SPI_SD, DISABLE);
	SPIx->CR1 &= ~SPI_CR1_BR;
	SPIx->CR1 |= SPI_BaudRatePrescaler;
	SPI_Cmd(SPI_SD, ENABLE);
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
	while(numbytes--)
	{
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_BSY));// Wait until not busy
		SPI_WriteByte(SPIx,*(buffer++));					// Send byte
	}
}
