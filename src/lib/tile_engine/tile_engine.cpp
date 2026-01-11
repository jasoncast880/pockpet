#include "include/tile_engine.h"

Tile::Tile()  //seems like a waste to call this; quite mem intensive
	: tile_len(16), buf(std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)) {}
Tile::Tile(int tile_len, uint16_t* srcBuf)
	: tile_len(tile_len), buf(std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)) {
	for (int i = 0 ; i < tile_len*tile_len ; i++) {
		buf[i] = srcBuf[i];
	}
}

Tile::Tile(const Tile& copySrc) noexcept
	: tile_len(copySrc.tile_len), buf(std::make_unique<uint16_t[]>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN)) {
	std::copy(copySrc.getBuf(), copySrc.getBuf()+(tile_len*tile_len), this->buf.get());
}
Tile& Tile::operator=(const Tile& cpySrc) noexcept {
	if(this != &cpySrc) {
		tile_len = cpySrc.tile_len;
		buf = (std::make_unique<uint16_t[]>(tile_len*tile_len));
		std::copy(cpySrc.getBuf(), cpySrc.getBuf()+(tile_len*tile_len), this->buf.get());
	}
	return *this;
}

void Tile::changePixel(uint16_t index, uint16_t value) {
	buf[index] = value;
}

uint16_t Tile::getPixel(uint16_t x,uint16_t y) const { //maybe not needed
  int index = (y*tile_len)+x;
  return buf[index];
}

uint16_t* Tile::getBuf() const {
  return buf.get();
}

Tileset::Tileset() {
}
Tileset::Tileset(Tile* tiles, uint8_t numTiles) //deep copy tiles..
	: numTiles(numTiles), tiles(std::make_unique<Tile[]>(numTiles)) {
		for (int i = 0 ; i < numTiles ; i++) {
			this->tiles[i] = tiles[i];
		}
	}
Tileset::Tileset(uint8_t tile_len, uint16_t* bufPtr, uint8_t numTiles) 
	: numTiles(numTiles), tiles(std::make_unique<Tile[]>(numTiles)) {
		uint16_t* tileBuf = new uint16_t [tile_len*tile_len];
		for(int i = 0 ; i < numTiles ; i++) {
			for(int j = 0 ; j < (tile_len*tile_len) ; j++){
				tileBuf[j] = *bufPtr;
				bufPtr++;
			}
			Tile* _tileArr = tiles.get();
			_tileArr[i] = Tile(tile_len, &tileBuf[0]); 
		}
		delete[] tileBuf;
	}

Tileset::Tileset(const Tileset& copySrc) noexcept{
	numTiles = copySrc.numTiles;
	std::copy(copySrc.getTiles(), copySrc.getTiles()+(numTiles), getTiles());
}
Tileset& Tileset::operator=(const Tileset& copySrc) noexcept {
	if(this != &copySrc) {
		numTiles = copySrc.numTiles;
		std::copy(copySrc.getTiles(), copySrc.getTiles()+(numTiles), getTiles());
	}
	return *this;
}

Tile& Tileset::getTilesetData(uint8_t tileNum) {
    Tile& tile = tiles[tileNum];
    return tile;
}
void Tileset::setTileData(uint8_t tileNum, Tile* tile) {
    tiles[tileNum] = *tile;
}

Tile* Tileset::getTiles() const{
	return tiles.get();
}

Tilemap::Tilemap(Tileset& tileset, uint8_t* mapBuf, uint8_t tiles_wide, uint8_t tiles_high){
    this->x=0;
    this->y=0;
    this->tiles_wide = tiles_wide;
    this->tiles_high = tiles_high;

    this->tileset = &tileset;
    map = std::unique_ptr<uint8_t[]>(new uint8_t[tiles_wide*tiles_high]);
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        map[i] = *mapBuf;
        mapBuf++;
    }
}

Tilemap::Tilemap(int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset& tileset, uint8_t* mapBuf){
    this->x = x;
    this->y = y;
    this->tiles_wide = tiles_wide;
    this->tiles_high = tiles_high;
    this->tileset = &tileset;
    map = std::unique_ptr<uint8_t[]>(new uint8_t[tiles_wide*tiles_high]);
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        map[i] = *mapBuf;
        mapBuf++;
    }       
}

Tile* Tilemap::getTilemapData(uint16_t tileNum) {
    return &(tileset->getTilesetData(map[tileNum]));
}
void Tilemap::setTilemap(uint16_t tileNum, uint16_t newTile) {
	map[tileNum] = newTile;
}

//NOTE: THIS IS THE BLITTING ALGO.
//consider using an iterator
void Tilemap::byteBlit( uint16_t* dst ) { 

	/*
	 * IMPLICATION:
	 * I will load a buffer the size of sprite into memory; effectively i'm 
	 * copying a x by y tiles sized buffer for the EXPRESS purpose 
	 * of background filtering.
	 * Should be used relatively sparingly, for UI/sprites.
	 */
	
	switch(order_code) {

		case DRAW_ORDER_ROW_MAJOR:
			for(int i = 0; i<tiles_high*DEFAULT_TILE_LEN; i++) {
				int y_offset__ = i%DEFAULT_TILE_LEN;
				for(int j = 0; j<tiles_wide; j++) {
					int tilemapIdx = i/DEFAULT_TILE_LEN*tiles_wide + j;

					//get the row's pixel data 
					Tile* tile_ = getTilemapData(tilemapIdx); 
					uint16_t* ptr = tile_->getBuf();
					ptr+=(y_offset__*DEFAULT_TILE_LEN);

					for(int count = 0 ; count<DEFAULT_TILE_LEN ; count++) {
						if(*ptr!=ALPHA_CLR_565) {
							*dst = *ptr;
						} 				
						dst++;
						ptr++;
					}
				}
			}
			break;

		case DRAW_ORDER_TILE_COLUMN_MAJOR:
			printf("PEE");
			break;
	}
}

uint16_t Tilemap::contextualize(uint8_t x, uint8_t y) {
	uint16_t idx = x/DEFAULT_TILE_LEN*tiles_wide+y/DEFAULT_TILE_LEN;
	Tile* tile =  getTilemapData(idx);
	
	return tile->getPixel(x%DEFAULT_TILE_LEN, y%DEFAULT_TILE_LEN);
}

Scene::Scene(Tileset& tileset, uint8_t* mapBuf){ //basically a hardware config
    this->x=0;
    this->y=0;
    this->tiles_wide = 20;
    this->tiles_high = 15;

    this->tileset = &tileset; 
    map = std::unique_ptr<uint8_t[]>(new uint8_t[tiles_wide*tiles_high]);
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        map[i] = *mapBuf;
        mapBuf++;
    }
}

Sprite::Sprite(int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset& tileset, uint8_t* mapBuf): Tilemap(x,y,tiles_wide,tiles_high, tileset, mapBuf){}

Sprite::Sprite(const Sprite& other) {
    this->x=other.x;
    this->y=other.y;
    this->tiles_wide = other.tiles_wide;
    this->tiles_high = other.tiles_high;
    this->tileset = other.tileset;

    map = std::unique_ptr<uint8_t[]>(new uint8_t[tiles_wide*tiles_high]);
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        this->map[i] = other.map[i];
    }       
}

Sprite& Sprite::operator=(const Sprite& other) {
    if (this == &other) return *this;
    
    this->x=other.x;
    this->y=other.y;
    this->tiles_wide = other.tiles_wide;
    this->tiles_high = other.tiles_high;
    this->tileset = other.tileset;

    map = std::unique_ptr<uint8_t[]>(new uint8_t[tiles_wide*tiles_high]);
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        this->map[i] = other.map[i];
    }       
    return *this;
}

void Sprite::setPosition(uint16_t x, uint16_t y){
    this->x = x;
    this->y = y;
}

void Sprite::setMap(uint8_t* mapBuf) {
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        map[i] = *mapBuf;
        mapBuf++;
    } 
}

uint8_t Sprite::getID() {
    return this->sprite_id;
}
void Sprite::setID(uint8_t id) {
    this->sprite_id = id;
}

//remember: top-left to bottom-right
void Sprite::setMapVector_Offsets(Tilemap& tilemap) {
    uint8_t tile_len = tileset->tile_len;
    this->x_offset = x%tile_len;
    this->y_offset = y%tile_len;

    int i_max = tiles_high;
    int j_max = tiles_wide;
    if(x_offset!=0) j_max++;
    if(y_offset!=0) i_max++;

    uint16_t x_boundary, y_boundary;
    for (int i = 0; i<i_max; i++) {
        y_boundary = (y+i*tile_len)/tile_len;
        for(int j = 0; j<j_max; j++) {
            x_boundary = (x+j*tile_len)/tile_len;
            uint16_t basemap_tile_idx = (y_boundary * tilemap.tiles_wide) + x_boundary;
            map_vec.push_back(basemap_tile_idx);
        }
    }
}

void Sprite::resetMapVector_Offsets(){ 
    this->x_offset = 0;
    this->y_offset = 0;

    map_vec.clear();
}

RenderController::RenderController(Scene& scene) { //modify the tilemap variants and their members
    this->base = &scene;

    guideLen = (base->tiles_wide)*(base->tiles_high);
    mapGuide = new uint8_t[guideLen];

    for(int i = 0 ; i<guideLen ; i++) {
        mapGuide[i] = 1;
    }
    
}

// partial frame updates; write a render block and then await clr;
// THIS IS FRAME-BY-FRAME
// TODO : update in a contiguous block ; reflect in indexList
void RenderController::render() {
	//check for sprite updates
	//determine region(s) of change and index
	//do bit blitting algo, use dirty rendering
	//return
	cursor->setPosition(67, 67);
	cursor->setMapVector_Offsets(*base); 
	
	size_t s=cursor->tiles_high*DEFAULT_TILE_LEN*cursor->tiles_wide*DEFAULT_TILE_LEN;
	scratchBuf = new std::vector<uint16_t>(s); //allocation step

	int i = 0;
	int j = 0;
	for(; i<cursor->tiles_high*DEFAULT_TILE_LEN; i++) {
		for(; j<cursor->tiles_wide*DEFAULT_TILE_LEN; i++) {
			scratchBuf->push_back(base->contextualize(cursor->x_offset+j, cursor->y_offset+i));
			//NOTE: BOTTLENECK!!!!!!!!!!!!!
		}
	}

	cursor->byteBlit(scratchBuf->data());
	//now read scratch buf

}

RenderController::display_msg_t RenderController::give_block() {
	display_msg_t msg;
	msg.buf = scratchBuf->data();
	msg.numPixels = cursor->tiles_high*DEFAULT_TILE_LEN*cursor->tiles_wide*DEFAULT_TILE_LEN;

	return msg;
}


void RenderController::sprite_update( uint8_t* spriteMapBuf) {
	cursor->setMap(spriteMapBuf);
}

void RenderController::sprite_update( uint16_t x, uint16_t y) {
	cursor->setPosition(x,y);
}

void RenderController::sprite_update( uint16_t x, uint16_t y,uint8_t* spriteMapBuf) {
	cursor->setMap(spriteMapBuf);
	cursor->setPosition(x,y);
}
