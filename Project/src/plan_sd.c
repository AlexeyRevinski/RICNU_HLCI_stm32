#include "plan_sd.h"

uint8_t SD_WriteByte(uint8_t Data)
{
  /* Wait until the transmit buffer is empty */
  while(SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
  }
  
  /* Send the byte */
  SPI_SendData8(SD_SPI, Data);
  
  /* Wait to receive a byte*/
  while(SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
  }
  
  /* Return the byte read from the SPI bus */ 
  return SPI_ReceiveData8(SD_SPI);
}