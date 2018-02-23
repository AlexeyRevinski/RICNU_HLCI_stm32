#ifndef PLAN_SD_H
#define PLAN_SD_H

#include "plan_include.h"

#define RESPONSE_TIMEOUT        0xFFF

#define DUMMY_BYTE              (uint8_t) 0xFF
#define SD_SLOW_PSCR			SPI_BaudRatePrescaler_128
#define SD_FAST_PSCR			SPI_BaudRatePrescaler_8

// CARD VERSIONS
typedef enum
{
  UNKNOWN               = 0x00,
  MMC_V3                = 0x01,
  SD_V1              	= 0x02,
  SD_V2              	= 0x04,
  SD_V2_BL              = 0x08
} MEMTYPE;

// RESPONSE TYPES
typedef enum
{
  RMask                 = 0xE00,         // 0b 1110 0000 0000
  R1                    = 1<<9,          // R1 response - all commands
  R3                    = 1<<10,
  R7                    = 1<<11
} restype;

// COMMANDS
typedef enum
{
  CMD0                  = 0 |R1,        // GO_IDLE_STATE
  CMD1                  = 1 |R1,        // SEND_OP_COND
  CMD8                  = 8 |R7,        // SEND_IF_COND
  CMD9					= 9 |R1,			/* SEND_CSD */
  CMD12                 = 12|R1,        // STOP_TRANSMISSION
  CMD16                 = 16|R1,        // SET_BLOCKLEN
  CMD17                 = 17|R1,        // READ_SINGLE_BLOCK
  CMD18                 = 18|R1,        // READ_MULTIPLE_BLOCK
  CMD24					= 24|R1,
  CMD25					= 25|R1,
  CMD32					= 32|R1,
  CMD33					= 33|R1,
  CMD38					= 38|R1,
  CMD55                 = 55|R1,        // APP_CMD
  CMD58                 = 58|R3,        // READ_OCR
  ACMDX                 = 1<<8,         // Mask that specifes app command
  ACMD13				= 13|R1|ACMDX,
  ACMD23				= 23|R1|ACMDX,
  ACMD41                = 41|R1|ACMDX   // (App command) APP_SEND_OP_COND
} command;

typedef enum
{
  // SD response codes
  SD_RESPONSE_NO_ERROR          = (0x00),
  SD_IN_IDLE_STATE              = (0x01),
  SD_ERASE_RESET                = (0x02),
  SD_ILLEGAL_COMMAND            = (0x04),
  SD_COM_CRC_ERROR              = (0x08),
  SD_ERASE_SEQUENCE_ERROR       = (0x10),
  SD_ADDRESS_ERROR              = (0x20),
  SD_PARAMETER_ERROR            = (0x40),
  SD_RESPONSE_FAILURE           = (0xFF),
  // SD Data response codes
  SD_DATA_OK                    = (0x05),
  SD_DATA_CRC_ERROR             = (0x0B),
  SD_DATA_WRITE_ERROR           = (0x0D),
  SD_DATA_OTHER_ERROR           = (0xFF)
} SD_Error;

typedef struct
{
  uint8_t       R1;             // Used for all responses
  uint32_t      extra;          // Used only for R3 and R7
} RESPONSE;

// MACROS
#define SD_CS_LOW() 	GPIO_ResetBits(GPIO_SD_NSS_PORT,GPIO_SD_NSS_PIN);
#define SD_CS_HIGH() 	GPIO_SetBits(GPIO_SD_NSS_PORT,GPIO_SD_NSS_PIN);
#define SD_DETECT		(!(GPIO_SD_CD_PORT->IDR & GPIO_SD_CD_PIN))

void 			SD_TimeUpdate();
uint8_t			SD_WaitReady(uint32_t ms);
uint8_t			SD_Select();								// Macro?
void			SD_Deselect();								// Macro?
uint8_t 		SD_ReadBlock(uint8_t* buff, uint32_t numbytes);
uint8_t 		SD_SendBlock(const uint8_t* buff, uint8_t token);
uint8_t    		SD_SendCmd(command cmd, uint32_t arg);

DSTATUS         SD_Init(void);
//RESPONSE        SD_SendCmd(command cmd, uint32_t arg);
//void            SD_ReadBlock(uint8_t* buff);


FRESULT open_append (FIL* fp,const char* path);

#endif
