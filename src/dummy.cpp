#include <cstddef>
#include <hardware/spi.h>
#include <stdio.h>
#include "jet_sprite.h"
#include "pico/stdlib.h"
#include "pinout.h"

#include "display.h"
#include "buttons.h"
#include "tile_engine.h"

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
		0 
		); //id not relevant yet ? TODO: id handling system.

	uint8_t cursor = screen->sprite_add(
		2,
		2,
		new Tileset((uint16_t*)&jet_sprite_16[0],static_cast<size_t>(16)),
		&demo_spritemap_1[0]
	);

	int x = 100;
	int y = 100;
	DisplayHandler& display = DisplayHandler::setup(screen); //setup should call layer-0
	button_setup();

	while(true) {
		sleep_ms(500); 
		int8_t input_flag = btn_sample; //TODO: Segault occurs here

		//do some kind of software (tile engine calls) render 
		if(input_flag & 0b0000'1000) {
			x--;
			screen->sprite_update_by_id(cursor, x, y, &demo_spritemap_1[0]);
			screen->render();
		}

		sleep_ms(500);
		display.draw_frame(screen);

		sleep_ms(500);
	}
}
