#include "plan_sd.h"

MEMTYPE			CardType	=	UNKNOWN;
uint8_t         memresp;			// Memory card response
uint32_t		memtime1	=	0;			// Memory card timer
uint32_t		memtime2	=	0;			// Memory card timer
extern DSTATUS 		diskstat;				// Memory card drive status

extern FATFS fs;

//==============================================================================
// FUNCTION SD_TimeUpdate()
//      - updates 1khz timers and checks SD card status
//==============================================================================
void 	SD_TimeUpdate()
{
	if (memtime1) memtime1--;			// Stop timers at 0
	if (memtime2) memtime2--;

	if (SD_DETECT) 	{diskstat &= ~STA_NODISK;}		// Is card present?
	else 				{diskstat |= (STA_NODISK | STA_NOINIT);}
}


//==============================================================================
// FUNCTION SD_WaitReady()
//      - polls response from card until timeout (specified in milliseconds)
//==============================================================================
uint8_t	SD_WaitReady(uint32_t ms)
{
	uint8_t resp;
	memtime2 = ms;							// Set SDTime2 to desired time
	do{resp = SPI_WriteByte(SPI_SD,0xFF);	// Send dummy byte
	} while (resp != 0xFF && memtime2);		// Check correct response
	return (resp == 0xFF) ? 1 : 0;			// If not timed-out - card is ready
}


//==============================================================================
// FUNCTION SD_Select()
//      - Selects memory card on SPI bus
//==============================================================================
uint8_t	SD_Select()
{
	SD_CS_LOW();							// Set CS low
	SPI_WriteByte(SPI_SD,0xFF);				// Send dummy byte
	if(SD_WaitReady(500)) return 1;			// If ready within 500ms, return success
	SD_Deselect();							// Otherwise, pull CS high
	return 0;								//   and return error
}


//==============================================================================
// FUNCTION SD_Deselect()
//      - Releases memory card on SPI bus
//==============================================================================
void	SD_Deselect()
{
	SD_CS_HIGH();
	SPI_WriteByte(SPI_SD,0xFF);
}


//==============================================================================
// FUNCTION SD_SendCmd()
//      - sends an SD-style command: {command code, argument, crc}
//==============================================================================
uint8_t    SD_SendCmd(command cmd, uint32_t arg)
{

	uint8_t resp;

	//Check if command is application command; if yes - send CMD55
	if ((cmd & (uint32_t)ACMDX) != 0) {// If application command - send CMD55
		resp = SD_SendCmd(CMD55,(uint32_t)0x00);
    	if(resp>SD_IN_IDLE_STATE) return resp;
	}

	// If not CMD12, release and select again. If not ready - error
	if (cmd != CMD12) {
		SD_Deselect();
		if (!SD_Select()) return 0xFF;
	}

	//------------------------------------
	// If CMD0 or CMD8, specific CRC; otherwise CRC is ignored by SD card, so send 0xFF
	uint8_t crc = 0xff;
	if            (cmd == CMD0) {crc = 0x95;}
	else if       (cmd == CMD8) {crc = 0x87;}

	// Make a command array: [0]cmd [1-4]arg [5]crc
	uint8_t cmdarray[6] = { ((uint8_t)(cmd))|0x40,(uint8_t)(arg>>24),(uint8_t)(arg>>16),
                          	  (uint8_t)(arg>>8),(uint8_t)(arg),(crc)};

	// Write the command array to SD card
	for (uint8_t i=0;i<6;i++){SPI_WriteByte(SPI_SD,cmdarray[i]);}
	//------------------------------------


	if (cmd == CMD12) SPI_WriteByte(SPI_SD,0xFF);	//Discard the following byte when CMD12


	// Keep reading bytes until 10 resp. failure bytes read or result is valid
	uint8_t timeout = 10;
	do{
		resp = SPI_WriteByte(SPI_SD,0xFF);
		timeout--;
	}
	while((resp & 0x80)&&timeout);

	return resp;
}


//==============================================================================
// FUNCTION SD_Init()
//      - initializes SD card and determines what kind of memory card it is
//==============================================================================
DSTATUS SD_Init(void)
{
  //uint32_t      timeout = 0;                    // Timeout value
  //MEMTYPE       memcard = SD_V2_HC;
  MEMTYPE		memcard = UNKNOWN;
  command		cmd;
  uint8_t 		n, ocr[4];

  if (diskstat & STA_NODISK) return diskstat;
  SD_CS_HIGH();												// Pull CS high
  SPI_ChangeSpeed(SPI_SD,SD_SLOW_PSCR);		// Change to 375kHz
  for (int i=0;i<10;i++){SPI_WriteByte(SPI_SD,0xFF);}       // Clock for 80 cycles


  if (SD_SendCmd(CMD0, 0) == 1) {			/* Put the card SPI/Idle state */
  		memtime1 = 1000;					/* Initialization timeout = 1 sec */
  		if (SD_SendCmd(CMD8, 0x1AA) == 1) {	/* SDv2? */
  			for (n = 0; n < 4; n++) ocr[n] = SPI_WriteByte(SPI_SD,0xFF);	/* Get 32 bit return value of R7 resp */
  			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {				/* Is the card supports vcc of 2.7-3.6V? */
  				while (memtime1 && SD_SendCmd(ACMD41, 1UL << 30)) ;	/* Wait for end of initialization with ACMD41(HCS) */
  				if (memtime1 && SD_SendCmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
  					for (n = 0; n < 4; n++) ocr[n] = SPI_WriteByte(SPI_SD,0xFF);
  					memcard = (ocr[0] & 0x40) ? SD_V2_BL : SD_V2;	/* Card id SDv2 */
  				}
  			}
  		} else {	/* Not SDv2 card */
  			if (SD_SendCmd(ACMD41, 0) <= 1) 	{	/* SDv1 or MMC? */
  				memcard = SD_V1; cmd = ACMD41;	/* SDv1 (ACMD41(0)) */
  			} else {
  				memcard = MMC_V3; cmd = CMD1;	/* MMCv3 (CMD1(0)) */
  			}
  			while (memtime1 && SD_SendCmd(cmd, 0)) ;		/* Wait for end of initialization */
  			if (!memtime1 || SD_SendCmd(CMD16, 512) != 0)	/* Set block length: 512 */
  				memcard = UNKNOWN;
  		}
  	}
  	CardType = memcard;	/* Card type */
  	SD_Deselect();

  	if (memcard) {			/* OK */
	SPI_ChangeSpeed(SPI_SD,SD_FAST_PSCR);			/* Set fast clock */
		diskstat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
	} else {			/* Failed */
		diskstat = STA_NOINIT;
	}

	return diskstat;
}



//==============================================================================
// FUNCTION SD_ReadBlock()
//      - reads data and fills up a provided buffer
//==============================================================================
uint8_t SD_ReadBlock(uint8_t* buff, uint32_t numbytes)
{

	uint8_t token;

	memtime1 = 200;
	do {							/* Wait for DataStart token in timeout of 200ms */
		token = SPI_WriteByte(SPI_SD,0xFF);
	} while ((token == 0xFF) && memtime1);
	if(token != 0xFE) return 0;		/* Function fails if invalid DataStart token or timeout */

	SPI_ReadMultByte(SPI_SD,buff, numbytes);		/* Store trailing data to the buffer */
	SPI_WriteByte(SPI_SD,0xFF); SPI_WriteByte(SPI_SD,0xFF);			/* Discard CRC */

	return 1;
}


//==============================================================================
// FUNCTION SD_SendBlock()
//      - sends a data block to the card (512 bytes)
//==============================================================================
uint8_t SD_SendBlock(const uint8_t* buff, uint8_t token)
{
	uint8_t resp;

	if (!SD_WaitReady(500)) return 0;		/* Wait for card ready */

	SPI_WriteByte(SPI_SD,token);					/* Send token */
	if (token != 0xFD) {				/* Send data if token is other than StopTran */
		SPI_SendMultByte(SPI_SD,buff,512);		/* Data */
		SPI_WriteByte(SPI_SD,0xFF);
		SPI_WriteByte(SPI_SD,0xFF);	/* Dummy CRC */

		resp = SPI_WriteByte(SPI_SD,0xFF);				/* Receive data resp */
		if ((resp & 0x1F) != 0x05) return 0;	/* Function fails if the data packet was not accepted */
	}
	return 1;
}


FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
)
{
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}



