/*
* EBS - RTFS (Real Time File Manager)
*
* Copyright EBS Inc 1987-2003
* All rights reserved.
* This code may not be redistributed in source or linkable object form
* without the consent of its author.
*/
/* drsdcard.c - SD card disk driver for rtfs version 6.

Summary

 Description
    File system independent entry points for MMC/SD card driver.
*/


void rtsdcardReportRemovalEvent(int unit);
void rtsdcardReportInsertionEvent(int unit);
typedef void (* INSERTCBFN)(int unit);
typedef void (* REMOVECBFN)(int unit);

void rtsdcard_driver_attach(INSERTCBFN pInsertFn, REMOVECBFN pReMoveFn);
int rtsdcard_device_media_parms(int unit, unsigned long *nSectors, unsigned long *BytesPerSector, int *isReadOnly);
int rtsdcard_device_read(int unit, unsigned long Sector, unsigned long nSectors, unsigned char *pData);
int rtsdcard_device_write(int unit, unsigned long Sector, unsigned long nSectors, unsigned char *pData);
int RtSdcard_init(int unitnumber);

unsigned long RtSdcard_Write_Block(int unit, unsigned long blockNum, unsigned long blockCount, unsigned char *pbuffer);
unsigned long RtSdcard_Read_Block(int unit, unsigned long blockNum, unsigned long blockCount, unsigned char *pbuffer);
int RtSdcard_device_media_parms(int unit, unsigned long *nSectors, unsigned long *BytesPerSector, int *isReadOnly);
int RtSdcard_attach(void);

#define SECTORSIZE(U) 512

static INSERTCBFN pFsInsertFn;
static REMOVECBFN pFsReMoveFn;

/* Called by the BLK_DEV_SDCARD_Mount wich is called by startup code to initialize the SD card driver
   Store insert and reove callback function pointers
   These are called from rtsdcardReportInsertionEvent() and rtsdcardReportRemovalEvent()
   which are called by the sdcard BSP layer's card change interrupt.
*/
void rtsdcard_driver_attach(INSERTCBFN pInsertFn, REMOVECBFN pReMoveFn)
{
    pFsInsertFn = pInsertFn;
    pFsReMoveFn = pReMoveFn;
    RtSdcard_attach();              /* Initialize the device driver and call pFsInsertFn*/
}


int rtsdcard_device_media_parms(int unit, unsigned long *nSectors, unsigned long *BytesPerSector, int *isReadOnly)
{
 return RtSdcard_device_media_parms(unit, nSectors, BytesPerSector, isReadOnly);
}

int rtsdcard_device_open(int unit)
{
    if (RtSdcard_init(unit)==0)
    {
//        rtp_printf("Force card insert \n");
//        pFsInsertFn(unit);
        return 0;
    }
    else
        return -1;
}

int rtsdcard_device_read(int unit, unsigned long Sector, unsigned long nSectors, unsigned char *pData)
{
    while (nSectors)
    {
    unsigned long nread;
       nread=RtSdcard_Read_Block(unit, Sector,nSectors, pData);
       if (nread < 1)
          return -1;
       Sector += nread;
       pData += nread*SECTORSIZE(unit);
       nSectors -= nread;
    }
    return 0;
}


int rtsdcard_device_write(int unit, unsigned long Sector, unsigned long nSectors, unsigned char *pData)
{
    while (nSectors)
    {
        unsigned long nWritten;
        nWritten=RtSdcard_Write_Block(unit, Sector, nSectors, pData);
        if (nWritten < 1)
            return -1;
        Sector += nWritten;
        pData += nWritten*SECTORSIZE(unit);
        nSectors -= nWritten;
    }
    return 0;
}
/*  These two routines are called from the sdcard BSP layer's card change interrupt */
void rtsdcardReportRemovalEvent(int unit)
{
    if (pFsReMoveFn)
        pFsReMoveFn(unit);
}
void rtsdcardReportInsertionEvent(int unit)
{
    if (pFsInsertFn)
        pFsInsertFn(unit);
}

#include "diskio.h"
#ifdef __linux__
#define USE_AMAT_SD_DRIVE 0
#warning "hack"
#else
#define USE_AMAT_SD_DRIVE 1
#warning "hack"
#endif

int RtSdcard_init(int unitnumber) {
#if (USE_AMAT_SD_DRIVE)
 DRESULT r= SDCard_Initialize(0);

#else
 DRESULT r= SDMM_initialize(0);
#endif
 if (r==RES_OK)
   return 0;
 else
   return -1;
}

unsigned long RtSdcard_Write_Block(int unit, unsigned long blockNum, unsigned long blockCount, unsigned char *pbuffer)
{
#if (USE_AMAT_SD_DRIVE)
  for (blocksRead=0; blocksRead<blockCount;blocksRead++)
  {
    if (Write_SDData(pbuffer, blockNum) != RES_OK)
       return 0;
    pbuffer += 512;
    blockNum += 1;
  }
  return blockCount;
#else
DRESULT r = SDMM_write (0,  pbuffer,  blockNum, blockCount);
 if (r!=RES_OK)
   return 0;
 else
   return blockCount;
#endif
}
unsigned long RtSdcard_Read_Block(int unit, unsigned long blockNum, unsigned long blockCount, unsigned char *pbuffer)
{
unsigned long blocksRead;

#if (USE_AMAT_SD_DRIVE)
  for (blocksRead=0; blocksRead<blockCount;blocksRead++)
  {
    if (Read_SDData(pbuffer, blockNum) != RES_OK)
       return 0;
    pbuffer += 512;
    blockNum += 1;
  }
  return blockCount;
#else
DRESULT r = SDMM_read (0,  pbuffer,  blockNum, blockCount);
 if (r!=RES_OK)
   return 0;
 else
   return blockCount;
#endif
}
int RtSdcard_device_media_parms(int unit, unsigned long *nSectors, unsigned long *BytesPerSector, int *isReadOnly)
{
DRESULT r;
#if (USE_AMAT_SD_DRIVE)
r = sdcard_ioctl (
    0,        /* Physical drive nmuber (0) */
    GET_SECTOR_COUNT,        /* Control code */
    (void *) (nSectors));
#else
  // Get nSectors
  r = SDMM_ioctl (
    0,        /* Physical drive nmuber (0) */
    GET_SECTOR_COUNT,        /* Control code */
    (void *) (nSectors));
#endif
 if (r!=RES_OK)
   return -1;

 *isReadOnly = 0;
 *BytesPerSector = 512;
  return 0;
}
int RtSdcard_attach(void) {
  rtsdcardReportInsertionEvent(0);
}
