#include "plan_sd.h"

void SD_Init(void)
{
  uint32_t      timeout = 0;      // Timeout value
  uint32_t      rval[5] = {0,0,0,0,0};      // Used for R3 ,R7
  // Slow down SPI to 400kHz or lower (375kHz in this case)
  change_spi_mode(SPI_MODE_SD_INIT);
  // Pull CS high
  SPI_SS_SD_DESELECT();
  // Send 80 clock cycles while CS is pulled high
  for (int i=0;i<10;i++){SPI_ReadByte();}
  
  // CMD0 TRANSACTION  ---------------------------------------------------------
  //    Send CMD0 until the card is in idle state.
  //    If timed out - card is broken
  //----------------------------------------------------------------------------
  timeout = 10;
  do            // Send CMD0 until proper or timed out
  {
    SD_SendCmd(CMD0,(uint32_t)0x00000000,rval);
    timeout--;
  }
  while(!(rval[0]&SD_IN_IDLE_STATE) && timeout);
  if(timeout==0)  // If timed out - card is unresponsive
  {
    // TODO ERROR STATE;
    GPIO_SetBits(LEDx_PORT,LED2_PIN);
  }
    
  // CMD8 TRANSACTION  ---------------------------------------------------------
  //    Send CMD8 until illegal command or valid response 
  //    without illegal command. If illegal - SD1.x or not SD Card.
  //    If timed out - card is broken
  //----------------------------------------------------------------------------
  timeout = 10;
  do            // Send CMD8 until illegal, proper, or timeout
  {
    SD_SendCmd(CMD8,(uint32_t)0x000001AA,rval);
    if(rval[0]&SD_ILLEGAL_COMMAND)     // SD1.x or not SD
    { // UNTESTED - NEED SD 1.x CARD TO TEST
      SD_SendCmd(CMD55,(uint32_t)0x00000000,rval);  // Send CMD55 to reset illegal
      do
      {
        SD_SendCmd(ACMD41,(uint32_t)0x00000000,rval); // Send CMD55 and CMD41
        if(rval[0]&SD_ILLEGAL_COMMAND)     // ==> SD Card is most likely MMC
        {
          // TODO ERROR STATE;
          GPIO_SetBits(LEDx_PORT,LED2_PIN);
        }
        timeout--;
      }while((rval[0]&SD_IN_IDLE_STATE) && timeout);
      if(timeout==0)  // If timed out - card is unresponsive
      {
        // TODO ERROR STATE;
        GPIO_SetBits(LEDx_PORT,LED2_PIN);
      }
      // ==> SD Card is SD v1.xx
      break;
    }
    timeout--;
  }while(!((rval[0]&SD_IN_IDLE_STATE)&&(rval[3]==0x01)&&(rval[4]==0xAA)) && timeout);
  if(timeout==0)  // If timed out - card is unresponsive
  {
    // TODO ERROR STATE;
    GPIO_SetBits(LEDx_PORT,LED2_PIN);
  }
  // ===> SD Card is SD v2.00 or later
  
  
  // ACMD41 TRANSACTION  -------------------------------------------------------
  //    Send ACMD41 (CMD55 and CMD41) with HCS=1 (arg 0x 4000 0000)
  //    If returns 0x01, keep sending until time out
  //----------------------------------------------------------------------------
  timeout = 500;
  do
  {
    SD_SendCmd(ACMD41,(uint32_t)0x40000000,rval); // Send CMD55 and CMD41
    if(rval[0]&SD_ILLEGAL_COMMAND)     // ==> SD Card is broken
    {
      // TODO ERROR STATE;
      GPIO_SetBits(LEDx_PORT,LED2_PIN);
    }
    timeout--;
  }while((rval[0]&SD_IN_IDLE_STATE) && timeout);
  if(timeout==0)  // If timed out - card is unresponsive
  {
    // TODO ERROR STATE;
    GPIO_SetBits(LEDx_PORT,LED2_PIN);
  }
  // ==> Card is ready
  
  // CMD58 TRANSACTION  --------------------------------------------------------
  //    Send CMD58 to check whether or not the card is SDSC or SDHC/SDXC
  //----------------------------------------------------------------------------
  timeout = 10;
  rval[0] = 0xFF;
  do
  {
    SD_SendCmd(CMD58,(uint32_t)0x00000000,rval); // Send CMD55 and CMD41
    timeout--;
  }while((rval[0]!=0x00) && timeout);
  if(timeout==0)  // If timed out - card is unresponsive
  {
    // TODO ERROR STATE;
    GPIO_SetBits(LEDx_PORT,LED2_PIN);
  }
  
  if(rval[1]&0x40) // If the OCR bit is set ==> v2.0 SDHC/SDXC
  {
    ;GPIO_SetBits(LEDx_PORT,LED2_PIN);
  }
  else                  // Otherwise, SDSC
  {
    ;GPIO_SetBits(LEDx_PORT,LED2_PIN);
  }
  // ==> Card is initialized!
}

//==============================================================================
// FUNCTION SD_SendCmd()
//      - sends an SD-style command: {command code, argument, crc}
//==============================================================================
void    SD_SendCmd(command cmd, uint32_t arg, uint32_t* rval)
{
  uint8_t timeout = 8;
  
  //Check if command is application command; if yes - send CMD55
  if ((cmd & (uint32_t)ACMDX) != 0) {// If application command - send CMD55
    SD_SendCmd(CMD55,(uint32_t)0x00,rval);
    if(rval[0]&SD_ILLEGAL_COMMAND)
    {
      //Got an old card type;
    }
  }
  
  // If CMD0 or CMD8, specific CRC; otherwise ignored by SD card, so send 0xFF
  uint8_t crc = 0xff;
  if            (cmd == CMD0) {crc = 0x95;}
  else if       (cmd == CMD8) {crc = 0x87;}
  
  // Make a command array: [0]cmd [1-4]arg [5]crc
  uint8_t cmdarray[6] = { ((uint8_t)(cmd))|0x40,(uint8_t)(arg>>24),(uint8_t)(arg>>16),
                          (uint8_t)(arg>>8),(uint8_t)(arg),(crc)};
  
  // Pull CS down to start transmission
  SPI_SS_SD_SELECT();                         // Set CS high
  
  // Write the command array to SD card
  for (uint8_t i=0;i<6;i++){SPI_WriteByte(cmdarray[i]);}
  
  // Keep reading bytes until 8 resp. failure bytes read or result is valid
  do{
    rval[0] = SPI_ReadByte();
    timeout--;
  }
  while((rval[0]&0x80)&&timeout);
  
  // Get response type from cmd variable
  const uint32_t response = (cmd&(uint32_t)RMask);
  
  // If response is not R1, read more bytes in
  if (response != R1) {
      // Read the next 4 bytes
      for (uint8_t i=0; i<4; ++i) {rval[i+1] = SPI_ReadByte();}
  }
  
  // Pull CS up to stop transmission
  SPI_SS_SD_DESELECT();
  // Clock one more byte
  SPI_ReadByte();
}

//==============================================================================
// FUNCTION SPI_WriteByte()
//      - writes and reads one data byte via SPI
//==============================================================================
uint8_t SPI_WriteByte(uint8_t data)
{//Wait until tx not empty, send data, wait until rx not empty, read data
  uint8_t result = 0;
  while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){;}
  SPI_SendData8(SPIx, data);
  while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){;}
  result = SPI_ReceiveData8(SPIx);
  return result;
}

//==============================================================================
// FUNCTION SPI_ReadByte()
//      - clocks SPI bus to receive MISO data
//==============================================================================
uint8_t SPI_ReadByte(void)
{
  uint8_t result = 0;
  result = SPI_WriteByte(DUMMY_BYTE);
  return result;
}