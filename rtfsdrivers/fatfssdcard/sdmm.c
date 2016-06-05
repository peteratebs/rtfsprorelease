/*------------------------------------------------------------------------/
/  Foolproof MMCv3/SDv1/SDv2 (in SPI mode) control module
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2013, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/
#include "diskio.h"        /* Common include file for FatFs and disk I/O layer */

/*-------------------------------------------------------------------------*/
/* Platform dependent macros and functions needed to be modified           */
/*-------------------------------------------------------------------------*/
DSTATUS SDMM_initialize (
    BYTE drv        /* Physical drive nmuber (0) */
)
{
   return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT SDMM_read (
    BYTE drv,            /* Physical drive nmuber (0) */
    BYTE *buff,            /* Pointer to the data buffer to store read data */
    DWORD sector,        /* Start sector number (LBA) */
    BYTE count            /* Sector count (1..256(0)) */
)
{
    return RES_OK;
}
/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT SDMM_write (
    BYTE drv,            /* Physical drive nmuber (0) */
    const BYTE *buff,    /* Pointer to the data to be written */
    DWORD sector,        /* Start sector number (LBA) */
    BYTE count            /* Sector count (1..256(0)) */
)
{
  return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT SDMM_ioctl (
    BYTE drv,        /* Physical drive nmuber (0) */
    BYTE ctrl,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
  return RES_OK;
}
