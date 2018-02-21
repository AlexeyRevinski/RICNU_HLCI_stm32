#include "plan_include.h"

DSTATUS diskstat = STA_NOINIT; // Status of disk drive
DRESULT diskrslt; // Result of disk functions
//MEMTYPE memtype;

extern MEMTYPE			CardType;

DSTATUS disk_status (BYTE drv)
{
  if (drv) return STA_NOINIT;                   // SD card only has a drive 0
  return diskstat;
}

DSTATUS disk_initialize (BYTE drv)
{  
  if (drv) return STA_NOINIT;                   // SD card only has a drive 0
  
  if(SD_DETECT){diskstat &= ~STA_NODISK;}     // SD card detected
  else           {return STA_NODISK;}           // SD card not detected
  
  //if(SD_Init()) {diskstat &= ~STA_NOINIT;}      // Card initialized okay
  //else          {return STA_NOINIT;}            // Card init failed
  
  return SD_Init();
}

DRESULT disk_read (BYTE drv, BYTE* buff, DWORD sector, UINT count)
{ 
	if (drv || !count) return RES_PARERR;		/* Check parameter */
	if (diskstat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	if (!(CardType==SD_V2_BL)) sector *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (count == 1) {	/* Single sector read */
		if ((SD_SendCmd(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
			&& SD_ReadBlock(buff, 512)) {
			count = 0;
		}
	}
	else {				/* Multiple sector read */
		if (SD_SendCmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (!SD_ReadBlock(buff, 512)) break;
				buff += 512;
			} while (--count);
			SD_SendCmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	SD_Deselect();

	return count ? RES_ERROR : RES_OK;	/* Return result */
}


DRESULT disk_write (BYTE drv, const BYTE* buff, DWORD sector, UINT count)
{
	if (drv || !count) return RES_PARERR;		/* Check parameter */
	if (diskstat & STA_NOINIT) return RES_NOTRDY;	/* Check drive status */
	if (diskstat & STA_PROTECT) return RES_WRPRT;	/* Check write protect */

	if (!(CardType==SD_V2_BL)) sector *= 512;	/* LBA ==> BA conversion (byte addressing cards) */

	if (count == 1) {	/* Single sector write */
		if ((SD_SendCmd(CMD24, sector) == 0)	/* WRITE_BLOCK */
			&& SD_SendBlock(buff, 0xFE)) {
			count = 0;
		}
	}
	else {				/* Multiple sector write */
		if (CardType==SD_V1||CardType==SD_V2) SD_SendCmd(ACMD23, count);	/* Predefine number of sectors */
		if (SD_SendCmd(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
			do {
				if (!SD_SendBlock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!SD_SendBlock(0, 0xFD)) count = 1;	/* STOP_TRAN token */
		}
	}
	SD_Deselect();

	return count ? RES_ERROR : RES_OK;	/* Return result */
}

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive number (0) */
	BYTE cmd,		/* Control command code */
	void *buff		/* Pointer to the conrtol data */
)
{
	DRESULT res;
	BYTE n, csd[16];
	DWORD *dp, st, ed, csize;


	if (drv) return RES_PARERR;					/* Check parameter */
	if (diskstat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	res = RES_ERROR;

	switch (cmd) {
	case CTRL_SYNC :		/* Wait for end of internal write process of the drive */
		if (SD_Select()) res = RES_OK;
		break;

	case GET_SECTOR_COUNT :	/* Get drive capacity in unit of sector (DWORD) */
		if ((SD_SendCmd(CMD9, 0) == 0) && SD_ReadBlock(csd, 16)) {
			if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
				csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
				*(DWORD*)buff = csize << 10;
			} else {					/* SDC ver 1.XX or MMC ver 3 */
				n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
				csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
				*(DWORD*)buff = csize << (n - 9);
			}
			res = RES_OK;
		}
		break;

	case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
		if (CardType==SD_V2) {	/* SDC ver 2.00 */
			if (SD_SendCmd(ACMD13, 0) == 0) {	/* Read SD status */
				SPI_WriteByte(SPI_SD, 0xFF);
				if (SD_ReadBlock(csd, 16)) {				/* Read partial block */
					for (n = 64 - 16; n; n--) SPI_WriteByte(SPI_SD,0xFF);	/* Purge trailing data */
					*(DWORD*)buff = 16UL << (csd[10] >> 4);
					res = RES_OK;
				}
			}
		} else {					/* SDC ver 1.XX or MMC */
			if ((SD_SendCmd(CMD9, 0) == 0) && SD_ReadBlock(csd, 16)) {	/* Read CSD */
				if (CardType==SD_V1) {	/* SDC ver 1.XX */
					*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
				} else {					/* MMC */
					*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
				}
				res = RES_OK;
			}
		}
		break;

	case CTRL_TRIM :	/* Erase a block of sectors (used when _USE_ERASE == 1) */
		if (!(CardType==SD_V1||CardType==SD_V2)) break;				/* Check if the card is SDC */
		if (disk_ioctl(drv, MMC_GET_CSD, csd)) break;	/* Get CSD */
		if (!(csd[0] >> 6) && !(csd[10] & 0x40)) break;	/* Check if sector erase can be applied to the card */
		dp = buff; st = dp[0]; ed = dp[1];				/* Load sector block */
		if (!(CardType==SD_V2_BL)) {
			st *= 512; ed *= 512;
		}
		if (SD_SendCmd(CMD32, st) == 0 && SD_SendCmd(CMD33, ed) == 0 && SD_SendCmd(CMD38, 0) == 0 && SD_WaitReady(30000)) {	/* Erase sector block */
			res = RES_OK;	/* FatFs does not check result of this command */
		}
		break;

	default:
		res = RES_PARERR;
	}

	SD_Deselect();

	return res;
}




