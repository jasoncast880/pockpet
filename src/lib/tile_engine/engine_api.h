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

	uint16_t x,y; //for buf. true count of current pixel pointer
	uint8_t x_tile, y_tile; //for tile-by-tile read.
} Engine;

typedef struct { //structure useful for app-level logic with sprites.
				 //needs to be decoupled because of C++ lang barrier
				 
	int32_t  x,y; //coords of the top-left of sprite rect.
	uint8_t* map;
	uint8_t tiles_wide, tiles_high;
	uint8_t id; //id sets apart/secondary handle. Struct itself is the primary handle
	 
	struct Sprite* s;
} Entity_Handle;

Engine* engine_init(struct Layer* layer);
uint16_t* engine_render(Engine* e); //needs to return a contiguous arr

struct Layer* add_layer(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high); 
int update_layer(struct Layer* layer, uint8_t* map, uint8_t x, uint8_t y);

Entity_Handle* add_sprite(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high, struct Layer* associated_layer);
void update_sprite_map(Entity_Handle* sprite, uint8_t* map);
void update_sprite_position(Entity_Handle* sprite, uint16_t x, uint16_t y); 
void increment_sprite_x(Entity_Handle* sprite); //easy-increment
void increment_sprite_y(Entity_Handle* sprite); //easy-increment

int delete_layer(struct Layer* layer); //TODO weird system behaviours..
int delete_sprite(struct Sprite* sprite); //TODO essential and needs tons of testing

#ifdef __cplusplus
} //extern "C"
#endif 

#endif //ENGINE_API_H
