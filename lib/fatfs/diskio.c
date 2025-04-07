/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"
#include "sdc_driver.h"  

DSTATUS disk_initialize(BYTE pdrv) {
    return sdc_init(5,19,4,18) ? 0 : STA_NOINIT;
}

DSTATUS disk_status(BYTE pdrv) {
    return 0;  // Always ready for now
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
    return sdc_read(buff, sector, count) ? RES_OK : RES_ERROR;
}

#if _USE_WRITE
DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
    return sdc_write((uint8_t *)buff, sector, count) ? RES_OK : RES_ERROR;
}
#endif

#if _USE_IOCTL
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    switch (cmd) {
        case CTRL_SYNC: return RES_OK;
        case GET_SECTOR_SIZE: *(WORD *)buff = 512; return RES_OK;
        case GET_BLOCK_SIZE:  *(DWORD *)buff = 1; return RES_OK;
        case GET_SECTOR_COUNT: *(DWORD *)buff = sd_card_sector_count(); return RES_OK;
        default: return RES_PARERR;
    }
}
#endif

