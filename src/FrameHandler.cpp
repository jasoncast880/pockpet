#include "include/FrameHandler.h"

/*
 * @brief: this class will handle all of the buffer rendering *before* the sprites are added. tilemaps & sets used to reduce wram size.
 *
 * control the frame buffer within wram, and you control the display itself.
 * */

/*
 * GOALS OF THIS HELPER FILE:
 * is it faster to:
 * -update specific tiles vs updating whole frame using dma?
 *
 * is it more mem efficient? (definitely, this is the main advantage of tiles)
 *
 */

//uint8_t frameBuffer[(240)*(320)] = new uint8_t[];

Tile::Tile(int tile_len, uint8_t* bufPtr) {
    this->tile_len = tile_len;
    this->bufPtr = bufPtr;
}

void Tile::render(uint16_t x, uint16_t y) {
    ili9341_setAddrWindow(x, y, this->tile_len, this->tile_len);
    ili9341_writeCommand(RAM_WR);
    ili9341_writeDataBuffer(this->bufPtr, 128); //size_t is  32 bits, (4 pixels) so (tile_len/4pix)*tile_len
    ili9341_writeCommand(NOOP);
    sleep_ms(250);// temp for timing purposes. tweak later.
}

