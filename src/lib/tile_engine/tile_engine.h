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

typedef struct {
	uint8_t* buf;
	uint16_t x,y;
} tile_item_t;

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
	uint16_t x, y; //top left corner of entity.
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

	virtual ~Tilemap() = 0;
}; 

//position, size enforced in context with layer id-0 (fullscreen)
class Layer: public Tilemap{
	uint8_t id; //enforce ID-0 as 320x240 base-screen..
							//ID acts as priority.	
	std::vector<tile_item_t> dirty_tiles; //tiles to throw at the hw
																				//NOTE: STORED SEQUENTIALLY
	tile_item_t* render();

	void set_tile(uint16_t idx, Tile* tile);

	void sprite_add(Sprite sprit);
	void sprite_update( uint16_t x, uint16_t y, uint8_t* map);
	void sprite_delete_by_id(uint8_t id);

public:
	Layer();
	Layer(const uint8_t tiles_wide,const uint8_t tiles_high,const Tileset& tileset, uint8_t* map, uint8_t id);
	uint8_t get_id();
	
	~Layer() override;
};

//position enforced in context with associated_layer
class Sprite: public Tilemap { 
	uint8_t id;
	Layer* associated_layer;

	Sprite(uint8_t tiles_wide, uint8_t tiles_high, Tileset& tileset, uint8_t* mapBuf);
	Sprite(const Sprite& other);
	Sprite& operator=(const Sprite& other);

	Sprite(Sprite&&) noexcept = default;
	Sprite& operator=(Sprite&&) noexcept = default;

	tile_context_t contextualize(uint16_t x, uint16_t y) override; 
	bool check_filter(uint16_t x, uint16_t y);

	void blit();
};


class RenderController { //masher; expose to the system
	
	//users: 
	//1) hardware wrapper task, uses tile_items for its cb. structure.
	//2) main code, acts as input via Layer.add, sprite_add, etc.

	Layer base_layer;
	std::vector<tile_item_t> tile_items; //expose.
	void add_item(tile_item_t);
	void clear_items(); //call after a hardware read.
	
public:
	void render(Layer layer); 

	RenderController();
};

#endif //TILE_ENGINE_H
