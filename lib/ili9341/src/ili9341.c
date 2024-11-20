#include "ili9341.h"

static void ili9341_writeCommand(uint8_t commandByte){
    gpio_write(_ILI9341_DC, false);
    gpio_write(_ILI9341_CS, false);
    spi_write_blocking(spi0, &commandByte, 1);
    gpio_write(_ILI9341_CS, true);
}

static void ili9341_writeData(uint8_t dataByte){
    gpio_write(_ILI9341_DC, true);
    gpio_write(_ILI9341_CS, false);
    spi_write_blocking(spi0, &dataByte, 1);
    gpio_write(_ILI9341_CS, true);
}

static void ili9341_writeData_Buffer(unit8_t* dataBuf, uint32_t len){
    gpio_write(_ILI9341_DC, true);
    gpio_write(_ILI9341_CS, false);
    spi_write_blocking(spi0, dataBuf, len);
    gpio_write(_ILI9341_CS, true);
}

static void ili9341_hard_reset(){
    gpio_write(_ILI9341_RST, true);
    sleep_ms(10);
    gpio_write(_ILI9341_RST, false);
    sleep_ms(10);
    gpio_write(_ILI9341_RST, true);
    sleep_ms(10);
}

void ili9341_initialize(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t sdata){
    _ILI9341_RST = rst;
    _ILI9341_DC = dc;
    _ILI9341_CS = cs;
    _ILI9341_SCLK = sclk;
    _ILI9341_SDATA = sdata;
 
    _RST_INIT;
    _DC_INIT;
    _CS_INIT;
    _SCLK_INIT;
    _SDATA_INIT;   
   
    gpio_write(_ILI9341_DC, 0);
    gpio_write(_ILI9341_CS, 1);

    _RST_SetDigitalOut;
    _DC_SetDigitalOut;
    _CS_SetDigitalOut;
    _SCLK_SetDigitalOut;
    _SDATA_SetDigitalOut;

    //initialize hardware spi instance type and speed
    spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz
    TFT_SCLK_SPI_FUNC;
    TFT_SDATA_SPI_FUNC;

    ili9341_hard_reset(); //will reset all vram to random

    
}
