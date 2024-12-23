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
//can have terrain tiles, ui tiles, bg ui tiles. leaves room for polymorphism.

/*
class Frame {
private:
uint8_t* changedTiles[];//store all of the addresses of changed tiles in frameBuf. this 

public:
static uint8_t frameBuffer[];
static uint8_t* tileMap[];

//tile funcs
void updateTile(Tile tile);

void pushWholeFrame(); //resend the entire contents of frameBuffer[]
void pushUpdatedTiles(uint8_t* changedTiles[]); //resend the appropriate tiles based on changedTiles[]; also clear the array.
};
*/

//something like this should be suitable
//only needs to access the HAL's DRAM, adhere to some timing constraints.
//frame is an object that only needs one instance
//tile is gonna go polymorphic probably.
