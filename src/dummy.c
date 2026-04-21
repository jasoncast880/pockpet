#include <pico/stdio.h>
#include <stddef.h>
#include <hardware/spi.h>
#include <stdio.h>
#include "pico/stdlib.h" 
#include "pinout.h"
#include "graphics_conf.h"

#include "display.h"

//DUMMY.CPP PURPOSE: Test drivers, services without RTOS bloat/interference.

int main() {
	stdio_init_all();
	sleep_ms(5000);
	printf("GO");

	display_setup();

	uint8_t blue[2] = {0x00,0x1f};
	uint8_t red[2]  = {0xf8,0x00};

	for(int i = 0 ; i < 10 ; i++) {
		ili9341_setCS_LO();

		ili9341_setAddrWindow(0, i * DEFAULT_TILE_LEN, DEFAULT_TILE_LEN, DEFAULT_TILE_LEN);
		for(int j = 0 ; j < DEFAULT_TILE_LEN * DEFAULT_TILE_LEN ; j++) {
			ili9341_writeData(blue[0]);
			ili9341_writeData(blue[1]);
		}

		ili9341_setCS_HI();

		sleep_ms(100);

		ili9341_setCS_LO();

		ili9341_setAddrWindow(DEFAULT_TILE_LEN, i * DEFAULT_TILE_LEN, DEFAULT_TILE_LEN, DEFAULT_TILE_LEN);
		for(int j = 0 ; j < DEFAULT_TILE_LEN * DEFAULT_TILE_LEN ; j++) {
			ili9341_writeData(red[0]);
			ili9341_writeData(red[1]);
		}

		ili9341_setCS_HI();

	}
	
}
