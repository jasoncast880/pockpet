#include <stdio.h>
#include "pico/stdlib.h"

//#include "bgrDemo.h"
#include "landscape1.h"
#include "my_image.h"
#include "ili9341.h"

//serves as a testbench for the things

int main() {
    stdio_init_all();

    sleep_ms(4000);
    printf("GO\n");

    ili9341_initialize(17,20,21,19,6,16);

    //demmo here
    ili9341_setAddrWindow(10,10,150,200);
    sleep_ms(100);

    ili9341_writeCommand(RAM_WR);

    uint8_t red_kek[2] = {0xF8,0x00};
    for(int i = 0; i < (150*200);i++){
        ili9341_writeDataBuffer(&red_kek[0], 1);
    }
    ili9341_writeCommand(NOOP);

    sleep_ms(4000);

    ili9341_setAddrWindow(0,0,320,240);
    ili9341_writeCommand(RAM_WR);
    ili9341_writeDataBuffer((uint8_t*)landscape1, sizeof(landscape1));
    ili9341_writeCommand(NOOP);

}
