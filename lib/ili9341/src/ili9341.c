#include "ili9341.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

int8_t _ILI9341_CS;
int8_t _ILI9341_RST;
int8_t _ILI9341_DC;
int8_t _ILI9341_MOSI;
int8_t _ILI9341_SCLK;
int8_t _ILI9341_MISO;

void ili9341_initialize(int8_t cs, int8_t rst, int8_t dc, int8_t mosi, int8_t sclk, int8_t miso){
    /*
     * testbench:
     * cs - blue - gp17
     * rst - ylo - gp20
     * dc - grn  - gp 21
     * mosi - wht - gp19
     * sck - blu - gp6
     * led - vcc line
     * miso - blk - gp16
     */

    _ILI9341_CS = cs;
    _ILI9341_RST = rst;
    _ILI9341_DC = dc;
    _ILI9341_MOSI = mosi;
    _ILI9341_SCLK = sclk;
    _ILI9341_MISO = miso;
 
    _CS_INIT;
    _RST_INIT;
    _DC_INIT;
    _MOSI_INIT;
    _SCLK_INIT;
    _MISO_INIT;   
   
    _CS_SetDigitalOut;
    _RST_SetDigitalOut;
    _DC_SetDigitalOut;
    _MOSI_SetDigitalOut;
    _SCLK_SetDigitalOut;
    _MISO_SetDigitalOut;

    gpio_put(_ILI9341_DC, 0);
    gpio_put(_ILI9341_CS, 1);

    //initialize hardware spi instance type and speed
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz
    _SCLK_SPI_FUNC;
    _MOSI_SPI_FUNC;

    ili9341_hard_reset(); 
    ili9341_init_sub_pwr();
    //test
    sleep_ms(500);

    ili9341_writeCommand(RAM_WR);

    uint8_t red_kek[2] = {0x00, 0xF8};
    for(int i = 0; i < (240*380);i++){
        ili9341_writeDataBuffer(&red_kek[0], sizeof(red_kek));
    }
    ili9341_writeCommand(NOOP);

    //demo things
    /*
    ili9341_setAddrWindow(10,10,20,20);

    sleep_ms(100);

    ili9341_writeCommand(RAM_WR);
    uint8_t blue_kek[2] = {0xF8,0x00};
    for(int i = 0; i<=((20)*(20)); i++){
        ili9341_writeDataBuffer(&blue_kek[0], sizeof(blue_kek));
    }
    ili9341_writeCommand(NOOP);

    //ili9341_setAddrWindow(70,70,20,20);

    ili9341_writeCommand(CASET);  //x0,x1
    //accomodate for little endian-ness w bit shifting.
    ili9341_writeData(0);
    ili9341_writeData(70);
    ili9341_writeData(0);
    ili9341_writeData(90);

    ili9341_writeCommand(RASET);

    ili9341_writeData(0);
    ili9341_writeData(70);
    ili9341_writeData(0);
    ili9341_writeData(90);

    sleep_ms(100);

    ili9341_writeCommand(RAM_WR);
    for(int i = 0; i<=((20)*(20)); i++){
        ili9341_writeDataBuffer(&blue_kek[0], sizeof(blue_kek));
    }
    ili9341_writeCommand(NOOP);
    */
    //end of demo things
}

void ili9341_writeCommand(uint8_t commandByte){
    gpio_put(_ILI9341_DC, false);
    gpio_put(_ILI9341_CS, false);
    spi_write_blocking(spi0, &commandByte, 1);
    gpio_put(_ILI9341_CS, true);
}

void ili9341_writeData(uint8_t dataByte){
    gpio_put(_ILI9341_DC, true);
    gpio_put(_ILI9341_CS, false);
    spi_write_blocking(spi0, &dataByte, 1);
    gpio_put(_ILI9341_CS, true);
}

void ili9341_writeDataBuffer(uint8_t* dataBuf, size_t len){
    gpio_put(_ILI9341_DC, true);
    gpio_put(_ILI9341_CS, false);
    spi_write_blocking(spi0, dataBuf, len);
    gpio_put(_ILI9341_CS, true);
}

//before and after use; *must* spi_set_format(spi, x-bits, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
void ili9341_writeData16Buffer(uint16_t* dataBuf, size_t len){
    gpio_put(_ILI9341_DC, true);
    gpio_put(_ILI9341_CS, false);
    spi_write16_blocking(spi0, dataBuf, len);
    gpio_put(_ILI9341_CS, true);
}

static uint16_t _cursorx_ = 0x0000;
static uint16_t _cursory_ = 0x0000;

//240x320-pixel dims - wip
uint32_t ili9341_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h){
    //
    //update the cursor value to reflect the new x0 y0 coords for the address Window 
    _cursorx_ = x0;
    _cursory_ = y0;

    //spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    uint16_t x1 = x0+w-1;
    uint16_t y1 = y0+h-1;

    ili9341_writeCommand(CASET);  //x0,x1
    ili9341_writeData(0x00);
    ili9341_writeData(0x0A);
    ili9341_writeData(0x00);
    ili9341_writeData(0x1E);

    ili9341_writeCommand(RASET);
    ili9341_writeData(0x00);
    ili9341_writeData(0x0A);
    ili9341_writeData(0x00);
    ili9341_writeData(0x1E);

    //go back to regular spi format
    //spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    return ((w)*(h));
}

static void ili9341_hard_reset(){
    gpio_put(_ILI9341_RST, true);
    sleep_ms(10);
    gpio_put(_ILI9341_RST, false);
    sleep_ms(10);
    gpio_put(_ILI9341_RST, true);
    sleep_ms(120);
}

static void ili9341_init_sub_pwr(){
    ili9341_writeCommand(SWRESET);
    ili9341_writeCommand(SLPOUT);
    sleep_ms(120);
    ili9341_writeCommand(DISPON);
    sleep_ms(100);

    ili9341_writeCommand(PIXSET);
    ili9341_writeData(0x55); //set the pixel format to RGB 5-6-5
}

static void ili9341_init_sub_vram(){
    ili9341_writeCommand(NOOP);
}
