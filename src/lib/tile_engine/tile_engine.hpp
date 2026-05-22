#ifndef TILE_ENGINE_H
#define TILE_ENGINE_H

#include <cstddef>
#include <stdint.h>

#include <queue>
#include <memory>
#include <vector>
#include <algorithm>

#define ALPHA_FILTER 0xF81F

//TODO: KEEP ALL THIS STUFF IN A CONFIG FILE SO EASY FOR ALL TO READ
#define DEFAULT_TILE_LEN 16
#define DEFAULT_SCREEN_TILES_Y 15
#define DEFAULT_SCREEN_TILES_X 20
#define MAX_LAYERS 2
#define MAX_SPRITES_PER_LAYER 5

struct Tile {
	Tile();
	Tile(uint16_t* src);

	Tile(const Tile& copySrc) noexcept;
	Tile& operator=(const Tile& copySrc) noexcept; 
	Tile(Tile&&) noexcept = default;
	Tile& operator=(Tile&& moveSrc) noexcept = default;
	~Tile();

	void set_pixel(uint16_t idx, uint16_t val);
	uint16_t get_pixel(uint16_t idx);
	uint16_t* get_buffer();
	
	std::unique_ptr<uint16_t[]> pixels;
};

struct DirtyTile : public Tile {
	DirtyTile(uint16_t* src,uint16_t x, uint16_t y);
	uint16_t x,y;
	uint8_t display_params[8];
	/*  DISP_PARAMS ORDER: (L.E)
	 *	CASET START 2 by
	 *	CASET END   2 by
	 *	RASET START 2 by
	 *	RASET END   2 by
	 */

	DirtyTile(const DirtyTile& copySrc) noexcept;
	DirtyTile& operator=(const DirtyTile& copySrc) noexcept; 
	DirtyTile(DirtyTile&&) noexcept = default;
	DirtyTile& operator=(DirtyTile&& moveSrc) noexcept = default;
	
	~DirtyTile();
};

struct Tileset{ 
	Tileset( uint16_t* buf, size_t size );

	Tile* get_tile(uint8_t idx); 
	size_t get_num_tiles(uint8_t idx); 

	~Tileset();

	size_t num_tiles;
private:
	Tile* tiles;
	uint16_t* buf;
	uint8_t tile_len = DEFAULT_TILE_LEN;
};

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
	Tile* get_tile(uint16_t idx); 
	void set_map(uint8_t* map); 

	typedef struct {
		uint8_t map_idx; //index of the map in current layer
		uint16_t tile_idx; //index of tile in the current map
	} tile_context_t; //gets returned given x,y

	virtual tile_context_t contextualize(uint16_t x, uint16_t y) = 0;

	virtual void render() = 0;

	~Tilemap();
}; 

class Sprite;
class Layer: public Tilemap{
public:

	//TEMP : temp allocate memory for a full frame buffer. (do in constructor)
	uint16_t* framebuf_data; //keep as 16-bit 565 pixel data, de-compress via masking after.
	
	std::vector<Sprite> sprites;

	void dirty_tiles_add( DirtyTile* tile);

	void render() override;
	Layer();
	Layer(uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* map, uint8_t id);
	
	uint8_t sprite_add(uint8_t tiles_wide, uint8_t tiles_high, Tileset* ts, uint8_t* map); 
	void sprite_update_by_id(uint8_t id, uint16_t x, uint16_t y, uint8_t* map);
	void sprite_delete_by_id(uint8_t id); 

	tile_context_t contextualize(uint16_t x, uint16_t y) override; 

	void clear();

	std::vector<DirtyTile> dirty_tiles; //tiles to throw at the hw

	~Layer();
};

//position enforced in context with associated_layer
class Sprite: public Tilemap { //touched by Layer only ; id is index within 'sprites' field
public:
	Layer* associated_layer = nullptr;

	Sprite();
	Sprite(uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf, Layer* associated_layer);

	Sprite(const Sprite& copy);
	Sprite& operator=(const Sprite& copy);

	Sprite(Sprite&&) noexcept = default;
	Sprite& operator=(Sprite&&) noexcept = default;

	tile_context_t contextualize(uint16_t x, uint16_t y) override; 
	static bool check_filter(uint16_t x, uint16_t y);

	DirtyTile* blit_tile(uint16_t idx, uint16_t x, uint16_t y); //TODO

	~Sprite();
	void render() override;

	friend Layer;
};

//API THINGS
static uint8_t layer_count = 0;
struct LayerHandle_t {
	Tileset* tiles;
	Layer* layer;
	uint8_t sprite_count = 0;

	uint8_t id;

	LayerHandle_t() {}
	LayerHandle_t(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high) {
		this->tiles = new Tileset( tiles, (DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)*(30) ); 
		this->layer = new Layer( tiles_wide, tiles_high, this->tiles, tilemap, 0); //replace magic nums todo
		
	}
};

struct SpriteHandle_t {
	Tileset* tiles;
	Sprite* sprite;
	
	LayerHandle_t layer_handle;
	uint8_t id;

	SpriteHandle_t();
	SpriteHandle_t( uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high, struct LayerHandle_t* associated_layer ) {
		this->tiles = new Tileset( tiles, (DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)*(30) ); 
		this->sprite =new Sprite( tiles_wide, tiles_high, this->tiles, tilemap, associated_layer->layer );
	}
};

#endif //TILE_ENGINE_H
