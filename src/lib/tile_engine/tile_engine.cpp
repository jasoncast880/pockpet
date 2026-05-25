#include "tile_engine.hpp"

Tile::Tile() {}
Tile::Tile(uint16_t* src)
	: pixels(std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)) {
	for (int i = 0 ; i < DEFAULT_TILE_LEN*DEFAULT_TILE_LEN ; i++) {
		pixels[i] = src[i];
	}
}
Tile::Tile(const Tile& copy) noexcept //not really used.
	: pixels(std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)) {
	uint16_t* src = this->pixels.get();
	uint16_t* dst = copy.pixels.get();
	if(copy.pixels!=nullptr) {
		for( int i = 0 ; i<(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN); i++ ) {
			*src = *dst;
			src++;
			dst++;
		}
	}	
}
Tile& Tile::operator=(const Tile& copy) noexcept {
	uint16_t* src = this->pixels.get();
	uint16_t* dst = copy.pixels.get();
	if(this != &copy && copy.pixels!=nullptr) {
		for( int i = 0 ; i<(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN); i++ ) {
			*src = *dst;
			src++;
			dst++;
		}
	}

	return *this;
}
void Tile::set_pixel(uint16_t idx, uint16_t val) {
	*(this->get_buffer()+idx) = val;
}
uint16_t Tile::get_pixel(uint16_t idx) {
	uint16_t* pix = this->get_buffer()+idx;
	return *pix;
}
uint16_t* Tile::get_buffer() {
  return pixels.get();
}
Tile::~Tile(){
	this->pixels.release();
}

Tileset::Tileset( uint16_t* buf, size_t size ) : buf(buf), num_tiles(size/(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)){
	tiles = new Tile[num_tiles];
	for(int i = 0; i<num_tiles ; i++) {
		tiles[i] = Tile(buf);
		buf+=DEFAULT_TILE_LEN*DEFAULT_TILE_LEN;
	}
}
Tile* Tileset::get_tile(uint8_t idx) { return &tiles[idx]; }
size_t Tileset::get_num_tiles(uint8_t idx) { return num_tiles; }
Tileset::~Tileset() { delete tiles; }

Tilemap::Tilemap(){}
void Tilemap::set_position(uint16_t x, uint16_t y) { this->x0 = x; this->y0 = y;}
Tile* Tilemap::get_tile(uint16_t idx) { return tileset->get_tile(map[idx]); }
void Tilemap::set_map(uint8_t* map) { this->map = map; }

Layer::Layer( uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* map, uint8_t id) {
	framebuf_data = new uint16_t[ (tiles_wide*tiles_high) * (DEFAULT_TILE_LEN*DEFAULT_TILE_LEN) ];

	this->tiles_wide = tiles_wide;
	this->tiles_high = tiles_high;
	this->tileset = tileset;
	this->map = map;
	this->id =id;
}

uint8_t Layer::sprite_add(uint8_t tiles_wide, uint8_t tiles_high, Tileset* ts, uint8_t* map) {
	if(sprites.size() <= MAX_SPRITES_PER_LAYER) {
		Sprite sprite(tiles_wide, tiles_high, ts, map, this);
		sprites.push_back(sprite);
		sprite.id = sprites.size(); //id is size of vector.
		return sprite.id;
	} else return 0; //0 id means no space on the layer.
}

void Layer::sprite_update_by_id(uint8_t id, uint16_t x, uint16_t y, uint8_t* map) {
	Sprite sprite = sprites.at(id);
	sprite.set_position(x,y);
	sprite.set_map(map);
}
void Layer::sprite_delete_by_id(uint8_t id) {} //no delete by index on vec

void Layer::render() {
	for( int i = 0 ; i < sprites.size() ; i++ ) {
		sprites[i].render();
	}

	//TEMP ?
	//use the dirty tile buffer to superimpose a buffer.
	int i = 0;
	int j = 0;
	for( ; i < this->tiles_high ; i++ ) {
		for( ; j < this->tiles_wide ; j++ ) {
			Tile* tile = this->tileset->get_tile(i*tiles_wide + j);
			uint16_t* buf = tile->get_buffer();

			for( int k = 0 ; k < (DEFAULT_TILE_LEN * DEFAULT_TILE_LEN) ; k++ ) {
				uint16_t* p = (framebuf_data+(i*tiles_wide+j)*(DEFAULT_TILE_LEN * DEFAULT_TILE_LEN)) + k;
				*p = *buf;
				buf++;
			}
		}
	}

}
void Layer::clear() {
	dirty_tiles.clear();
	//TODO:do clean tiles on where the dirt tiles were.
}

void Sprite::render() { //todo: for now this assumes that its blitting on layer-0
	for(int i = 0; i<tiles_high; i++) {
		for(int j = 0; j<tiles_wide; j++) {
			this->associated_layer->dirty_tiles_add( 
				blit_tile(
					i*tiles_wide + j,
					x0+j*DEFAULT_TILE_LEN,
					y0+i*DEFAULT_TILE_LEN));
		}
	}
}

void Layer::dirty_tiles_add( DirtyTile* tile) {	dirty_tiles.push_back(*tile); }

tile_context_t Layer::contextualize(uint16_t x, uint16_t y) {
	tile_context_t dummy;
	return dummy;
} //todo

tile_context_t Sprite::contextualize(uint16_t x, uint16_t y) {
	/*
	 * returns the tile, tile index in reference to the current layer.
	 * Sprite position always is in reference to the layer it resides in.
	 */
	tile_context_t tc; uint16_t x_tile_ = x/DEFAULT_TILE_LEN; uint16_t y_tile_ = y/DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_ + this->associated_layer->tiles_wide*y_tile_;

	uint16_t x_tile_offset = x % DEFAULT_TILE_LEN;
	uint16_t y_tile_offset = y % DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_offset + DEFAULT_TILE_LEN*y_tile_offset;

	return tc;
}

DirtyTile* Sprite::blit_tile(uint16_t idx, uint16_t x, uint16_t y) { //consider caching optimizations.
	uint16_t buf[DEFAULT_TILE_LEN*DEFAULT_TILE_LEN];
	for(int x = 0; x<DEFAULT_TILE_LEN; x++) {
		for(int y = 0; y<DEFAULT_TILE_LEN; y++) {
			tile_context_t tc = this->contextualize((uint16_t) x, (uint16_t) y);
			uint16_t pixel;
			if( pixel == static_cast<uint16_t>(ALPHA_FILTER) ){
				pixel = associated_layer->get_tile(tc.map_idx)->get_pixel(tc.tile_idx);
			} else {
				pixel = this->tileset->get_tile(idx)->get_pixel(x+y*DEFAULT_TILE_LEN);
			}
			buf[x+y*DEFAULT_TILE_LEN] = pixel;
		}
	}
	DirtyTile* tile = new DirtyTile(&buf[0], x, y ); 
	return tile;
}



Layer::~Layer() {
	//kill the vector and the sprites on the layer
	/*
	for( int i = 0; i < sprites.size() ; i++ ) {
		sprite_delete_by_id(i);
	}
	*/
	//dirty tiles should auto-delete its elements??
}

Sprite::~Sprite() {
}

#include "engine_api.h"
struct LayerHandle_t* add_layer(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high) {
	LayerHandle_t* handle = new LayerHandle_t( tiles, num_tiles, tilemap, tiles_wide, tiles_high);
	return handle;
}
struct SpriteHandle_t* add_sprite(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high, LayerHandle_t associated_layer) {
	SpriteHandle_t* handle = new SpriteHandle_t( tiles, num_tiles, tilemap, tiles_wide, tiles_high, &associated_layer );
	return handle;
}

int update_layer(LayerHandle_t* layer_handle, uint8_t* map, uint8_t x, uint8_t y) {
	layer_handle->layer->set_position(x,y);
	layer_handle->layer->set_map(map);

	return 0;
}
int update_sprite(LayerHandle_t* layer_handle, uint8_t sprite_id, uint8_t* map, uint8_t x, uint8_t y) {
	layer_handle->layer->sprite_update_by_id(sprite_id, x, y, map);
	return 0;
}

uint16_t* get_framebuf_data( struct LayerHandle_t* layer_handle ) {
	return layer_handle->layer->framebuf_data;
}

void soft_render() {
}


struct RenderInfo_t* engine_render( struct LayerHandle_t* layer_handle ) {
	//do shi
	//return new RenderInfo_t();
	return nullptr;
}

Tilemap::~Tilemap() {}
DirtyTile::~DirtyTile() {}

DirtyTile::DirtyTile(uint16_t* src,uint16_t x, uint16_t y) : x(x), y(y){
	pixels = (std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)) ;
	for (int i = 0 ; i < DEFAULT_TILE_LEN*DEFAULT_TILE_LEN ; i++) {
		pixels[i] = src[i];
	}
}
Sprite::Sprite(uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf, Layer* associated_layer) {
	this->tiles_wide = tiles_wide;
	this->tiles_high = tiles_wide;
	this->tileset = tileset;
	this->map = mapBuf;
	this->associated_layer = associated_layer;
}

Engine::Engine() {
#if   DIRTY_RENDER
	//vector alloc, too lazy to write 
#elif FULSCREEN_RENDER
	pix_buf = new uint16_t[DEFAULT_TILE_LEN * DEFAULT_TILE_LEN * DEFAULT_SCREEN_TILES_X * DEFAULT_SCREEN_TILES_Y];
#elif HSCANLINE_RENDER
	pix_buf = new uint16_t[DEFAULT_SCREEN_TILES_X*DEFAULT_TILE_LEN*HSCANLINE_SIZE];
#endif
}

uint16_t* Engine::hscanline_render() {
	for(int i = 0 ; i<(DEFAULT_SCREEN_TILES_Y*DEFAULT_TILE_LEN)/HSCANLINE_SIZE ; i++ ) {
		//gather the tilemap layer data
		for( int j = 0 ; j < HSCANLINE_SIZE*DEFAULT_SCREEN_TILES_X*DEFAULT_TILE_LEN ; j++) {
			if(this->x == (DEFAULT_SCREEN_TILES_X*DEFAULT_TILE_LEN) - 1) {
				this->x = 0;
				this->y++;
			}

			tile_context_t context = this->layer->contextualize(this->x, this->y);
			Tile* tile = layer->tileset->get_tile(context.map_idx);
			uint16_t pix = tile->get_pixel(context.tile_idx);
			
			pix_buf[j] = pix;
		}
		for( int j = 0 ; j < this->layer->sprites.size() ; j++ ) {
			//TODO: store indices of occupation on render call or something


			
		}

		//iff sprites:
		//add on the sprites' data based on their position (x,y,z) and/or blit.
	
	}
	
	return &pix_buf[0];
}

