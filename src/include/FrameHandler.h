#pragma once

#include "pico/stdlib.h"
#include "ili9341.h"

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
    void render(uint16_t x, uint16_t y, uint8_t tileNum);
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
    //assume u take up the entire screen
    Tileset* tileset;
    uint8_t* mapBuf;
    uint8_t tiles_wide;
    uint8_t tiles_high;

    Tilemap(Tileset* tileset, uint8_t* mapBuf);
    void render();//assuming tilemap takes over the entire screen
    void alterTile(uint8_t tileNo, uint8_t* tilePtr); //todo
};

//can have terrain tiles, ui tiles, bg ui tiles. leaves room for polymorphism.

