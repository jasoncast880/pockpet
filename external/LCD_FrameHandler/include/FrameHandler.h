#ifndef FRAMEHANDLER_H
#define FRAMEHANDLER_H

#include <stdexcept>
#include <cstring>
#include <string>
#include <queue>
#include <memory>

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
;

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
public:
    uint8_t x, y;
    uint8_t tiles_wide, tiles_high;
    Tileset* tileset;
    uint8_t* mapBuf;

    Tilemap();
    Tilemap(Tileset* tileset, uint8_t* mapBuf,uint8_t tiles_wide, uint8_t tiles_high); //take the whole screen
    Tile* getTilemapData(uint8_t tileNum);
    void alterTile(uint16_t tile_idx, uint8_t newTile);
    uint8_t hashPos(int x_pix,int y_pix); 
    virtual ~Tilemap();
}; 

struct Sprite;

struct Base: public Tilemap{ //only make 1
public:
    uint8_t* mapBuf;   // keeps track of which tiles go where
    uint8_t* mapGuide; // helps keeps count of dirty, clean tiles
     /*
    mapGuide[x] = 0: CLEAN : dont do nothing
    mapGuide[x] = 1: DIRTY : replace tile 'x' with tileset->tiles[mapBuf[x]] 
      * IMPORTANT!!!!
      * mapGuide[x] = 0 should only be called if the class 
      * user has drawn base->tileset->tileArr[x]::
      * 
    */

    size_t guideLen;//number of elements in mapGuide&mapBuf

    Tileset* copyTiles; //a copy of the original base, so once sprites 
                        //leave you can recreate base during clean op
    Sprite* spriteArr;
    uint8_t numSprites;

    /* 
     * !!members associated below!!
     */

    Base();
    Base(Tileset* tileset, uint8_t* mapBuf); //take the whole screen
    void printMapGuide();
    //base will never be destroyed (so far)
    void render();
    
};

struct Sprite: public Tilemap{
private:
    void getDims();
    uint8_t sprite_size;
    uint8_t statusFlag = 0x00;

    struct hashedPix; 
    void tileset_validator(Tileset*,int w, int h);

    //map based on tiles sprite contiguously inhabits
    //used in getFinalTileset()
    Tilemap* getBaseTilemap(); 

    //for processing the tileset
    Tileset* getFinalTileset();

public:
    Base* base;

    // positional data;
    int x,y;
    //describes the height/width of the spritemap
    uint8_t tiles_wide, tiles_high; 
    uint8_t final_tiles_wide, final_tiles_high; 
    Tileset* tileset;
    uint8_t* mapBuf;

    Sprite();
    Sprite(Base* base, int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf);
    ~Sprite();

    // members created after mashing 
    Tileset* finishedTiles;

    void render(); 
    //serve as a setter and a processing func same time!!
    void render(uint8_t* spriteMapBuf); 
    void render(uint16_t x, uint16_t y); 
    void render(uint16_t x, uint16_t y,uint8_t* spriteMapBuf); 
};

//these below should go into a menu/ui configuration file, on 
//app level
/*
struct Char_16{ //simplified hash-map structure for storing font data
public:
    char glyph;
    uint8_t tileNum;
    size_t len=16*16;
};

struct Font{
public:
    Tileset* tileset;
    char* charBuf;
    Char_16 fontArr[250]; 

    Font();
    Font(Tileset* tileset, char* charBuf,size_t len);
    void printFont(uint8_t x,uint8_t y,std::string txt);
};
*/



#endif
