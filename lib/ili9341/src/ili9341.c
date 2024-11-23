#include "ili9341.h"
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
    spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz
    _SCLK_SPI_FUNC;
    _MOSI_SPI_FUNC;

    ili9341_hard_reset(); 
    ili9341_init_sub_pwr();

    ili9341_writeCommand(COLMOD);
    ili9341_writeData(0x55);

    //test
    sleep_ms(500);

    ili9341_writeCommand(RAM_WRT);

    uint8_t red_kek[2] = {0x00, 0xF8};
    for(int i = 0; i < (240*380);i++){
        ili9341_writeData_Buffer(&red_kek[0], 2);
    }
}

static void ili9341_writeCommand(uint8_t commandByte){
    gpio_put(_ILI9341_DC, false);
    gpio_put(_ILI9341_CS, false);
    spi_write_blocking(spi0, &commandByte, 1);
    gpio_put(_ILI9341_CS, true);
}

static void ili9341_writeData(uint8_t dataByte){
    gpio_put(_ILI9341_DC, true);
    gpio_put(_ILI9341_CS, false);
    spi_write_blocking(spi0, &dataByte, 1);
    gpio_put(_ILI9341_CS, true);
}

static void ili9341_writeData_Buffer(uint8_t* dataBuf, size_t len){
    gpio_put(_ILI9341_DC, true);
    gpio_put(_ILI9341_CS, false);
    spi_write_blocking(spi0, dataBuf, len);
    gpio_put(_ILI9341_CS, true);
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
}

static void ili9341_init_sub_vram(){
    ili9341_writeCommand(NOOP);
}


