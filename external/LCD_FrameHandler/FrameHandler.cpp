#include "include/FrameHandler.h"
#include <algorithm>
#include <memory>

/*
 * @brief: this class will handle all of the buffer rendering *before* the sprites are added. tilemaps & sets used to reduce wram size.
 *
 * */

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
    
    sprites.reserve(10); //this is super variable
}

//built and optimized for partial frame updates
void RenderController::render() {
	for(int i = 0 ; i < sprites.size() ; i++) {
		Sprite& sprite = sprites[i];
		
		if(true) { //add logic to see if sprite pos has changed...
			baseBuf.clear();
			spriteBuf.clear();

			sprite.setMapVector_Offsets((Scene&)*base);
			uint8_t tile_len = base->tileset->tile_len;

			//get the base tiles to alter
			for(int j = 0; j<sprite.map_vec.size(); j++) {
				Tile* tempTile = base->getTilemapData(sprite.map_vec[j]);
				uint16_t* buf = tempTile->getBuf();
				for(int k = 0; k<tile_len*tile_len; k++) {
					baseBuf.push_back(*buf);
					buf++;
				}

				//alter the mapGuide
				mapGuide[sprite.map_vec.at(j)] = 1;
			}

			//get the sprit tiles to alter
			for(int j = 0; j<sprite.tiles_wide*sprite.tiles_high; j++) {
				Tile* tempTile = sprite.getTilemapData(j);
				uint16_t* buf = tempTile->getBuf();
				for(int k = 0; k<tile_len*tile_len; k++) { //reuse tile_len from base
					spriteBuf.push_back(*buf);
					buf++;
				}
			}

			//iterate through the bufs and mash
			int baseBuf_pix_width = tile_len*base->tiles_wide;
			int baseBuf_pix_height = tile_len*base->tiles_high;
			bool x_cond, y_cond;
			int counter = 0;
			for(int j = 0; j<baseBuf_pix_height; j++) {
				y_cond = (j>=sprite.y_offset) ? true : false;
				for(int k = 0; k<baseBuf_pix_width; k++) {
					x_cond = (k>=sprite.x_offset) ? true : false;
					if( x_cond&& y_cond && (counter<spriteBuf.size())
						&& (spriteBuf[counter]!=ALPHA_CLR_565)) {
						baseBuf[(i*baseBuf_pix_width)+j] = spriteBuf[counter]; //FILTERED
					}
				}
			} //now convert this to a tiles

			for(int j = 0; j<sprite.map_vec.size(); j++) {
				uint16_t* buf = baseBuf.data();
				renderedTiles.push_back(Tile(tile_len, buf));
				buf+=tile_len*tile_len;

				indexList.push_back(sprite.map_vec[i]);
			}
		}
	}
}

display_msg_t RenderController::giveTiles(size_t s) {
	//TODO
}

void RenderController::sprite_add(Sprite& sprite) {
    sprites.push_back(sprite);
    sprite.setID(sprites.size());
}

void RenderController::sprite_update(int spriteid, uint8_t* spriteMapBuf) {
    Sprite& sprite = sprites.at(spriteid);
    sprite.setMap(spriteMapBuf);
}

void RenderController::sprite_update(int spriteid, uint16_t x, uint16_t y) {
    Sprite& sprite = sprites.at(spriteid);
    sprite.setPosition(x,y);
}

void RenderController::sprite_update(int spriteid, uint16_t x, uint16_t y,uint8_t* spriteMapBuf) {
    Sprite& sprite = sprites.at(spriteid);
    sprite.setMap(spriteMapBuf);
    sprite.setPosition(x,y);
}
