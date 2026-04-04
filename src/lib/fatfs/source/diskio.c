//TODO:
//Write definitions for the Commands based on elmchan docs' abbrev. table
//


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


/* Definitions of physical drive number for each drive */
#define DEV_MMC		0	
#define DEV_RAM		1	
#define DEV_USB		2	

/* */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
        
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

		//power ON/card insertion 
		sleep_ms(10);
		gpio_init(SDC_CS); //todo: adding a pattern to control spi periph. thread safe.
		gpio_put(SPI0_TX, 1);
		gpio_put(SDC_CS, 1);
		sleep_ms(10);

		//software reset (CMD0, CS LOW)

		uint8_t dummy = 0x00;
		gpio_put(SDC_CS, 0);
		sdc_writeCommand(0x00, &dummy);
		gpio_put(SDC_CS, 1);

		uint8_t scratch;
		scratch = sdc_writeCommand(0x08, &dummy);

		if(scratch | 0x04) { //v1.0 sd card
			sdc_writeCommand(0x55, &dummy); //for ACMD
			scratch = sdc_writeCommand(0x41, &dummy);
			while(scratch | 0x01) {
				sleep_ms(10);
				scratch = sdc_writeCommand(0x41, &dummy);
				if(scratch | 0x04) {break;}
			}
		} else { //v2.0+ sd card
			if(!scratch) {
				
			}
		}

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
        gpio_put(SDC_CS, 0);
        result = spi_write_blocking((spi_inst_t*)spi0_hw, buff, count);
        gpio_put(SDC_CS, 1);
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
        gpio_put(SDC_CS, 0);
        spi_write_blocking((spi_inst_t*)spi0_hw, &cmd, 1);
        spi_read_blocking((spi_inst_t*)spi0_hw, 0xff, buff, 1);
        gpio_put(SDC_CS, 1);

        if ( *buff==0xff ) return RES_OK; //idk
        else return RES_ERROR;
        
    default: res = RES_PARERR;
    } return res;
}


