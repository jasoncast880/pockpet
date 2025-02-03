#pragma once

#include "pico/stdlib.h"
#include "ili9341.h"

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
};

struct Tileset{
public:
    int tile_len;
    uint8_t* bufPtr;
    uint8_t numTiles;

    Tile* tileArr;

    //default constructor for passing by reference
    Tileset();
    Tileset(int tile_len, uint8_t* bufPtr, uint8_t numTiles);
    virtual void render(uint16_t x, uint16_t y, uint8_t tileNum); 
    uint8_t* getTileData(uint8_t tileNum);
    void setTileData(uint8_t tileNum);

    ~Tileset();
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

    void alterTile(uint8_t tileNo, uint8_t newTile);
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
};

struct Sprite: public Tilemap{
private:
    //helper functions
    void set_position(int x0, int y0);
    void mask_on_mapGuide(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height); //subroutine on constructor call
public:
    Base* basePtr;
    uint8_t* guidePtr;

    Sprite(Base* basePtr, int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf);
    void render(); //render will 'temporarily' render the sprite, once its lifecycle is done and/or its position changes, the space it occupies in vram becomes the base sprite again
    
    //1) retreive the tiles of the sprite object, associated configurations
    //2) retrieve data of the base objects'tiles that this object will be over
    //3) mask these tiles inside of the sprite object subroutine and then
    //prepare them to be written in base class's spare tiles tileset
    //4) render spare tiles in the proper configuration (from base::render())

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
