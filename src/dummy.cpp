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

//
#include "usb.h"
//

//DUMMY.CPP PURPOSE: Test drivers, services without RTOS bloat/interference.

int main() {
	Tileset* sys_tileset = new Tileset( (uint16_t*)&ampalaya_tileset_16[0], static_cast<size_t>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*30) );
	Layer* screen = new Layer(
		static_cast<uint8_t>(DEFAULT_SCREEN_TILES_X),
		static_cast<uint8_t>(DEFAULT_SCREEN_TILES_Y),
		sys_tileset,
		&tile_bg_16[0],
		0
		); //id not relevant yet ? TODO: id handling system.

	Tileset* jet_tileset = new Tileset((uint16_t*)&jet_sprite_16[0],static_cast<size_t>(4096));
	uint8_t cursor = screen->sprite_add(
		2,
		2,
		jet_tileset,
		&demo_spritemap_1[0]
	);

	int x = 100;
	int y = 100;
	int i = 1;
	DisplayHandler& display = DisplayHandler::setup(screen); 
	//button_setup();
	//usb_setup();
	
	while(true) {
		sleep_ms(500); 

        screen->sprite_update_by_id(cursor, x, y, &demo_spritemap_4[0]);
		screen->render();

        display.draw_clean_tiles(screen);
		display.draw_dirty_tiles(screen);
		
		x--;
		//i =	(i == 4) ? 0 : i++;
	}
}
