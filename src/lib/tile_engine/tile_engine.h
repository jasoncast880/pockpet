#include <cstddef>
#include <stdint.h>

#include <queue>
#include <memory>
#include <vector>

#define ALPHA_FILTER 0xF881F

#define DEFAULT_TILE_LEN 16

#define DEFAULT_SCREEN_TILES_Y 15
#define DEFAULT_SCREEN_TILES_X 20

typedef struct Tile {
	Tile(uint16_t* src);

	Tile(const Tile& copySrc) noexcept;
	Tile& operator=(const Tile& copySrc) noexcept; 
	Tile(Tile&&) noexcept = default;
	Tile& operator=(Tile&& moveSrc) noexcept = default;
	~Tile();

	void set_pixel(uint16_t idx, uint16_t val);
	uint16_t get_pixel(uint16_t idx, uint16_t val);
	uint16_t* get_buffer();
	
private:
	std::unique_ptr<uint16_t[]> pixels;
}; //Tile 

//structure(s) that encapsulate mem. cfg. pure memory creation.
struct Tileset{ 

	Tileset(uint8_t tile_len, uint16_t* buf, size_t size);
	//if i ever need to delete, move tilesets then i will implement those later.

	Tile* get_tile(uint8_t idx); //vector manipulations/reads.
	void set_tile(uint8_t idx, Tile* tile);

private:
	std::vector<Tile> tiles;
	uint8_t tile_len = DEFAULT_TILE_LEN;
	uint8_t num_tiles;
};

//strcture and sub structurees that encapsulate position, superposition, filtering.
struct Tilemap{ 
	uint8_t map[]; 
	uint16_t x, y; //top left corner of entity.
	Tileset* tileset;
	uint8_t tiles_wide, tiles_high;

	Tilemap()={};
	Tilemap(int x,
		int y,
		uint8_t tiles_wide,
		uint8_t tiles_high, 
		Tileset& ts,
		uint8_t* map
		);

	typedef struct {
		uint8_t map_idx; //index of the map in current layer
		uint16_t tile_idx; //index of tile in the current map
	} tile_context_t; //gets returned given x,y

	Tile* get_tile(uint16_t idx);
	void set_tile(uint16_t idx, Tile* tile);

}; 

//collisions 
// sprite-to-sprite
// sprite-to-boundary/threshhold.

class Layer: public Tilemap{
	//position, size enforced in context with id-0 Layer.
	uint8_t id; //enforce ID-0 as 320x240 base-screen..
							//ID acts as priority.
	
	std::vector<Tile> blitted_tiles;
	std::vector<uint16_t> blitted_indices;

	void sprite_add(Sprite sprit);
	void sprite_update( uint16_t x, uint16_t y, uint8_t* map);
	void sprite_delete_by_id(uint8_t id);

public:
	Layer(int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset& tileset, uint8_t* map);

	Layer(const Layer& other);
	Layer& operator=(const Layer& other);

	Layer(Layer&&) noexcept = default;
	Layer& operator=(Layer&&) noexcept = default;

	uint8_t get_id();
	void setID(uint8_t id);

	void set_position(uint16_t x, uint16_t y);
	void set_map(uint8_t* mapBuf); //change the entire map
	
	
	~Layer()=default;
};

class Sprite: public Tilemap { //position, dimensions in context with associated layer
	uint8_t id;
	Layer* associated_layer;

	Sprite(int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset& tileset, uint8_t* mapBuf);
	Sprite(const Sprite& other);
	Sprite& operator=(const Sprite& other);

	Sprite(Sprite&&) noexcept = default;
	Sprite& operator=(Sprite&&) noexcept = default;
	
	//positional context vars
	uint8_t x_offset = 0; //+ve offset from tile bounds
	uint8_t y_offset = 0; //+ve offset from tile bounds
												//top and left most bounds
	void setMapVector_Offsets(Tilemap& tilemap); 
	void resetMapVector_Offsets();

	context_t contextualize(uint16_t x, uint16_t y); //return indeces of 
};

class RenderController { //masher; expose to the system

public:
	void render(Layer layer); 

	RenderController();


};

#endif //TILE_ENGINE_H
