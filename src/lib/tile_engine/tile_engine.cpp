#include "tile_engine.hpp"

Tile::Tile() {}
Tile::Tile(const uint16_t* src) {
	buf = new uint16_t[DEFAULT_TILE_LEN * DEFAULT_TILE_LEN];
	for(int i = 0 ; i < DEFAULT_TILE_LEN * DEFAULT_TILE_LEN ; i++) {
		buf[i] = *src;
		src++;
	}
}
void Tile::get_pixel(uint16_t idx, uint16_t& val) { 
	val = *(buf+=idx); 
}
Tile::~Tile(){
	delete this->buf;
}

Tileset::Tileset( const uint16_t* buf, size_t num_tiles ) : buf(buf), num_tiles(num_tiles) {}
Tile Tileset::get_tile(uint8_t idx) {
	if( idx < this->num_tiles ) {
		const uint16_t* p = this->buf+=(idx*DEFAULT_TILE_LEN*DEFAULT_TILE_LEN); return Tile(p);
	} else return nullptr;
}
size_t Tileset::get_num_tiles(uint8_t idx) { return this->num_tiles; }

Tilemap::Tilemap(){}
Tile Tilemap::get_tile(uint16_t idx) { 
	return tileset->get_tile(map[idx]); 
}
void Tilemap::set_map(uint8_t* map) { this->map = map; }
Tilemap::~Tilemap(){}

Layer::Layer( uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, const uint8_t* map, uint8_t id) {
	this->tiles_wide = tiles_wide;
	this->tiles_high = tiles_high;
	this->tileset = tileset;
	this->map = map;

	this->sprites.reserve(SPRITE_RESERVE_SIZE);
}

uint8_t Layer::sprite_add(Sprite* s) {
	if(this->sprites.size() <= MAX_SPRITES_PER_LAYER) {
		this->sprites.push_back(*s);
		return (sprites.size()-1);
	} else return -1; 
}

void Layer::sprite_update_by_id(uint8_t id, int16_t x, int16_t y, uint8_t* map) {
	Sprite sprite = sprites.at(id);
	sprite.set_position(x,y);
	sprite.set_map(map);
}
void Layer::sprite_delete_by_id(uint8_t id) {} //no delete by index on vec

tile_context_t Layer::contextualize(int16_t x, int16_t y) {
	tile_context_t tc;

	int16_t x_tile_ = x/DEFAULT_TILE_LEN;
	int16_t y_tile_ = y/DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_ + this->tiles_wide*y_tile_;

	int16_t x_tile_offset = x % DEFAULT_TILE_LEN;
	int16_t y_tile_offset = y % DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_offset + DEFAULT_TILE_LEN*y_tile_offset;

	return tc;
} 

Sprite::Sprite(uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf, Layer* associated_layer) {
	this->tiles_wide = tiles_wide;
	this->tiles_high = tiles_wide;
	this->tileset = tileset;
	this->map = mapBuf;
	this->associated_layer = associated_layer;
}

tile_context_t Sprite::contextualize(int16_t x, int16_t y) {
	/*
	 * returns the tile, tile index in reference to the current layer.
	 * Sprite position always is in reference to the layer it resides in.
	 */
	tile_context_t tc; 
	int16_t x_tile_ = x/DEFAULT_TILE_LEN; 
	int16_t y_tile_ = y/DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_ + this->associated_layer->tiles_wide*y_tile_;

	int16_t x_tile_offset = x % DEFAULT_TILE_LEN;
	int16_t y_tile_offset = y % DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_offset + DEFAULT_TILE_LEN*y_tile_offset;

	return tc;
}

void Sprite::set_position(int16_t x, int16_t y) {
	this->x0 = x0;
	this->y0 = y0;
}

Layer::Layer() {}
Layer::~Layer() {
	//kill the vector and the sprites on the layer
	/*
	for() {
		sprite_delete_by_id(i);
	}
	*/
	//dirty tiles should auto-delete its elements??
}

Sprite::~Sprite() {
}

#include "engine_api.h"
Engine* e = nullptr;
Engine* engine_init(struct Layer* layer) { //todo : kill this bih
	e = new Engine(); //global static
#if   DIRTY_RENDER
	//vector alloc, too lazy to write 
#elif FULSCREEN_RENDER
	e->render_data = new uint16_t[DEFAULT_TILE_LEN * DEFAULT_TILE_LEN * DEFAULT_SCREEN_TILES_X * DEFAULT_SCREEN_TILES_Y];
#elif HSCANLINE_RENDER
	e->render_data = new uint16_t[DEFAULT_SCREEN_TILES_X*DEFAULT_TILE_LEN*HSCANLINE_SIZE];
#endif

	e->layer = layer;
	e->h_scanline_counter = 0;
	e->x = 0;
	e->y = 0;
	e->x_tile = 0;
	e->y_tile = 0;

	return e;
}

struct Layer* add_layer(const uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high) {

	Tileset* ts = new Tileset( tiles, (DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)*(30) ); 
	Layer* handle = new Layer( tiles_wide, tiles_high, ts, tilemap, 0 );
	return handle;
}
int update_layer(Layer* layer, uint8_t* map) {
	layer->set_map(map);

	return 0;
}

Entity_Handle* add_sprite(const uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high, Layer* associated_layer) {
	Tileset* ts = new Tileset( tiles, (DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)*tiles_wide*tiles_high);

	Entity_Handle* eh = new Entity_Handle();
	Sprite* s = new Sprite( tiles_wide, tiles_high , ts, tilemap , associated_layer );

	s->handle = eh;
	eh->id = associated_layer->sprite_add(s);

	return eh;
}

void update_sprite_position(Entity_Handle* eh, int16_t x, int16_t y) {
	//engine-side handling
	Sprite& sprite = e->layer->sprites.at(eh->id);
	sprite.set_position(x,y);
	
	//app's entity handling
	eh->x = x;
	eh->y = y;
}

void increment_sprite_x(Entity_Handle* eh) {
	//engine-side handling
	Sprite& sprite = e->layer->sprites.at(eh->id);
	sprite.x0++;
	
	//app's entity handling
	eh->x++;
}
void increment_sprite_y(Entity_Handle* eh) {
	//engine-side handling
	Sprite& sprite = e->layer->sprites.at(eh->id);
	sprite.y0++;
	
	//app's entity handling
	eh->y++;
}

void update_sprite_map(Entity_Handle* eh, uint8_t* map) {
	//engine-side handling
	Sprite& sprite = e->layer->sprites.at(eh->id);
	sprite.set_map(map);
	
	//app's entity handling
	eh->map = map;
}

void delete_sprite(Entity_Handle* eh) {
	int deleted_index = eh->id;
	e->layer->sprites.erase(e->layer->sprites.begin() + deleted_index);

	for(int i = deleted_index ; i < e->layer->sprites.size() ; i++) {
		//reduce their id's by 1
		//engine-side handling
		Sprite& sprite = e->layer->sprites.at(i);
		sprite.id--;

		//app entity handling
		sprite.handle--;
	}
}

#if HSCANLINE_RENDER
uint16_t* engine_render() { 

	uint16_t* p = e->render_data;

	for(int i = e->y*DEFAULT_SCREEN_TILES_X*DEFAULT_TILE_LEN ; i<i+(DEFAULT_SCREEN_TILES_Y*DEFAULT_TILE_LEN)/HSCANLINE_SIZE ; i++ ) {
		//gather the tilemap layer data

		for( int j = 0 ; j < HSCANLINE_SIZE*DEFAULT_SCREEN_TILES_X*DEFAULT_TILE_LEN ; j++) {
			if(e->x == (DEFAULT_SCREEN_TILES_X*DEFAULT_TILE_LEN) - 1) {
				e->x = 0;
				e->y++;
			}

			tile_context_t context = e->layer->contextualize(e->x, e->y);
			Tile tile = e->layer->tileset->get_tile(context.map_idx);
			uint16_t pix; 
			tile.get_pixel(context.tile_idx, pix);
			
			p[j] = pix;
		}
		for( int j = 0 ; j < e->layer->sprites.size() ; j++ ) {
			//TODO: store indices of occupation on render call or something
		}
	}
	
	return &p[0];
}
#endif //TODO: implement other forms of rendering as needed

