#ifndef TILE_ENGINE_H
#define TILE_ENGINE_H

#include <cstddef>
#include <stdint.h>

#include <queue>
#include <memory>
#include <vector>
#include <algorithm>

#define ALPHA_FILTER 0xF881F

#define DEFAULT_TILE_LEN 16

#define DEFAULT_SCREEN_TILES_Y 15
#define DEFAULT_SCREEN_TILES_X 20

struct Tile {
	Tile();
	Tile(uint16_t* src);
	Tile(uint16_t* src,uint16_t x, uint16_t y);

	Tile(const Tile& copySrc) noexcept;
	Tile& operator=(const Tile& copySrc) noexcept; 
	Tile(Tile&&) noexcept = default;
	Tile& operator=(Tile&& moveSrc) noexcept = default;
	~Tile();

	void set_pixel(uint16_t idx, uint16_t val);
	uint16_t get_pixel(uint16_t idx);
	uint16_t* get_buffer();
	
	//FOR DIRTY TILES ONLY:
	uint16_t x,y;
private:
	std::unique_ptr<uint16_t[]> pixels;

};

struct Tileset{ 
	Tileset( uint16_t* buf, size_t size );

	Tile* get_tile(uint8_t idx); 
	size_t get_num_tiles(uint8_t idx); 

	~Tileset();

private:
	Tile* tiles;
	uint16_t* buf;
	uint8_t tile_len = DEFAULT_TILE_LEN;
	size_t num_tiles;
};

class Tilemap{ 
public:
	uint8_t* map; 
	uint16_t x0, y0; //top left corner of entity.
	Tileset* tileset;
	uint8_t tiles_wide, tiles_high;

	static uint16_t x_offset, y_offset; // w. reference to container

	Tilemap();

	void set_position(uint16_t x, uint16_t y);

	//simple shared accessor/modifiers
	Tile* get_tile(uint16_t idx); 
	void set_map(uint8_t* map); 

	typedef struct {
		uint8_t map_idx; //index of the map in current layer
		uint16_t tile_idx; //index of tile in the current map
	} tile_context_t; //gets returned given x,y

	virtual tile_context_t contextualize(uint16_t x, uint16_t y) = 0;

	virtual void render() = 0;

	virtual ~Tilemap() = 0;
}; 

class Sprite;
class Layer: public Tilemap{
	uint8_t id; //TODO enforce ID-0 as 320x240 base-screen..

	std::vector<Sprite> sprites;

	void dirty_tiles_add( Tile* tile);
	void render() override;

public:
	Layer();
	Layer(uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* map, uint8_t id);
	
	uint8_t sprite_add(Sprite* sprite); //return id
	void sprite_update_by_id(uint8_t id, uint16_t x, uint16_t y, uint8_t* map);
	//void sprite_delete_by_id(uint8_t id); //TODO

	tile_context_t contextualize(uint16_t x, uint16_t y) override; 

	void clear();

	std::vector<Tile> dirty_tiles; //tiles to throw at the hw

	~Layer() override;

	friend Sprite;
};

//position enforced in context with associated_layer
class Sprite: public Tilemap { //touched by Layer only
private:
public:
	uint8_t id = 0;//set by the Layer.

	Layer* associated_layer;

	Sprite(uint8_t tiles_wide, uint8_t tiles_high, Tileset& tileset, uint8_t* mapBuf);
	Sprite(const Sprite& other);
	Sprite& operator=(const Sprite& other);

	Sprite(Sprite&&) noexcept = default;
	Sprite& operator=(Sprite&&) noexcept = default;

	tile_context_t contextualize(uint16_t x, uint16_t y) override; 
	static bool check_filter(uint16_t x, uint16_t y);

	Tile* blit_tile(uint16_t idx, uint16_t x, uint16_t y); //TODO

	~Sprite() override;
	uint8_t* get_id();
	void render() override;
};

#endif //TILE_ENGINE_H
