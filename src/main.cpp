#include <stdio.h>
#include "pico/stdlib.h"

//#include "landscape1.h"
#include "roll_scroll_demo.h"
#include "ili9341.h"

//serves as a testbench for the things

int main() {
    stdio_init_all();

    sleep_ms(3000);
    printf("GO\n");

    ili9341_initialize(17,20,21,19,6,16);

    //demmo here
    /*
    ili9341_setAddrWindow(40,40,150,150);
    ili9341_writeCommand(RAM_WR);
    uint8_t red_kek[2] = {0xF8,0x00};
    for(int i = 0; i < (150*150);i++){
        ili9341_writeData(red_kek[0]);
        ili9341_writeData(red_kek[1]);
        sleep_ms(1);
    }
    ili9341_writeCommand(NOOP);

    ili9341_setAddrWindow(50,50,20,20);
    ili9341_writeCommand(RAM_WR);
    uint8_t grn_kek[2] = {0x07,0xE0};
    for(int i = 0; i < (20*20);i++){
        ili9341_writeData(grn_kek[0]);
        ili9341_writeData(grn_kek[1]);
        sleep_ms(1);
    }
    ili9341_writeCommand(NOOP);

    ili9341_setAddrWindow(160,50,20,20);
    ili9341_writeCommand(RAM_WR);
    uint8_t ylo_kek[2] = {0xFF,0xC0};
    for(int i = 0; i < (20*20);i++){
        ili9341_writeData(ylo_kek[0]);
        ili9341_writeData(ylo_kek[1]);
        sleep_ms(1);
    }
    ili9341_writeCommand(NOOP);

    ili9341_setAddrWindow(160,100,20,20);
    ili9341_writeCommand(RAM_WR);
    uint8_t blu_kek[2] = {0x00,0x1F};
    for(int i = 0; i < (20*20);i++){
        ili9341_writeData(blu_kek[0]);
        ili9341_writeData(blu_kek[1]);
        sleep_ms(1);
    }
    ili9341_writeCommand(NOOP);

    sleep_ms(2000);
    */

    /*
    ili9341_setAddrWindow(0,0,320,240);
    ili9341_writeCommand(RAM_WR);
    ili9341_writeDataBuffer((uint8_t*)landscape1, sizeof(landscape1));
    ili9341_writeCommand(NOOP);
    */

    ili9341_setAddrWindow(0,0,320,240);
    ili9341_writeCommand(RAM_WR);
    ili9341_writeDataBuffer((uint8_t*)roll_scroll_demo, sizeof(roll_scroll_demo));
    ili9341_writeCommand(NOOP);

    sleep_ms(500);
    ili9341_setScrollWindow(0,240,0);

    sleep_ms(500);

    ili9341_setScrollPtr(120);

    sleep_ms(500);

    ili9341_writeCommand(RAM_WR);
    ili9341_writeDataBuffer((uint8_t*)roll_scroll_demo, sizeof(roll_scroll_demo));
    ili9341_writeCommand(NOOP);
}
