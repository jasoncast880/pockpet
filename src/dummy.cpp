#include <cstddef>
#include <hardware/spi.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "display.h"
#include "buttons.h"

#include <ampalaya_tileset_16.h>
#include <tilemaps.h>


//DUMMY.CPP PURPOSE: Test drivers, services without RTOS bloat/interference.

int main() {

	
	//TEMP required globs
	Layer* screen = new Layer(
		static_cast<uint8_t>(DEFAULT_SCREEN_TILES_X),
		static_cast<uint8_t>(DEFAULT_SCREEN_TILES_Y),
		new Tileset(
			(uint16_t*)&ampalaya_tileset_16[0],
			static_cast<size_t>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*30)
			),
		&tile_bg_16[0],
		0 //id not relevant yet ? TODO: id handling system.
	);

	DisplayHandler& display = DisplayHandler::setup(screen);
	button_setup();

	dma_hw->intf0 = 1u << DisplayHandler::cmd_chan; //CRUCIAL: kick start the isr

	while(true) {
		sleep_ms(50);
		int8_t input_flag = btn_sample;

		//stack is setup. relatively thread-safe. display.draw_frame(Layer);

	}
}
