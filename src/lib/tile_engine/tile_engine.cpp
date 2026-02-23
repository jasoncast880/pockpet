#include "tile_engine.h"

Tile::Tile() {}

Tile::Tile(uint16_t* src)
	: pixels(std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)) {
	for (int i = 0 ; i < DEFAULT_TILE_LEN*DEFAULT_TILE_LEN ; i++) {
		pixels[i] = src[i];
	}
}

DirtyTile::DirtyTile(uint16_t* src, uint16_t x, uint16_t y ) //FOR DIRT TILE
	:  x(x) , y(y){

	pixels = (std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN));

	for (int i = 0 ; i < DEFAULT_TILE_LEN*DEFAULT_TILE_LEN ; i++) {
		pixels[i] = src[i];
	}
	display_params[0]= x>>8;
	display_params[1]= x&0xff;
	display_params[2]= (x+15)>>8;
	display_params[3]= (x+15)&0xff;
	display_params[4] = y>>8;
	display_params[5] = y&0xff;
	display_params[6] = (y+15)>>8;
	display_params[7] = (y+15)&0xff;

}

DirtyTile::DirtyTile(const DirtyTile& copy) noexcept {
	pixels = (std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN));
	x = copy.x;
	y = copy.y;

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

DirtyTile& DirtyTile::operator=(const DirtyTile& copy) noexcept {
	pixels = (std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN));
	x = copy.x;
	y = copy.y;

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
	uint16_t* pix = this->get_buffer();
	uint16_t val = *(pix+idx);
  return val;
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
	this->tiles_wide = tiles_wide;
	this->tiles_high = tiles_high;
	this->tileset = tileset;
	this->map = map;
	this->id =id;
}

uint8_t Layer::sprite_add(uint8_t tiles_wide, uint8_t tiles_high, Tileset* ts, uint8_t* map) {
	Sprite sprite;
	sprites.push_back( Sprite(tiles_wide, tiles_high, ts, map, this) );
    //for now id is not useful
	//uint8_t* id = sprite.get_id(); 
	//*id = (sprites.size()-1); 
	return 0;//*id;
}

void Layer::sprite_update_by_id(uint8_t id, uint16_t x, uint16_t y, uint8_t* map) {
	Sprite* sprite = &sprites.at(id);
	sprite->set_position(x,y);
	sprite->set_map(map);
}

//TODO: delete_by_id 

void Layer::render() {
	for( int i = 0 ; i < sprites.size() ; i++ ) {
		sprites[i].render();
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

void Layer::dirty_tiles_add(DirtyTile* tile) {	dirty_tiles.push_back(*tile); }

Layer::tile_context_t Layer::contextualize(uint16_t x, uint16_t y) {
	tile_context_t dummy;
	return dummy;
} //todo

Layer::tile_context_t Sprite::contextualize(uint16_t x, uint16_t y) {
	/*
	 * returns the tile, tile index in reference to the current layer.
	 * Sprite position always is in reference to the layer it resides in.
	 */
	tile_context_t tc; uint16_t x_tile_ = x/DEFAULT_TILE_LEN; uint16_t y_tile_ = y/DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_ + this->associated_layer->tiles_wide*y_tile_;

	uint16_t x_tile_offset = x % DEFAULT_TILE_LEN;
	uint16_t y_tile_offset = y % DEFAULT_TILE_LEN;
	tc.tile_idx = x_tile_offset + DEFAULT_TILE_LEN*y_tile_offset;

	return tc;
}

DirtyTile* Sprite::blit_tile(uint16_t idx, uint16_t x, uint16_t y) { //consider caching optimizations.
	uint16_t buf[DEFAULT_TILE_LEN*DEFAULT_TILE_LEN];

    for(int i = 0 ; i < DEFAULT_TILE_LEN*DEFAULT_TILE_LEN; i++) {
			Tile* ref_tile = this->tileset->get_tile(idx);
			uint16_t ref_pix = ref_tile->get_pixel(i);
			buf[i] = ref_pix; //TODO: condense to reduce cycles after testing.
    }

	for(int col = 0; col<DEFAULT_TILE_LEN; col++) {
		for(int row = 0; row<DEFAULT_TILE_LEN; row++) {
			tile_context_t tc;
				uint16_t pixel = buf[col+row*DEFAULT_TILE_LEN];
				if( pixel == static_cast<uint16_t>(ALPHA_FILTER) ){
					tc = this->contextualize((uint16_t) x+col, (uint16_t) y+row);
					buf[col+row*DEFAULT_TILE_LEN] = associated_layer->get_tile(tc.map_idx)->get_pixel(tc.tile_idx);
			}
    }
	}
	DirtyTile* tile = new DirtyTile(&buf[0], (uint16_t)x, (uint16_t)y ); 
	return tile;
}

uint8_t* Sprite::get_id() {
	return &id;
}

Tilemap::~Tilemap() {
	
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

Sprite::Sprite() {}

Sprite::Sprite(const Sprite& copy) {
	map = copy.map;
	x0 = copy.x0;
	y0 = copy.y0;
	tileset = copy.tileset;
	tiles_wide = copy.tiles_wide;
	tiles_high = copy.tiles_high;
}

Sprite& Sprite::operator=(const Sprite& copy) {
	map = copy.map;
	x0 = copy.x0;
	y0 = copy.y0;
	tileset = copy.tileset;
	tiles_wide = copy.tiles_wide;
	tiles_high = copy.tiles_high;
	return *this;
}

Sprite::~Sprite() {
}

Sprite::Sprite(uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf, Layer* associated_layer){
	this->tileset = tileset;
	this->tiles_wide = tiles_wide;
	this->tiles_high = tiles_high;
	this->map = mapBuf;
	this->associated_layer = associated_layer;

	this->x0 = 0;
	this->y0 = 0;
}
