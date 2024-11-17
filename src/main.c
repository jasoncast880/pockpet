#include <stdio.h>
#include "pico/stdlib.h"

#include "st7735.h"

//serves as a testbench for the things

int main() {
    stdio_init_all();

    sleep_ms(8000);
    printf("GO\n");

    //test util functions here 
    /*
    st7735_initialize(pisn here);
    st7735_setAddrWindow(here);
    st7735_drawFrame(&buffer, len);
    */
    
}
