/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pinout.h"

/* Definitions of physical drive number for each drive */
#define DEV_MMC		0	/* Example: Map MMC/SD card to physical drive 1 */
//if i add more later (i won't <@:) )
/*
#define DEV_RAM		1	
#define DEV_USB		2	
*/


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
    case DEV_MMC :
        
        if(spi_is_busy((spi_inst_t*)spi0_hw)) {
            stat = STA_PROTECT;
        }
        return stat;

    default: stat = STA_OK;
    } return stat;
} 

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_MMC :
        //is this necessary??
        result = spi_init((spi_inst_t*)spi0_hw, 8000*1000); //6MHz
        //result is the baud rate

        if(result == 0) {
            stat = STA_NOINIT;
        }

    default: stat = STA_OK;
	} return stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_MMC :
		// translate the arguments here

        result = spi_read_blocking((spi_inst_t*)spi0_hw, (uint16_t)0xff, buff, count);
        if (result == count) return RES_OK;
        else return RES_ERROR;
        
    default: res = RES_PARERR;
    } return res;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
){
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_MMC :
		// translate the arguments here
        result = spi_write_blocking((spi_inst_t*)spi0_hw, buff, count);
        if (result == count) return RES_OK;
        else return RES_ERROR;
        
    default: res = RES_PARERR;
    } return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	BYTE *buff		/* Buffer to send/receive control data */
){
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_MMC :
        spi_write_blocking((spi_inst_t*)spi0_hw, &cmd, 1);
        spi_read_blocking((spi_inst_t*)spi0_hw, 0xff, buff, 1);
        if ( *buff==0xff ) return RES_OK; //idk
        else return RES_ERROR;
        
    default: res = RES_PARERR;
    } return res;
}
