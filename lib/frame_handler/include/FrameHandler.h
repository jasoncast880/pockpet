#pragma once

#include "pico/stdlib.h"
#include "ili9341.h"

#include <stdexcept>
#include <cstring>
#include <string>

struct Tile { //implement assuming indexed color
public:
    int tile_len;        
    uint8_t* buf_ptr;

    Tile();
    Tile(int tile_len, uint8_t* buf_ptr);
    Tile& operator=(const Tile& other);
    void render(uint16_t x, uint16_t y); //bot left corner (x,y)
    void changePixel(uint16_t index, uint8_t color); //bot left corner (x,y)
    uint8_t getPixel(uint16_t x,uint16_t y); //return indexed color value
                                             
    ~Tile(); //ok
};

struct Tileset{
private:
    uint8_t* bufPtr; //sole purpose to populate tileArr
public:
    int tile_len;
    uint8_t numTiles;

    Tile* tileArr;

    //default constructor for passing by reference
    Tileset();
    Tileset(int tile_len, uint8_t* bufPtr, uint8_t numTiles);
    Tileset(Tile* tiles, uint8_t numTiles); 

    Tileset(const Tileset& other);
    Tileset& operator=(const Tileset& other);
    //Tileset(const Tileset && other);
    //Tileset& operator=(const Tileset && other);

    virtual void render(uint16_t x, uint16_t y, uint8_t tileNum); 
    Tile* getTileData(uint8_t tileNum);
    void setTileData(uint8_t tileNum, Tile* tile);

    ~Tileset(); //need to fix
};

/* maps to the frame, so it should be (assuming 16 pix tilemaps)
 *  .____.
 *  |    | 240:15 tiles
 *  o____.
 *   320:20 tiles
 *
 *  IF using 8 pix tilemap: 40 tiles by 30 tiles
 *
 *  implement feature:
 *  tilemap has to remember which tiles have been changed, so that it can 
 *  re-render the necessary pixels.
 *
 */ 

struct Tilemap{ 
public:
    uint8_t x, y;
    uint8_t tiles_wide, tiles_high;
    Tileset* tileset;
    uint8_t* mapBuf;

    Tilemap();
    Tilemap(Tileset* tileset, uint8_t* mapBuf); //take the whole screen
    void render();
    Tile* getTileData(uint8_t tileNum);
    void alterTile(Tile* tile, Tile* newTile);

    virtual ~Tilemap();
}; 

struct Base: public Tilemap{
public:
    uint8_t* mapGuidePtr;
    size_t guideLen;

    /* idea: make a second tileset for masked sprite data
     * allows me to keep the current base without changing the size of tileset
     * !!members associated below!!
     */
    Tileset* spareTileset; //temporary: 20 tiles
    //

    Base();
    Base(Tileset* tileset, uint8_t* mapBuf); //take the whole screen
    void render();
    void printMapGuide();
    
    //base will never be destroyed (so far)
};

struct Sprite: public Tilemap{
private:
    void getDims();
    uint8_t sprite_size;
    uint8_t statusFlag = 0x00;

    Tile* spliceX(Tile* tile1, Tile* tile2, uint8_t cutoff); //after splicing, insert into sprite_Tileset
    Tile* spliceY(Tile* tile1, Tile* tile2, uint8_t cutoff);
    Tile* merge(Tile* tile1, Tile* tile2);

    uint8_t hashPos(int x_pix,int y_pix); //get base's tilemap index of current tile idx via x/y
    Tileset* baseTiles;
    void tileset_validator(Tileset*,int w, int h);

public:
    Base* basePtr;

    // positional data;
    int x,y;
    //describes the height/width of the spritemap
    uint8_t tiles_wide, tiles_high; 
    Tileset* tileset;
    uint8_t* mapBuf;


    Sprite();
    Sprite(Base* basePtr, int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf);
    ~Sprite();

    // members created after mashing 
    Tileset* finishedTiles;

    void render(); 
    void render(uint8_t* spriteMapBuf); 
    void render(uint16_t x, uint16_t y); 
    void getBaseTileset();
    void getFinalTileset();

    void setPosition(uint16_t x, uint16_t y);
};

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
