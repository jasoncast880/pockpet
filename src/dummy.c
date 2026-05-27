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

	/*
	gpio_set_function(2, GPIO_FUNC_PWM);
	uint slice = pwm_gpio_to_slice_num(2);

	pwm_set_wrap(slice, 1000);              // frequency control
	pwm_set_chan_level(slice, PWM_CHAN_A, 500); // 50% duty cycle

	pwm_set_enabled(slice, true);
	
	while(1) {
		tight_loop_contents();
	}
	*/

	display_setup();

    Engine* e = engine_init(add_layer( &ampalaya_tileset_16[0], 30, &tile_bg_16[0], 320/DEFAULT_TILE_LEN,  240/DEFAULT_TILE_LEN)); 

	while(true) {
		//BUTTON PROC
		
		//DISPLAY PROC
		if(!render_flag) {


			engine_render(e);
			//todo: parse inputs into engine api calls

			render_flag = true;
		} 

		if(render_flag) {
#if HSCANLINE_RENDER
			//reconfigure the hw display write window to the next scanline
			
			uint16_t y0 = 0; //TODO needs to increment based on hsline counter
			ili9341_setAddrWindow(0, y0, DEFAULT_TILE_LEN*DEFAULT_SCREEN_TILES_X, HSCANLINE_SIZE);
			
			//reconfigure the dma channel to start of engine's render_data
			//start the channel and let it run, on end it will set flags via isr
#endif
		}
	}
}
	
