#include "plan_sd.h"

RESPONSE        resp;

//==============================================================================
// FUNCTION SD_Detect()
//      - detects if SD card is in its hardware slot
//==============================================================================
uint8_t SD_Detect(void)
{
  if ((GPIO_EXTI_CD_PORT->IDR & GPIO_EXTI_CD_PIN)){return 1;}   // Detected
  return 0;                                                     // Not detected
}

//==============================================================================
// FUNCTION SD_Init()
//      - initializes SD card and determines what kind of memory card it is
//==============================================================================
MEMTYPE SD_Init(void)
{
  uint32_t      timeout = 0;                    // Timeout value
  MEMTYPE       memcard = SD_V2_HC;
  
  // LOW LEVEL INITIALIZATION STEPS  -------------------------------------------
  //    Slow down SPI, pull CS high, clock for 80 cycles
  //----------------------------------------------------------------------------
  change_spi_mode(SPI_MODE_SD_INIT);            // SPI to 375kHz
  SPI_SS_SD_DESELECT();                         // Pull CS high
  for (int i=0;i<10;i++){SPI_ReadByte();}       // Clock for 80 cycles 
  
  
  // CMD0 TRANSACTION  ---------------------------------------------------------
  //    Send CMD0 until the card is in idle state.
  //    If timed out - card is broken
  //----------------------------------------------------------------------------
  timeout = 10;
  // Send CMD0 until idle state or timeout
  do{resp = SD_SendCmd(CMD0,(uint32_t)0x00000000);timeout--;
  }while(!(resp.R1&SD_IN_IDLE_STATE) && timeout);
  if(timeout==0){return UNKNOWN;} // If timed out - card is broken
      
  
  // CMD8 TRANSACTION  ---------------------------------------------------------
  //    Send CMD8 until illegal command or valid response without illegal flag.
  //    If illegal - card is SD1.x/MMC/Unknown. If timed out - card is broken
  //----------------------------------------------------------------------------
  timeout = 10;
  // Send CMD8 until illegal, proper, or timeout
  do            
  {
    resp = SD_SendCmd(CMD8,(uint32_t)0x000001AA);
    // Catch illegal response - means card is v1.0 or MMC/unknown
    if(resp.R1&SD_ILLEGAL_COMMAND){memcard = SD_V1_SC; break;}
    timeout--;
  } while(!((resp.R1&SD_IN_IDLE_STATE)&&(resp.extra==0x000001AA))&&timeout);
  if(timeout==0){return UNKNOWN;} // If timed out (or mismatch) - card is broken
  
  
  // ACMD41 TRANSACTION  -------------------------------------------------------
  //    Send ACMD41 (CMD55 and CMD41) with HCS=1 (arg 0x 4000 0000)
  //    If returns idle, keep sending until time out.
  //----------------------------------------------------------------------------
  
  // If card is not high capacity (CMD8 illegal), dummy CMD55 to clear illegal
  if(!(memcard&(SD_V2_HC|SD_V2_XC)))
  {
    resp = SD_SendCmd(CMD55,(uint32_t)0x00000000);
  }
  
  timeout = 500;
  // Send ACMD41 until illegal, proper, or timeout
  do
  {
    resp = SD_SendCmd(ACMD41,(uint32_t)0x40000000); // Send CMD55 and CMD41
    // If caught response illegal ==> either MMC or error
    if(resp.R1&SD_ILLEGAL_COMMAND)
    {
      // If illegal and testing for v1.x/MMC, try CMD1 - maybe card is MMC
      if(memcard&SD_V1_SC){memcard=MMC_V3;}
      else{return UNKNOWN;}
    }
    timeout--;
  }while((resp.R1&SD_IN_IDLE_STATE) && timeout);
  if(timeout==0)
  { // If timed out and testing for v1.x/MMC, try CMD1 - maybe card is MMC
    if(memcard&SD_V1_SC){memcard=MMC_V3;}
    else{return UNKNOWN;}
  }
  
  // CMD1 TRANSACTION  ---------------------------------------------------------
  //    Send CMD1 to initialize MMC or see whether the card is non-standard
  //    - Only if CMD8 and ACMD41 got illegal responses ==> dealing with MMC
  //----------------------------------------------------------------------------
  if(memcard&MMC_V3)
  {
    timeout = 10;
    // Send CMD1 to initialize an MMC card
    do
    {
      resp = SD_SendCmd(CMD1,(uint32_t)0x00000000);
      timeout--;
    }while((resp.R1!=0x00) && timeout); // Until first valid response
    if(timeout==0){return UNKNOWN;} // If timed out - card is broken
  }
  
  // CMD58 TRANSACTION  --------------------------------------------------------
  //    Send CMD58 to check whether or not the card is SDSC or SDHC/SDXC
  //    - Only if CMD8 and ACMD41 got proper responses (state is still HC)
  //----------------------------------------------------------------------------
  if(memcard&(SD_V2_HC|SD_V2_XC))
  {
    resp.R1 = 0xFF;
    // Send CMD58 to tell if the card is high capacity or standard
    timeout = 10;
    do
    {
      resp = SD_SendCmd(CMD58,(uint32_t)0x00000000);
      timeout--;
    }while((resp.R1!=0x00) && timeout); // Until first valid response
    if(timeout==0){return UNKNOWN;} // If timed out - card is broken
    
    // If the OCR bit is not set ==> v2.0 SDSC
    if(!(resp.extra&0x40000000)){memcard = SD_V2_SC;}
  }
  
  // CMD16 TRANSACTION  --------------------------------------------------------
  //    Send CMD16 to force block size to 512 bytes to work with FAT system
  //    - Only if determined that card is SDSCv2, SDSCv1, or MMC)
  //----------------------------------------------------------------------------
  if(memcard&(SD_V2_SC|SD_V1_SC|MMC_V3))
  {
    timeout = 10;
    // Send CMD16 to set block size to 512 bytes
    do
    {
      resp = SD_SendCmd(CMD16,(uint32_t)0x00000200);
      timeout--;
    }while((resp.R1!=0x00) && timeout); // Until first valid response
    if(timeout==0){return UNKNOWN;} // If timed out - card is broken
  }
  
  return memcard;
}

//==============================================================================
// FUNCTION SD_SendCmd()
//      - sends an SD-style command: {command code, argument, crc}
//==============================================================================
RESPONSE    SD_SendCmd(command cmd, uint32_t arg)
{
  uint8_t timeout = 8;
  RESPONSE rval;
  uint8_t rvals[4];
  
  //Check if command is application command; if yes - send CMD55
  if ((cmd & (uint32_t)ACMDX) != 0) {// If application command - send CMD55
    resp = SD_SendCmd(CMD55,(uint32_t)0x00);
    if(resp.R1&SD_ILLEGAL_COMMAND)
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
    rval.R1 = SPI_ReadByte();
    timeout--;
  }
  while((rval.R1&0x80)&&timeout);
  
  // Get response type from cmd variable
  const uint32_t response = (cmd&(uint32_t)RMask);
  
  // If response is not R1, read more bytes in
  if (response != R1) {
      // Read the next 4 bytes
      for (uint8_t i=0; i<4; ++i) {rvals[i] = SPI_ReadByte();}
  }
  rval.extra = rvals[0]<<24|rvals[1]<<16|rvals[2]<<8|rvals[3];
  
  // Pull CS up to stop transmission
  SPI_SS_SD_DESELECT();
  
  // Clock one more byte
  SPI_ReadByte();
  
  return rval;
}

//==============================================================================
// FUNCTION SD_ReadData()
//      - reads data and fills up a provided buffer
//==============================================================================
void SD_ReadBlock(uint8_t* buff)
{
  // Select SD card
  SPI_SS_SD_SELECT();
  
  // Clock until SD card ready to send data
  while(SPI_ReadByte()!=0xFE);  //do{token = SPI_ReadByte();}while(token!=0xFE);
  
  // Read data bytes
  for (int i=0;i<512;i++)
  { // Fill buffer with desired data; skip the rest
    buff[i] = SPI_ReadByte();
  }
  
  // Read the two CRC bytes
  SPI_ReadByte(); SPI_ReadByte();
  
  // Deselect SD card
  SPI_SS_SD_DESELECT();
  
  // Clock additional byte
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
  SPI_I2S_SendData(SPIx, data);
  while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET){;}
  result = SPI_I2S_ReceiveData(SPIx);
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
