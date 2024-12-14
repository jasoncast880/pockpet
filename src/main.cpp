#include <stdio.h>
#include "pico/stdlib.h"

//#include "bgrDemo.h"
#include "my_image.h"
#include "ili9341.h"

//serves as a testbench for the things

int main() {
    stdio_init_all();

    sleep_ms(8000);
    printf("GO\n");

    ili9341_initialize(17,20,21,19,6,16);

    ili9341_writeCommand(RAM_WR);
    //ili9341_writeDataBuffer((uint8_t*)ili9341_Demo1, sizeof(ili9341_Demo1));
    ili9341_writeDataBuffer((uint8_t*)my_image, sizeof(my_image));
    ili9341_writeCommand(NOOP);

}
