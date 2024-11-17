#include "ili9341.h"

static void st7735_writeCommand(uint8_t commandByte){
    gpio_write(_ST7735_DC, false);
    gpio_write(_ST7735_CS, false);
    spi_write_blocking(spi0, &commandByte, 1);
    gpio_write(_ST7735_CS, true);
}

static void st7735_writeData(uint8_t dataByte){
    gpio_write(_ST7735_DC, true);
    gpio_write(_ST7735_CS, false);
    spi_write_blocking(spi0, &dataByte, 1);
    gpio_write(_ST7735_CS, true);
}

static void st7735_writeData_Buffer(unit8_t* dataBuf, uint32_t len){
    gpio_write(_ST7735_DC, true);
    gpio_write(_ST7735_CS, false);
    spi_write_blocking(spi0, dataBuf, len);
    gpio_write(_ST7735_CS, true);
}

static void st7735_reset(){
    gpio_write(_ST7735_RST, true);
    sleep_ms(10);
    gpio_write(_ST7735_RST, false);
    sleep_ms(10);
    gpio_write(_ST7735_RST, true);
    sleep_ms(10);
}

void st7735_initialize(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t sdata){
    _ST7735_RST = rst;
    _ST7735_DC = dc;
    _ST7735_CS = cs;
    _ST7735_SCLK = sclk;
    _ST7735_SDATA = sdata;
 
     // 1 perform gpio initialization
    _RST_INIT;
    _DC_INIT;
    _CS_INIT;
    _SCLK_INIT;
    _SDATA_INIT;   
   
    _RST_SetDigitalOut;
    _DC_SetDigitalOut;
    _CS_SetDigitalOut;
    _SCLK_SetDigitalOut;
    _SDATA_SetDigitalOut;

    st7735_reset();

    //initialize hardware spi instance type and speed
    spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz
    TFT_SCLK_SPI_FUNC; 
    TFT_SDATA_SPI_FUNC;

    /*
    // 2 screen setup
    //these adjustments to the lcd screen
    _col_start = 2;
    _row_start = 2; 
    //width & height
    _SCREEN_WIDTH = 128;
    _SCREEN_HEIGHT = 160;
    */
    
    //3 True hardware initialization Routine for 'ST7735R Black Tab'

    //research the initialization routines and etc
    gpio_write(_ST7735_DC, 0);
    gpio_write(_ST7735_CS, 1);
}
