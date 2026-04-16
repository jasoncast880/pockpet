#include "tile_engine.h"

#include <cstddef>
#include <stdint.h>


static Layer* layer_id[NUM_LAYERS];
size_t layer_count = 0;

uint8_t add_layer(uint16_t *tileset, int num_tiles, uint8_t tilemap, uint8_t tiles_wide, uint8_t tiles_high) {
	uint8_t id;

	id = layer_count;
	Layer* layer = new Layer( tiles_wide, tiles_high, Tileset* tileset
			, uint8_t* map, id);


	return id;
}

uint8_t add_sprite(uint16_t *tileset, int num_tiles, uint8_t tilemap, uint8_t tiles_wide, uint8_t tiles_long, uint8_t layer_id) {
}
void update_sprite(uint8_t *map, uint8_t x, uint8_t y) {
}

void redner_layer(uint8_t layer_id) {

}
