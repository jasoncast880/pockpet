#include "tile_engine.hpp"

Tile::Tile() {}
Tile::Tile(uint16_t* src) : buf(src) {
	buf = new uint16_t[DEFAULT_TILE_LEN*DEFAULT_TILE_LEN];
	for (int i = 0 ; i < DEFAULT_TILE_LEN*DEFAULT_TILE_LEN ; i++) {
		buf[i] = src[i]; //TRUE RAM TO FLASH facilitated by cpu
	}
}

uint16_t Tile::get_pixel(uint16_t idx) {
	uint16_t* pix = this->buf+idx;
	return *pix;
}

Tile::~Tile(){
	delete this->buf;
}

Tileset::Tileset( uint16_t* tileset_buf, size_t num_tiles ) : buf(tileset_buf), num_tiles(num_tiles) {

}

Tile Tileset::get_tile(uint8_t idx) { 
	uint16_t* p = this->buf + (idx * DEFAULT_TILE_LEN * DEFAULT_TILE_LEN );
	return Tile(p); 
} 

Tilemap::Tilemap(){}
void Tilemap::set_position(uint16_t x, uint16_t y) { this->x0 = x; this->y0 = y;}
Tile Tilemap::get_tile(uint16_t idx) { return tileset->get_tile(map[idx]); }
void Tilemap::set_map(uint8_t* map) { this->map = map; }

Layer::Layer( uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* map, uint8_t id) {
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
		//sprites.render populates 'dirty indices'
		//makes it so layer can go sprite by sprite in its scanline and do that shi
	}


#if DIRTY_RENDER
	int j = 0;
	for( ; i < this->tiles_high ; i++ ) { //what is this this crazy af
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
#elif FULLSCREEN_RENDER
#elif HSCANLINE_RENDER
#endif

}
void Layer::clear() {
	//TODO:do clean tiles on where the dirt tiles were.
}

void Sprite::render() { //todo: for now this assumes that its blitting on layer-0
	for(int i = 0; i<tiles_high; i++) {
		for(int j = 0; j<tiles_wide; j++) {
		}
	}
}


tile_context_t Layer::contextualize(uint16_t x, uint16_t y) {
	tile_context_t tc;

	uint16_t x_tile_ = x/DEFAULT_TILE_LEN;
	uint16_t y_tile_ = y/DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_ + this->tiles_wide * y_tile_;

	uint16_t x_tile_offset = x % DEFAULT_TILE_LEN;
	uint16_t y_tile_offset = y % DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_offset + DEFAULT_TILE_LEN * y_tile_offset;

	return tc;
} //todo

tile_context_t Sprite::contextualize(uint16_t x, uint16_t y) {
	/*
	 * returns the tile, tile index in reference to the current layer.
	 * Sprite position always is in reference to the layer it resides in.
	 */
	tile_context_t tc;
	uint16_t x_tile_ = x/DEFAULT_TILE_LEN;
	uint16_t y_tile_ = y/DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_ + this->associated_layer->tiles_wide*y_tile_;

	uint16_t x_tile_offset = x % DEFAULT_TILE_LEN;
	uint16_t y_tile_offset = y % DEFAULT_TILE_LEN;
	tc.map_idx = x_tile_offset + DEFAULT_TILE_LEN*y_tile_offset;

	return tc;
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
Layer* add_layer(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high) {

	Tileset* ts = new Tileset( tiles, (DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)*(30) ); 
	Layer* handle = new Layer( tiles_wide, tiles_high, ts, tilemap, 0 );
	return handle;
}

Sprite* add_sprite(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high, Layer* associated_layer) {
	Tileset* ts = new Tileset( tiles, (DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)*tiles_wide*tiles_high);

	Sprite* s = new Sprite( tiles_wide, tiles_high , ts, tilemap , associated_layer );
	return s;
}

int update_layer(Layer* layer, uint8_t* map, uint8_t x, uint8_t y) {
	layer->set_position(x,y);
	layer->set_map(map);

	return 0;
}
int update_sprite(Layer* layer, uint8_t sprite_id, uint8_t* map, uint8_t x, uint8_t y) {
	layer->sprite_update_by_id(sprite_id, x, y, map);
	return 0;
}

uint16_t* get_framebuf_data( struct Layer* layer ) {
	return layer->framebuf_data;
}

Tilemap::~Tilemap() {}

Sprite::Sprite(uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf, Layer* associated_layer) {
	this->tiles_wide = tiles_wide;
	this->tiles_high = tiles_wide;
	this->tileset = tileset;
	this->map = mapBuf;
	this->associated_layer = associated_layer;

	//this is the moment of Sprite creation, associated_layer->
	

}

//ENGINE's API CODE !!! LEAVE AT BOTTOM
Engine* engine_init(struct Layer* layer) {
	Engine* e = new Engine();
#if   DIRTY_RENDER
	//vector alloc, too lazy to write 
#elif FULSCREEN_RENDER
	e->render_buf_size = DEFAULT_TILE_LEN * DEFAULT_TILE_LEN * DEFAULT_SCREEN_TILES_X * DEFAULT_SCREEN_TILES_Y;
#elif HSCANLINE_RENDER
	e->render_buf_size = DEFAULT_TILE_LEN * DEFAULT_SCREEN_TILES_X * HSCANLINE_SIZE;
#endif

	e->render_data = new uint16_t[e->render_buf_size];
	e->layer = layer;
	return e;
}

uint16_t* engine_render(Engine* e) { 
	uint16_t* p = e->render_data;

#if HSCANLINE_RENDER
	//fill the p buffer with the default sprite tilemap pixels
	uint32_t y0 = e->y;

	for(int i = e->y ; i<y0+HSCANLINE_SIZE ; i++ ) { //TODO on incr. whats value of e->y, y0??
		for(int j = 0; j< DEFAULT_SCREEN_TILES_X*DEFAULT_TILE_LEN ; j++ ) {
			tile_context_t ctx = e->layer->contextualize(j,i);
			Tile tile = e->layer->tileset->get_tile(ctx.map_idx); 
			p[j] = tile.get_pixel(ctx.tile_idx); 
		}
	}

	//ascertain which sprites reside in the scanline region
	for( int j = 0 ; j < e->layer->sprites.size() ; j++ ) {
		Sprite* sprite = &e->layer->sprites.at(j); //TODO during testing, try with 1, then add more
		
		int diff_a = y0 - sprite->y0; //assumes below top of scanline
		int diff_b = sprite->y0+(DEFAULT_TILE_LEN*sprite->tiles_high) - y0+HSCANLINE_SIZE; //assumes above top of scanline

		if(diff_a>=0 && diff_a <= HSCANLINE_SIZE) {
			for(int l = diff_a ; l < (HSCANLINE_SIZE-diff_a) ; l++ ) {
				for(int m = 0 ; m < sprite->tiles_wide ; m++ ) {
					uint8_t map_index = sprite->tiles_wide*((sprite->y0-y0+l)/DEFAULT_TILE_LEN)+m;
					Tile tile = sprite->get_tile(map_index); 
					for(int n=0 ; n < DEFAULT_TILE_LEN; n++ ) {
						uint8_t tile_index = n + (l%DEFAULT_TILE_LEN)*DEFAULT_TILE_LEN;
						uint16_t pix = tile.get_pixel(tile_index);
						
						//now you need to tranlate this optimized value into the scanline's value
						if(pix!=ALPHA_FILTER){
							uint8_t x_temp = sprite->x0+m*DEFAULT_TILE_LEN+n;
							p[ l*(DEFAULT_TILE_LEN*DEFAULT_SCREEN_TILES_X)+x_temp ] = pix;
						} 
					}
				}
			}
		} else if( diff_a<=0 && diff_b < HSCANLINE_SIZE ) { //scanline bounds check
			uint16_t L_MAX = ( (y0+HSCANLINE_SIZE-(sprite->y0+DEFAULT_TILE_LEN*sprite->tiles_high))<HSCANLINE_SIZE );
			L_MAX = L_MAX < HSCANLINE_SIZE ? L_MAX : HSCANLINE_SIZE; //idk
			for(int l = y0 ; l < (L_MAX) ; l++) {
				for(int m = 0 ; m < sprite->tiles_wide ; m++ ) {
					uint8_t map_index = sprite->tiles_wide*((sprite->y0-y0+l)/DEFAULT_TILE_LEN)+m;
					Tile tile = sprite->get_tile(map_index); 
					for(int n = 0 ; n < DEFAULT_TILE_LEN ; n++ ) {
						uint8_t tile_index = n + (l%DEFAULT_TILE_LEN)*DEFAULT_TILE_LEN;
						uint16_t pix = tile.get_pixel(tile_index);
						
						//now you need to tranlate this optimized value into the scanline's value
						if(pix!=ALPHA_FILTER){
							uint8_t x_temp = sprite->x0+m*DEFAULT_TILE_LEN+n;
							p[ l*(DEFAULT_TILE_LEN*DEFAULT_SCREEN_TILES_X)+x_temp ] = pix;
						} 
					}
				}
			}
		}
	}
#endif //TODO: implement other forms of rendering as needed
	   
	return &p[0];
}

