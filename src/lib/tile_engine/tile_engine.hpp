#ifndef TILE_ENGINE_H
#define TILE_ENGINE_H

//TODO What happens if the entities are hitting the edge/screen boundary?
//Without edge-case handling this will cause warping.

#include <cstddef>
#include <stdint.h>

#include <queue>
#include <memory>
#include <vector>
#include <algorithm>

#define ALPHA_FILTER 0xF81F
#include "graphics_conf.h" 
#include "engine_api.h"

typedef struct {
	uint8_t map_idx; //index of the map in current layer
	uint16_t tile_idx; //index of tile in the current map
} tile_context_t; //gets returned given x,y . Helper struct for tilemap blitting

//
struct Tile {
	Tile();
	Tile(const uint16_t* src);
	void get_pixel(uint16_t idx, uint16_t& val);
	~Tile(); //essentially this is now just a tileset-friendly unique ptr

	uint16_t* buf;
};

struct Tileset{ 
	Tileset( const uint16_t* tileset_buf, size_t num_tiles ); 

	Tile get_tile(uint8_t idx); 
	size_t get_num_tiles(uint8_t idx); 

private:
	const uint16_t* buf;
	size_t num_tiles; // errhandle/bounds-check
};
//

class Tilemap{ 
public:
	uint8_t* map; 
	uint16_t x_offset, y_offset; // w. reference to container
	uint16_t x0, y0; //top left corner of entity.
	Tileset* tileset;
	uint8_t tiles_wide, tiles_high;

	uint8_t id; 

	Tilemap();

	void set_position(uint16_t x, uint16_t y);

	//simple shared accessor/modifiers
	Tile get_tile(uint16_t idx); 
	void set_map(uint8_t* map); 

	virtual tile_context_t contextualize(uint16_t x, uint16_t y) = 0;

	virtual ~Tilemap();
}; 

class Sprite;
class Layer: public Tilemap {
public:

	//TEMP : temp allocate memory for a full frame buffer. (do in constructor)
	uint16_t* framebuf_data; //keep as 16-bit 565 pixel data, de-compress via masking after.
	
	std::vector<Sprite> sprites;


	Layer();
	Layer(uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* map, uint8_t id);
	
	uint8_t sprite_add(uint8_t tiles_wide, uint8_t tiles_high, Tileset* ts, uint8_t* map); 
	void sprite_update_by_id(uint8_t id, uint16_t x, uint16_t y, uint8_t* map);
	void sprite_delete_by_id(uint8_t id); 

	tile_context_t contextualize(uint16_t x, uint16_t y) override; 

	void clear();

	~Layer();
};

//position enforced in context with associated_layer
class Sprite: public Tilemap { //touched by Layer only ; id is index within 'sprites' field
public:
	Layer* associated_layer = nullptr;

	Sprite();
	Sprite(uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf, Layer* associated_layer);

	Sprite(const Sprite& copy) = default;
	Sprite& operator=(const Sprite& copy) = default;

	Sprite(Sprite&&) noexcept = default;
	Sprite& operator=(Sprite&&) noexcept = default;

	tile_context_t contextualize(uint16_t x, uint16_t y) override; 
	static bool check_filter(uint16_t x, uint16_t y);

	~Sprite();

	friend Layer;
};

#endif //TILE_ENGINE_H
