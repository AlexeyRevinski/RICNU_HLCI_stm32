#include "plan_include.h"

DSTATUS diskstat = STA_NOINIT; // Status of disk drive
DRESULT diskrslt; // Result of disk functions
MEMTYPE memtype;

DSTATUS disk_status (BYTE drv)
{
  if (drv) return STA_NOINIT;                   // SD card only has a drive 0
  return diskstat;
}

DSTATUS disk_initialize (BYTE drv)
{  
  if (drv) return STA_NOINIT;                   // SD card only has a drive 0
  
  if(SD_Detect()){diskstat &= ~STA_NODISK;}     // SD card detected
  else           {return STA_NODISK;}           // SD card not detected
  
  if(SD_Init()) {diskstat &= ~STA_NOINIT;}      // Card initialized okay
  else          {return STA_NOINIT;}            // Card init failed
  
  return diskstat;
}

DRESULT disk_read (BYTE drv, BYTE* buff, DWORD sector, UINT count)
{ 
  if (drv || !count) return RES_PARERR;		// Check parameters
  if (diskstat & STA_NOINIT) return RES_NOTRDY;	// Check drive status

  command CMDxx = CMD17;
  int timeout = 10;
  RESPONSE rval;
  
  change_spi_mode(SPI_MODE_SD_DATA);

  // Send read data request (multiplying sector by 512 to get byte address)
  if (count==1)         {CMDxx = CMD17;} //Single block read
  else                  {CMDxx = CMD18;} //Multiple block read
  
  do
  { // ONLY FOR HIGH CAPACITY CARDS (for standard, sector<<9)
    rval = SD_SendCmd(CMDxx,(uint32_t)sector);        // Send CMDxx; sector*512
    timeout--;
  }while((rval.R1!=0x00) && timeout); // Until first valid response
  if(timeout==0){return RES_NOTRDY;} // If timed out - card is broken
  
  // Read data based on number of sectors
  SD_ReadData(buff, count);
  
  return RES_OK;                                                                // TODO CHECK IF ACTUALLY OKAY
}

DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
  return diskrslt;
}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
  return diskrslt;
}