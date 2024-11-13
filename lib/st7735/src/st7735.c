#include "st7735.h"

void writeCommand(uint8_t commandByte){
}

void writeData(uint8_t dataByte){
}

st7735_initialize(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t sdata){
    
    //initialize hardware spi instance type and speed
    spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz
    TFT_SCLK_SPI_FUNC; 
    TFT_SDATA_SPI_FUNC;

     // 1 perform gpio initialization
    _ST7735_RST = rst;
    _ST7735_DC = dc;
    _ST7735_CS = cs;
    _ST7735_SCLK = sclk;
    _ST7735_SDATA = sdata;

    _RST_INIT;
    _DC_INIT;
    _CS_INIT;
    _SCLK_INIT;
    _SDATA_INIT;   //set the gpio

    // 2 screen setup
    
    //these 2 vars are adjustments to the lcd screen
    _col_start = 2;
    _row_start = 2; 
    //width & height
    _SCREEN_WIDTH = 128;
    _SCREEN_HEIGHT = 160;
    
    //3 True hardware initialization Routine for 'ST7735R Black Tab'
    /*
     * initpcbtype(mytft.tft_st7735s_black) 
     *  \--> TFTBlackTabInitialize
     */


}
