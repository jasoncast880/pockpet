#include <stdio.h>
#include "pico/stdlib.h"

#include "ili9341.h"

//serves as a testbench for the things

int main() {
    stdio_init_all();

    sleep_ms(8000);
    printf("GO\n");

    //test util functions here 
    ili9341_initialize(17,20,21,19,6,16);
    /*
    st7735_initialize(pisn here);
    st7735_setAddrWindow(here);
    st7735_drawFrame(&buffer, len);
    */
    
}
