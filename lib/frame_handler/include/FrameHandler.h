#pragma once

#include "pico/stdlib.h"
#include "ili9341.h"

#include <string>

class Tile {
private:
    int tile_len;        
    uint8_t* bufPtr;

public:
    Tile(int tile_len, uint8_t* bufPtr);
    void render(uint16_t x, uint16_t y); //bot left corner (x,y)
};

struct Tileset{
public:
    int tile_len;
    uint8_t* bufPtr;
    int tileNum;

    //default constructor for passing by reference
    Tileset();
    Tileset(int tile_len, uint8_t* bufPtr);
    virtual void render(uint16_t x, uint16_t y, uint8_t tileNum);
    uint8_t* getTileData(uint8_t tileNum);
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
    Tilemap(uint8_t x,uint8_t y,uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf);
    void render();

    void alterTile(uint8_t tileNo, uint8_t newTile);
}; //built and tested on 16 pix tiles!!!

struct Base: public Tilemap{
public:
    uint8_t* mapGuidePtr;
    size_t guideLen;

    //dynamically allocate two mapguides, one before and one after a render pass
    Base(Tileset* tileset, uint8_t* mapBuf); //take the whole screen
    void render();
};

struct Sprite: public Tilemap{
public:
    Base* basePtr;
    uint8_t* guidePtr;

    Sprite(Base* basePtr, uint8_t x,uint8_t y,uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf);
    void render(); //render will 'temporarily' render the sprite, once its lifecycle is done and/or its position changes, the space it occupies in vram becomes the base sprite again

    //helper functions
    void set_position(uint8_t x0, uint8_t y0);
    void mask_on_mapGuide(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height); //subroutine on constructor call
};

//doesn't work...
struct Char_16{ //simplified hash-map structure for storing font data
public:
    char glyph;
    uint8_t* bufPtr;
    size_t len=16*16*2;
};

struct Font{
public:
    Tileset* tileset;
    char* charBuf;
    Char_16 fontArr[100]; //might have to alter this

    Font(Tileset* tileset, char* charBuf,size_t len);
    void printFont(uint8_t x,uint8_t y,std::string txt);
};
