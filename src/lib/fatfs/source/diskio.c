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
#include <hardware/spi.h>
#include "diskio.h"		/* Declarations of disk functions */


/* Definitions of physical drive number for each drive */
#define DEV_MMC		0	
#define DEV_RAM		1	
#define DEV_USB		2	

/* */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/


#include "pinout.h"
#include "sdc_spi.h"

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	

} 

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
DSTATUS stat;

    sdc_initialize(SDC_CS, SPI0_BUS); 
    sdc_CS_LO();

    send_cmd(CMD_GO_IDLE_ST, 0);
    recv(recv_buf, 1);
    if(recv_buf[0]!=0x01) {
        return STA_NOINIT;
    }

    send_cmd(SEND_IF_COND, 0x000001AA);
    recv(recv_buf, 5);
    if(recv_buf[0]!=0x01) {
        return STA_NOINIT;
    }
    if(recv_buf[4]!=0xAA) {
        return STA_NOINIT;
    }

    send_cmd(READ_OCR, 0);
    recv(recv_buf, 5);
    if(recv_buf[0]!=0x01) {
        return STA_NOINIT;
    }
    uint8_t ocr = recv_buf[3] & 0x00; //!!!!
    //check ocr, identify card type.

    send_cmd(APP_CMD, 0);
    recv(recv_buf, 1);
    if(recv_buf[0]!=0x01) {
        return STA_NOINIT;
    }

    recv_buf[0] = 0x01; //idle state 0x01
    size_t timeout = 0; 

    while(recv_buf[0] != 0x00 ) {
        send_cmd(SD_SEND_OP_COND, (uint32_t) ocr << 29 );
        recv(recv_buf, 1);

        timeout++;
        if(timeout==1000) {
            return STA_NOINIT;
        }
    }
    
    send_cmd(READ_OCR, 0);
    recv(recv_buf, 5);
    if(recv_buf[0]) {
        return STA_NOINIT;
    }
    //read the ccs for capacity information, etc.

    return STA_OK;

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
	uint32_t arg = 0;

	//assume you have a 512by block TODO 
	if(count == 1) {
		send_cmd(RD_SINGLE_BLOCK, sector*512);
		recv(recv_buf, 1);
		if(recv_buf[0]) {
			return RES_ERROR; 
		}
		if(recv( (uint8_t*) buff, (size_t) count*512 )) {
			return RES_ERROR;
		}

		return RES_OK;
	} else { //mult
		send_cmd(RD_MULT_BLOCK, sector*512);
		recv(recv_buf, 1);
		if(recv_buf[0]) {
			return RES_ERROR; 
		}
		if(recv( (uint8_t*) buff, (size_t) count*512 )) {
			return RES_ERROR;
		} 
		send_cmd(STOP_TRANS, 0);
		recv(recv_buf, 2);
		if(recv_buf[0]) {
			return RES_ERROR; 
		} //TODO consider second by

		return RES_OK;
	}
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


