#include <stdio.h>
#include "pico/stdlib.h"

//#include "portraitDemo.hpp" //contains pointer to the pixel array data
#include "ili9341.h"

int main() {
    stdio_init_all();

    sleep_ms(8000);
    printf("GO\n");

    //test util functions here 
    ili9341_initialize(17,20,21,19,6,16);
    //ili9341_writeDataBuffer(ptr_portrait, sizeof(ptr_portrait));
    
}
