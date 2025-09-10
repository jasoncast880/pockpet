#include "include/FrameHandler.h"

/*
 * @brief: this class will handle all of the buffer rendering *before* the sprites are added. tilemaps & sets used to reduce wram size.
 *
 * */

Tile::Tile()
    : tile_len(16), buf(std::unique_ptr<uint16_t[]>(new uint16_t[16*16])) {}
Tile::Tile(int tile_len, uint16_t* srcBuf)
    : tile_len(tile_len), 
      buf(std::unique_ptr<uint16_t[]>(new uint16_t[tile_len*tile_len])) {
           for (int i = 0 ; i < tile_len*tile_len ; i++) {
              buf[i] = srcBuf[i];
          }
      }

Tile::Tile(const Tile& other)
    : tile_len(other.tile_len),
      buf(std::unique_ptr<uint16_t[]>(new uint16_t[other.tile_len*other.tile_len])) {
          for (int i = 0 ; i < tile_len*tile_len ; i++) {
              buf[i] = other.buf[i];
          }
      }
Tile::Tile& operator=(const Tile& copySource) {
    if (this == &copySource) return *this;
    tile_len = copySource.tile_len;
    buf = std::unique_ptr<uint16_t[]>(new uint16_t[tile_len*tile_len]);
    for (int i = 0 ; i < tile_len*tile_len ; i++) {
        buf[i] = copySource.buf[i];
    }
    return *this;
}

Tile::Tile(Tile&&) noexcept = default;
Tile::Tile& operator=(Tile&&) noexcept = default;

void Tile::changePixel(uint16_t index, uint16_t value) {
    buf[index] = value;
}

uint16_t Tile::getPixel(uint16_t x,uint16_t y) const {
    int index = (y*tile_len)+x;
    return buf[index];
}

Tileset::Tileset(Tile* tiles, uint8_t numTiles) //deep copy tiles..
    : numTiles(numTiles), 
      tiles(std::unique_ptr<Tile[]>(new Tile[numTiles])) {
          for (int i = 0 ; i < numTiles ; i++) {
              this->tiles = tiles[i];
          }
      }
Tileset::Tileset(uint8_t tile_len, uint16_t* bufPtr, uint8_t numTiles) 
    : numTiles(numTiles),
    tiles(std::unique_ptr<Tile[]>(new Tile[numTiles])) {
        uint16_t* tileBuf = new uint16_t [tile_len*tile_len];
        for(int i = 0 ; i < numTiles ; i++) {
            for(int j = 0 ; j < (tile_len*tile_len) ; j++){
                tileBuf[j] = *bufPtr;
            }
            //now load the tileBuf into Tile constructor
            tiles[i] = Tile(tile_len, &tileBuf[0]); 
        }
        delete[] tileBuf;
    }

Tileset::Tileset(const Tileset& other)
    : numTiles(other.numTiles),
    tiles(std::unique_ptr<Tile[]>(new Tile[numTiles])) {
        for (int i = 0 ; i < numTiles ; i++) {
            Tile[i] = other->tiles[i];
        }
    }
Tileset::Tileset& operator=(const Tileset& other) {
    if (this == &other) return *this;
    numTiles = copySource.numTiles;
    tiles = std::unique_ptr<Tile[]>(new Tile[numTiles]);
    for (int i = 0 ; i < numTiles; i++) {
        tiles[i] = other->tiles[i];
    }
}

Tileset::Tileset(Tileset&&) noexcept = default;
Tileset::Tileset& operator=(Tileset&&) noexcept = default;

Tile* Tileset::getTilesetData(uint8_t tileNum) {
    return tiles[tileNum];
}
void Tileset::setTileData(uint8_t tileNum, Tile* tile) {
    tiles[i] = tile;
}


Tilemap::Tilemap()
    : x(0), y(0), tiles_wide(20), tiles_tall(15) {} //default dims for 240x320 pix screen
Tilemap::Tilemap(Tileset* tileset, uint8_t* mapBuf, uint8_t tiles_wide, uint8_t tiles_high){
    this.x=0;
    this.y=0;
    this.tiles_wide = tiles_wide;
    this.tiles_high = tiles_high;

    this->tileset = tileset; 
    map = std::unique_ptr<uint16_t[]>(new uint16_t[tiles_wide*tiles_high]);
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        map[i] = *mapBuf;
        mapBuf++;
    }
}

Tile* Tilemap::getTilemapData(uint16_t tileNum) {
    return map[tileNum];
}
void Tilemap::setTilemap(uint16_t tileNum, uint16_t newTile) {
    map[tileNum] = newTile;
}

virtual ~Tilemap()=defualt;


Scene::Scene(Tileset* tileset, uint8_t* mapBuf){ //basically a hardware config
    this.x=0;
    this.y=0;
    this.tiles_wide = 20;
    this.tiles_high = 15;

    this->tileset = tileset; 
    map = std::unique_ptr<uint16_t[]>(new uint16_t[tiles_wide*tiles_high]);
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        map[i] = *mapBuf;
        mapBuf++;
    }
}

Sprite::Sprite(int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf) 
    : x(x), y(y), tiles_wide(tiles_wide), tiles_high(tiles_high),
    tileset(tileset),
    tiles_buf(){
        map = std::unique_ptr<uint16_t[]>(new uint16_t[tiles_wide*tiles_high]);
        for (int i = 0; i < tiles_wide*tiles_high ; i++) {
            map[i] = *mapBuf;
            mapBuf++;
        }       
    }

Sprite(const Sprite& other)
    : x(other.x), y(other.y), tiles_wide(other.tiles_wide), tiles_high(other.tiles_high),
    tileset(other->tileset),
    tiles_buf(){
        map = std::unique_ptr<uint16_t[]>(new uint16_t[tiles_wide*tiles_high]);
        for (int i = 0; i < tiles_wide*tiles_high ; i++) {
            map[i] = *mapBuf;
            mapBuf++;
        }       
    }

Sprite& operator=(const Sprite& other) {
    if (this == &other) return this;
    
    this.x=other.x;
    this.y=other.y;
    this.tiles_wide = other.tiles_wide;
    this.tiles_high = other.tiles_high;
    this->tileset = other->tileset;

    map = std::unique_ptr<uint16_t[]>(new uint16_t[tiles_wide*tiles_high]);
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        map[i] = other.map[i];
    }       
}

Sprite(Sprite&&) noexcept = default;
Sprite& operator=(Sprite&&) noexcept = default;

void Sprite::setMap(uint8_t* mapBuf) {
    for (int i = 0; i < tiles_wide*tiles_high ; i++) {
        map[i] = *mapBuf;
        mapBuf++;
    } 
}
//src size: (for later)
//(tileset->tiles->tile_len)*(tileset->tiles->tile_len)*tiles_wide*tiles_high
void Sprite::writeTilesBuf(uint16_t* src, size_t size) {
    for(int i = 0 ; i<size; i++) {
        tiles_buf.push_back(*src);
        src++;
    }
}
void Sprite::resetTilesBuf() {
    tiles_buf.clear();
}

Sprite::~Sprite()=default;
