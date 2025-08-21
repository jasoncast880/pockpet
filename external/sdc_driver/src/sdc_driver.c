/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* Declarations of disk functions, include HAL */
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

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
    return sdc_write((uint8_t *)buff, sector, count) ? RES_OK : RES_ERROR;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    switch (cmd) {
        case CTRL_SYNC: return RES_OK;
        case GET_SECTOR_SIZE: *(WORD *)buff = 512; return RES_OK;
        case GET_BLOCK_SIZE:  *(DWORD *)buff = 1; return RES_OK;
        default: return RES_PARERR;
    }
}


DWORD get_fattime (void)
{
    return 0;
}

//HAL HERE (BELOW) ************************************************************************

int8_t _SDC_CS;
int8_t _SDC_MOSI;
int8_t _SDC_MISO;
int8_t _SDC_SCLK;

static void spi_select(){
    gpio_put(_SDC_CS, 1);
}

static void spi_deselect(){
    gpio_put(_SDC_CS, 0);
}

uint8_t sdc_send_command(uint8_t cmd, uint32_t arg){
    //break the arg off into byte segments
    uint8_t buf[6];
    buf[0] = cmd | 0x40;
    buf[1] = (arg >> 24 ) & 0xFF;
    buf[2] = (arg >> 16 ) & 0xFF;
    buf[3] = (arg >> 8 ) & 0xFF;
    buf[4] = arg & 0xFF;
    buf[5] = (cmd == CMD0) ? 0x95 : 0x01;

    spi_select();
    //send the bytes over spi, (not thread safe)
    for(int i = 0; i<6; i++){
        spi_write_blocking(spi0, &buf[i],1);
    }
    //wait and return a response, deselect
    uint8_t res;
    for(int i = 0; i<8; i++){
        spi_read_blocking(spi0, 0xff, &res, 1);
        if(!(res&0x80)) break;
    }
    spi_deselect();
    return res;
}

bool sdc_init(int8_t cs, int8_t mosi, int8_t miso, int8_t sclk){

    spi_init(spi0, 400 * 1000);

    _SDC_CS = cs;
    _SDC_MOSI = mosi;
    _SDC_MISO = miso;
    _SDC_SCLK = sclk;

    _CS_INIT;
    _MOSI_INIT;
    _MISO_INIT;
    _SCLK_INIT;

    _CS_SetPinOut;
    _MOSI_SetPinOut;
    _SCLK_SetPinOut;
    _MISO_SetPinIn;

    gpio_put(_SDC_CS, 1);

    //wakeup pulse (80 cycles)
    uint8_t dummy = 0xFF;
    for(int i = 0;i<10;i++){
        spi_write_blocking(spi0, &dummy, 1);
    }

    //sw reset cmd0
    if(sdc_send_command(CMD0,0)!=0x01){
        return false;
    }

    //cmd8 (acceptable voltage range check)
    if(sdc_send_command(CMD8,0x000001AA)!=0x01){
        return false;
    }

    //wait for ready from sdc
    for(int i = 0;i<1000;i++){
        if(sdc_send_command(CMD55,0)==0x01 && sdc_send_command(ACMD41, 0x40000000)==0x00){
            return true;
        }
        sleep_ms(10);
    }
    return false;
}

bool sdc_read(uint8_t *buf, uint32_t sector, uint32_t count){ //PRONE TO IMPLICIT LEAKS, OVERFLOW
    uint8_t CMDX = (count==1) ? CMD17 : CMD18;

    switch(CMDX){
    case CMD17:
        if(sdc_send_command(CMDX,sector)!=0x00) return false;

        uint8_t token;
        for(int i = 0;i<100;i++){
            spi_read_blocking(spi0, 0xFF, &token, 1);
            if(token==0xFE) break;
        }
        if(token!=0xFE) return false;

        //reads 512 bytes
        for(int i = 0; i<512; i++){
            spi_read_blocking(spi0,0xFF,&buf[i],1);
        }

        //read, discard CRC
        spi_read_blocking(spi0,0xFF,&token,1);
        spi_read_blocking(spi0,0xFF,&token,1);
        return true;

    case CMD18:
        if(sdc_send_command(CMDX,sector)!=0x00) return false;

        for(int i = 0; i<count; i++){
            //continuosly get 512 byte blocks until count over (same procedure as cmd17)
            uint8_t token;
            for(int j = 0;j<100;j++){
                spi_read_blocking(spi0, 0xFF, &token, 1);
                if(token==0xFE) break;
            }
            if(token!=0xFE) return false;

            //read 512 bytes
            for(int j=0;j<512;j++){ 
                spi_read_blocking(spi0,0xFF,&buf[i*512+j],1);
            }
            //crc irrelevant
            spi_read_blocking(spi0,0xFF,&token,1);
            spi_read_blocking(spi0,0xFF,&token,1);
        }

        if(sdc_send_command(CMD12,sector)!=0x00) return false; //maybe change

        return true;
    }
}

bool sdc_write(uint8_t *buf, uint32_t sector, uint32_t count){
    uint8_t CMDX = (count==1) ? CMD24 : CMD25;

    switch(CMDX){
    case CMD24: //write one block
        if(sdc_send_command(CMDX,sector)!=0x00) return false;

        //continuosly get 512 byte blocks until count over (same procedure as cmd17)
        uint8_t token = 0xFE;
        spi_write_blocking(spi0, &token, 1);

        //write 512 bytes
        for(int j=0;j<512;j++){ 
            spi_write_blocking(spi0,&buf[j],1);
        }
        //crc irrelevant
        spi_write_blocking(spi0,0xFF,1);
        spi_write_blocking(spi0,0xFF,1);

        spi_read_blocking(spi0, 0xFF, &token, 1);
        return (token & 0x1F) == 0x05;

    case CMD25:
        if(sdc_send_command(CMDX,sector)!=0x00) return false;
        for(int i = 0; i<count; i++){
            //continuosly get 512 byte blocks until count over (same procedure as cmd17)
            uint8_t token = 0xFC;
            spi_write_blocking(spi0, &token, 1);

            //write 512 bytes
            for(int j=0;j<512;j++){ 
                spi_write_blocking(spi0,&buf[i*512+j],1);
            }
            //crc irellevant
            spi_write_blocking(spi0,0xFF,1);
            spi_write_blocking(spi0,0xFF,1);
        }

        //send trans. stop token, etc.
        token = 0xFD;
        //send a dummy byte
        spi_write_blocking(spi0,0xFF,1);

        return true;   
    }
}

bool sdc_ioctl(uint32_t cmd, void* buf){
    switch(cmd){
        case(CMD58): {
            uint8_t* ocr = (uint8_t*)buf;
            if(sdc_send_command(CMD58,0)!=0) return false;
            for(int i = 0;i<4;i++){
                spi_read_blocking(spi0,0xFF,&ocr[i],1);
            }
            return true;
        }
        default:
            return false;
    }
}

