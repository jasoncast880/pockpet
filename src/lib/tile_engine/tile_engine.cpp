#include "tile_engine.h"

Tile::Tile(uint16_t* src)
	: tile_len(tile_len), pixels(std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)) {
	for (int i = 0 ; i < DEFAULT_TILE_LEN*DEFAULT_TILE_LEN ; i++) {
		pixels[i] = src[i];
	}
}

Tile::Tile(const Tile& other) noexcept
	: pixels(std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)) {
	std::copy(other.get_buffer(), other.getBuf()+(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN), this->pixels.get());
}

Tile& Tile::operator=(const Tile& cpySrc) noexcept {
	if(this != &cpySrc) {
		pixels = (std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN));
		std::copy(other.get_buffer(), other.getBuf()+(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN), this->pixels.get());
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
	this->pixels = nullptr;
}

Tileset::Tileset( uint16_t* buf, size_t size ) : buf(buf), num_tiles(size/(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)){
	tiles = new Tile[num_tiles];
	for(int i = 0; i<num_tiles ; i++) {
			tiles[i] = Tile(buf);
			buf+=DEFAULT_TILE_LEN*DEFAULT_TILE_LEN;
		}
	}
}
Tile* Tileset::get_tile(uint8_t idx) { return this->tiles[idx]; }
size Tileset::get_num_tiles(uint8_t idx) { return this->tiles[idx]; }
Tileset::~Tileset() { delete tiles; }

Tilemap::Tilemap(){}
void Tilemap::set_position(uint816_t x, uint816_t y) { this.x = x; this.y = y;}
Tile* Tilemap::get_tile(uint16_t idx) { return tileset->get_tile(map[idx]); }
void Tilemap::set_map(uint8_t* map) { this->map = map; }

Layer::Layer(const uint8_t tiles_wide,const uint8_t tiles_high,const Tileset& tileset, uint8_t* map, uint8_t id) 
	:tiles_wide(tiles_wide), tiles_high(tiles_high), tileset(tileset), map(map), id(id) {
}

uint8_t Layer::sprite_add(Sprite* sprite) {
	sprites.push_back(sprite);

	sprite->id = (sprites.size-1); 
	return sprite->id;
}

void Layer::sprite_update_by_id(uint8_t id, uint16_t x, uint16_t y, uint8_t* map) {
	Sprite* sprite = sprites.at(id);
	sprite->set_position(x,y)
	sprite->set_map(map);
}

void Layer::sprite_delete_by_id(uint8_t id) {
	sprites.erase(id);
}

void Layer::render() override {
	for( int i = 0 ; i < num_sprites ; i++ ) {
		sprites[i].render();
	}
}
void Layer::clear() {
	dirty_tiles.clear();
	//TODO:do clean tiles on where the dirt tiles were.
}

void Sprite::render() override{ //todo: for now this assumes that its blitting on layer-0
	for(int i = 0; i<tiles_high; i++) {
		for(int j = 0; j<tiles_wide; j++) {
			this->associated_layer->dirty_tiles_add( blit_tile(i*tiles_wide + j),
				x+j*DEFAULT_TILE_LEN,
				y+i*DEFAULT_TILE_LEN );
		}
	}
}

void Layer::dirty_tiles_add(Tile* tile) {
	dirty_tiles.push_back(tile);
}

tile_context_t Layer::contextualize(uint16_t x, uint16_t y) override {} //todo

tile_context_t Sprite::contextualize(uint16_t x, uint16_t y) override {
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

Tile* Sprite::blit_tile(uint16_t idx) { //consider caching optimizations.
	uint16_t buf[DEFAULT_TILE_LEN*DEFAULT_TILE_LEN];
	for(int x = 0; x<DEFAULT_TILE_LEN; x++) {
		for(int y = 0; y<DEFAULT_TILE_LEN; y++) {
			tile_context_t tc = this->contextualize((uint16_t) x, (uint16_t) y);
			uint16_t pixel = this->tileset->get_tile(idx)->get_pixel(x+y*DEFAULT_TILE_LEN);
			if(pixel == ALPHA_FILTER){
				tile_context_t tc = this->contextualize((uint16_t) x, (uint16_t) y);
				buf[x+y*DEFAULT_TILE_LEN] = associated_layer->get_tile(tc.map_idx)->get_pixel(tc.tile_idx);
			} else {
				buf[x+y*DEFAULT_TILE_LEN] = pixel;
			}
		}
	}
	Tile blit_tile = new Tile(&buf[0], x, y ); //!!!!!TODO!!!!!
																						 //fix tile_context positional reference-chain
	return &blit_tile;
}
