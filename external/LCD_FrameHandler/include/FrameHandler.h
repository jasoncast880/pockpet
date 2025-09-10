#ifndef FRAMEHANDLER_H
#define FRAMEHANDLER_H

#include <stdexcept>
#include <cstring>
#include <string>
#include <queue>
#include <memory>
#include <vector>

#define ALPHA_CLR_565 0xF81F //a 565 magenta color

struct Tile { //implement assuming indexed color
    uint8_t tile_len;
    std::unique_ptr<uint16_t[]> buf;
public:

    Tile();                             //default 16x16
    Tile(int tile_len, uint16_t* srcBuf);  //builds from a buffer

    Tile(const Tile& other);
    Tile& operator=(const Tile& copySource);

    Tile(Tile&&) noexcept = default;
    Tile& operator=(Tile&&) noexcept = default;

    void changePixel(uint16_t index, uint16_t value); 
    uint16_t getPixel(uint16_t x,uint16_t y); 
                                             
    ~Tile() = default; 
};

struct Tileset{
    uint8_t numTiles;
    uint16_t* buf; //sole purpose to populate tileArr, when using 3-param constructor

    std::unique_ptr<Tile[]> tiles;
public:

    Tileset(Tile* tiles, uint8_t numTiles); 
    Tileset(uint8_t tile_len, uint16_t* bufPtr, uint8_t numTiles); 

    Tileset(const Tileset& other);
    Tileset& operator=(const Tileset& other);

    Tileset(Tileset&&) noexcept = default;
    Tileset& operator=(Tileset&&) noexcept = default;

    Tile* getTilesetData(uint8_t tileNum);
    void setTileData(uint8_t tileNum, Tile* tile);

    ~Tileset() = default;
};

/* maps to the frame, so it should be (assuming 16 pix tilemaps)
 *  .____.
 *  |    | 240:15 tiles
 *  o____.
 *   320:20 tiles
 *
 *  IF using 8 pix tilemap: 40 tiles by 30 tiles
 */ 

struct Tilemap{ 
    uint8_t x, y;
    uint8_t tiles_wide, tiles_high;
    Tileset* tileset;
    std::unique_ptr<uint16_t[]> map; //map tileset num. to tilespace
public:
    Tilemap();
    Tilemap(Tileset* tileset, uint8_t* mapBuf,uint8_t tiles_wide, uint8_t tiles_high); 

    Tile* getTilemapData(uint16_t tileNum);
    void setTilemap(uint16_t tileNum, uint16_t newTile);

    virtual ~Tilemap()=defualt;
}; 

class Scene: public Tilemap{ //only make 1
public:
    Scene(Tileset* tileset, uint8_t* mapBuf); 
};

class Sprite: public Tilemap{
    uint8_t final_tiles_wide, final_tiles_high, spriteid; 
    std::vector<uint16_t> tiles_buf; //for final tiles
                                 
public:
    Sprite(int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf);
    ~Sprite();

    Sprite(const Sprite& other);
    Sprite& operator=(const Sprite& other);

    Sprite(Sprite&&) noexcept = default;
    Sprite& operator=(Sprite&&) noexcept = default;

    void setMap(uint8_t* mapBuf); //change the entire map
    void writeTilesBuf(uint16_t* src, size_t size);
    void resetTilesBuf();

    ~Sprite()=default;
};

class RenderController { //masher
    Scene base;
    uint8_t numSprites;
    Sprite[] sprites;

    uint8_t[] mapGuide; // helps keeps count of dirty, clean tiles
    size_t guideLen;//number of elements in mapGuide&mapBuf
     /*
    mapGuide[x] = 0: CLEAN : dont do nothing
    mapGuide[x] = 1: DIRTY : replace tile 'x' with tileset->tiles[mapBuf[x]] 
      * IMPORTANT!!!!
      * mapGuide[x] = 0 should only be called if the class 
      * user has drawn base->tileset->tileArr[x]::
      * 
    */

    uint8_t hashPos(int x_pix,int y_pix); 

    Tilemap* getBaseTilemap(); 
    set setSpriteTileset();
public:
    void render(); 
    void sprite_render(int spriteid, uint8_t* spriteMapBuf); 
    void sprite_render(int spriteid, uint16_t x, uint16_t y); 
    void sprite_render(int spriteid, uint16_t x, uint16_t y,uint8_t* spriteMapBuf); 
};

#endif
