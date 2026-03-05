#include "ili9341.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <hardware/spi.h>

static uint8_t _ILI9341_CS;
static uint8_t _ILI9341_RST;
static uint8_t _ILI9341_DC;
static uint8_t _ILI9341_MOSI;
static uint8_t _ILI9341_SCLK;
static uint8_t _ILI9341_MISO;

//note after init, you still have to put cs low in order to write spi.
void ili9341_initialize(int8_t cs, int8_t rst, int8_t dc) { 

    _ILI9341_CS = cs;
    _ILI9341_RST = rst;
    _ILI9341_DC = dc;

    gpio_init(cs);
    gpio_init(rst);
    gpio_init(dc);
   
    gpio_set_dir(cs, GPIO_OUT);
    gpio_set_dir(rst, GPIO_OUT);
    gpio_set_dir(dc, GPIO_OUT);

    gpio_put(dc, 0);
    gpio_put(cs, 1);

    ili9341_hard_reset(); 
    ili9341_init_sub_pwr();

    ili9341_writeCommand(0xC0); // Power Control 1
    ili9341_writeData(0x23);

    ili9341_writeCommand(0xC1); // Power Control 2
    ili9341_writeData(0x10);

    ili9341_writeCommand(0xC5); // VCOM control 1
    ili9341_writeData(0x3e);
    ili9341_writeData(0x28);

    ili9341_writeCommand(0xC7); // VCOM control 2
    ili9341_writeData(0x86);

    ili9341_writeCommand(0xB1); // Frame Rate Control
    ili9341_writeData(0x00);
    ili9341_writeData(0x18);

    ili9341_writeCommand(0xB6); // Display Function Control
    ili9341_writeData(0x08);
    ili9341_writeData(0x82);
    ili9341_writeData(0x27);

    ili9341_init_sub_vram();

    ili9341_writeCommand(DISPON);
    sleep_ms(100);
}

void ili9341_writeCommand(uint8_t commandByte){
    gpio_put(_ILI9341_DC, 0);
    spi_write_blocking(spi0, &commandByte, 1);
}

void ili9341_writeData(uint8_t dataByte){
    gpio_put(_ILI9341_DC, 1);
    spi_write_blocking(spi0, &dataByte, 1);
}

void ili9341_writeDataBuffer8(uint8_t* dataBuf, size_t len){ 
    gpio_put(_ILI9341_DC, 1);
		spi_write_blocking(spi0, dataBuf, len);
}
void ili9341_writeDataBuffer16(uint16_t* dataBuf, size_t len){ 
    gpio_put(_ILI9341_DC, 1);
    spi_write16_blocking(spi0, dataBuf, len);
}

//commands abstracted
void ili9341_setScrollWindow(uint16_t tfa, uint16_t vsa, uint16_t bfa){
    ili9341_writeCommand(VSCR_DEF);
    ili9341_writeData((uint8_t)(tfa>>8));
    ili9341_writeData((uint8_t)(tfa&0xFF));
    ili9341_writeData((uint8_t)(vsa>>8));
    ili9341_writeData((uint8_t)(vsa&0xFF));
    ili9341_writeData((uint8_t)(bfa>>8));
    ili9341_writeData((uint8_t)(bfa&0xFF));
    sleep_ms(10);
}

void ili9341_setScrollPtr(uint16_t vsp){
    ili9341_writeCommand(VSCR_ADD);
    ili9341_writeData((uint8_t)(vsp>>8));
    ili9341_writeData((uint8_t)(vsp&0xFF));
    sleep_ms(10);
}

void ili9341_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h) { 
    uint16_t x1 = x0+w-1;
    uint16_t y1 = y0+h-1;

    ili9341_writeCommand(CASET);
    ili9341_writeData((uint8_t)(x0>>8));
    ili9341_writeData((uint8_t)(x0&0xFF));
    ili9341_writeData((uint8_t)(x1>>8));
    ili9341_writeData((uint8_t)(x1&0xFF));

    ili9341_writeCommand(RASET);
    ili9341_writeData((uint8_t)(y0>>8));
    ili9341_writeData((uint8_t)(y0&0xFF));
    ili9341_writeData((uint8_t)(y1>>8));
    ili9341_writeData((uint8_t)(y1&0xFF));
} //YOU MUST FOLLOW WITH A RAMWR, THEN DO A 16 bit write

static void ili9341_hard_reset(){
    gpio_put(_ILI9341_RST, 1);
    sleep_ms(10);
    gpio_put(_ILI9341_RST, 0);
    sleep_ms(10);
    gpio_put(_ILI9341_RST, 1);
    sleep_ms(120);
}

static void ili9341_init_sub_pwr(){
    //call to set power & electrical presets
    ili9341_writeCommand(SWRESET);
    sleep_ms(150);
    ili9341_writeCommand(SLPOUT);
    sleep_ms(120);
}

static void ili9341_init_sub_vram(){
    //call to set controller's display orientation, pixel format
    ili9341_writeCommand(PIXSET);
    ili9341_writeData(0x55); //set the pixel format to RGB 5-6-5

    ili9341_writeCommand(MADCTL);
    ili9341_writeData(0xE8);
    ili9341_setAddrWindow(0,0,320,240); //recalibrate addressing to fit the whole frame. 
}
