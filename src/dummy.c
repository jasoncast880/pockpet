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
#include "jet_sprite.h"
#include "tilemaps.h"

#include "display.h"

//DUMMY.CPP PURPOSE: Test display drivers without RTOS bloat/interference.

int main() {
	stdio_init_all();
	sleep_ms(5000);
	printf("GO");


	struct Layer* system_layer = add_layer(ampalaya_tileset_16, 30, tile_bg_16, 
			DEFAULT_SCREEN_TILES_X, DEFAULT_SCREEN_TILES_Y );
	Engine* e = engine_init(system_layer);
	
	display_setup(e);
	
	add_sprite(&jet_sprite_16[0], 4, &demo_spritemap_1[0], 2, 2, system_layer);

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
	
