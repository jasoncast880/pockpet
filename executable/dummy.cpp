#include <cstddef>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "ili9341.h"

//test the screen hardware
int main() {
    ili9341_setAddrWindow(30,30,30,30);
		sleep_ms(10);

    for(int i = 0 ; i<30*30*2; i++) {
        ili9341_writeData(0xFF);
        ili9341_writeData(0xD0);
				sleep_ms(10);
    }
}
