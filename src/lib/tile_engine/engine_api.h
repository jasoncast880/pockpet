#ifndef ENGINE_API_H
#define ENGINE_API_H

//#include "pico/stddef.h"
//#include "tile_engine.hpp"

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct { 
	struct Layer* layer;
	uint16_t* render_data; //variable size, constant location.
	size_t render_buf_size;

	uint8_t h_scanline_counter; //use for hscanline renders

	uint16_t x,y; //for buf. true count
	uint8_t x_tile, y_tile; //for tile-by-tile read.
} Engine;

Engine* engine_init(struct Layer* layer);
uint16_t* engine_render(Engine* e); //needs to return a contiguous arr

struct Layer* add_layer(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high); 
struct Sprite* add_sprite(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high, struct Layer* associated_layer);

int update_layer(struct Layer* layer, uint8_t* map, uint8_t x, uint8_t y);
int update_sprite(struct Layer* layer, uint8_t sprite_id, uint8_t* map, uint8_t x, uint8_t y);

uint16_t* get_framebuf_data(struct Layer* layer);

int delete_layer(struct Layer* layer);
int delete_sprite(struct Sprite* sprite);

#ifdef __cplusplus
} //extern "C"
#endif 

#endif //ENGINE_API_H
