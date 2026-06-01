#include <hardware/gpio.h>

#include <pico/platform/common.h>
#include <stdio.h>
#include <stddef.h>
#include <hardware/spi.h>
#include <hardware/pwm.h>
#include "pico/stdlib.h" 
#include "pinout.h"
#include "graphics_conf.h"

//temp for assets
#include "ampalaya_tileset_16.h"
#include "tilemaps.h"

#include "display.h"

//DUMMY.CPP PURPOSE: Test drivers, services without RTOS bloat/interference.
int alloc_system_tileset() {
	return 0 ;	
}

int run_system_startup() {
	return 0 ;
}

int main() {
	stdio_init_all();
	sleep_ms(5000);
	printf("GO");

	display_setup();

	while(true) {
		//BUTTON PROC
		
		//DISPLAY 
		engine_render(e);

		if( e->h_scanline_counter < HSCANLINE_MAX ) {
			engine_render(e); //TODO how much time does this take?
			e->h_scanline_counter++;
		} else {
			e->h_scanline_counter = 0; 
			//TODO increment sprite entities here
		}


#if HSCANLINE_RENDER
		//reconfigure the hw display write window to the next scanline
		
		ili9341_setCS_LO();

		uint16_t y0 = e->h_scanline_counter * HSCANLINE_SIZE;
		ili9341_setAddrWindow(0, y0, DEFAULT_TILE_LEN*DEFAULT_SCREEN_TILES_X, HSCANLINE_SIZE);
		ili9341_writeCommand(RAM_WR);
#endif
	}
}
	
