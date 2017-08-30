#ifndef PLAN_SD_H
#define PLAN_SD_H

#include "plan_include.h"

#define RESPONSE_TIMEOUT        0xFFF

#define DUMMY_BYTE              (uint8_t) 0xFF

// CARD VERSIONS
typedef enum
{
  UNKNOWN               = 0x00,
  MMC_V3                = 0x01,
  SD_V1_SC              = 0x02,
  SD_V2_SC              = 0x04,
  SD_V2_HC              = 0x08,
  SD_V2_XC              = 0x10
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
  CMD16                 = 16|R1,        // SET_BLOCKLEN
  CMD17                 = 17|R1,        // READ_SINGLE_BLOCK
  CMD18                 = 18|R1,        // READ_MULTIPLE_BLOCK
  CMD55                 = 55|R1,        // APP_CMD
  CMD58                 = 58|R3,        // READ_OCR
  ACMDX                 = 1<<8,         // Mask specifying application command
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

#define SPI_SS_SD_SELECT() GPIO_ResetBits(GPIO_SPIx_SS_SD_PORT,GPIO_SPIx_SS_SD_PIN); GPIO_SetBits(GPIO_SPIx_EN_SD_PORT,GPIO_SPIx_EN_SD_PIN)
#define SPI_SS_SD_DESELECT() GPIO_SetBits(GPIO_SPIx_SS_SD_PORT,GPIO_SPIx_SS_SD_PIN); GPIO_ResetBits(GPIO_SPIx_EN_SD_PORT,GPIO_SPIx_EN_SD_PIN)

uint8_t         SD_Detect(void);
MEMTYPE         SD_Init(void);
RESPONSE        SD_SendCmd(command cmd, uint32_t arg);
void            SD_ReadData(uint8_t* buff, uint32_t count);
uint8_t         SPI_WriteByte(uint8_t Data);
uint8_t         SPI_ReadByte(void);

#endif
