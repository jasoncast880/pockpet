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


Tile::Tile(int tile_len, uint8_t* bufPtr) {
    this->tile_len = tile_len;
    this->bufPtr = bufPtr;
}

//testing right now...
//need to fix up ampalaya tools. so the tileset bmp makes sense
void Tile::render(uint16_t x, uint16_t y) {
    ili9341_setAddrWindow(x, y, (this->tile_len), (this->tile_len));
    ili9341_writeCommand(RAM_WR);
    ili9341_writeDataBuffer(this->bufPtr, (this->tile_len)*(this->tile_len)*(2)); 
    ili9341_writeCommand(NOOP);
    sleep_ms(250);// temp for timing purposes. tweak later.
}

Tileset::Tileset() {
}

Tileset::Tileset(int tile_len, uint8_t* bufPtr) {
    this->tile_len = tile_len;
    this->bufPtr = bufPtr;
}

uint8_t* Tileset::getTileData(uint8_t tileNum){//return tileset's tile no. data buf-ptr
    return (this->bufPtr+(tileNum*tile_len*2));
}

void Tileset::render(uint16_t x, uint16_t y, uint8_t tileNum) {
    ili9341_setAddrWindow(x, y, (this->tile_len), (this->tile_len));
    ili9341_writeCommand(RAM_WR);
    ili9341_writeDataBuffer(this->bufPtr+(tileNum*tile_len*tile_len*2), (tile_len)*(tile_len)*(2)); 
    ili9341_writeCommand(NOOP);
    //sleep_ms(250);// temp for timing purposes. tweak later.
    //revert to original bufPtr
}

Tilemap::Tilemap(Tileset* tileset, uint8_t* mapBuf){
    this->tileset = tileset;
    this->mapBuf = mapBuf;
    //HARDCODED SCREEN DIMS, add err handling just in case later.
    this->x=0;
    this->y=0;
    this->tiles_wide = 320/tileset->tile_len;
    this->tiles_high = 240/tileset->tile_len;
}

Tilemap::Tilemap(uint8_t x, uint8_t y, uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf){
    this->x = x;
    this->y = y;
    this->tiles_wide = tiles_wide;
    this->tiles_high = tiles_high;
    this->tileset = tileset;
    this->mapBuf = mapBuf;
}

void Tilemap::render(){ //rendr a whole screen frame; add the update frames
    int counter = 0;
    for(int i=0; i<tiles_high; i++){
        for(int j=0;j<tiles_wide;j++){
            tileset->render((x+j*tileset->tile_len),(y+i*tileset->tile_len),mapBuf[counter]);
            counter++;
        }
    }
}



//Font: Char_16 fontArr[100]
Font::Font(Tileset* tileset, char* charBuf, size_t len){
    for(int i=0;i<(len);i++){
        size_t index = static_cast<size_t>(*charBuf);
        fontArr[index].glyph=*charBuf;
        fontArr[index].bufPtr=tileset->getTileData(196-i);
        charBuf++;
        //
        printf("%d\n",i);
        sleep_ms(100);
    } //hashes all of the chars.
    printf("finished loop");
}

void Font::printFont(uint8_t x, uint8_t y, std::string txt){
    for(int i=0; i<txt.size(); i++){
        uint8_t* tempBuf=fontArr[static_cast<size_t>(txt[i])].bufPtr;

        ili9341_setAddrWindow(x+(16*i)+2, y, 16,16); //ASSUME A 16px tileset
        ili9341_writeCommand(RAM_WR);
        ili9341_writeDataBuffer(tempBuf, 16*16*2);
        ili9341_writeCommand(NOOP);
        sleep_ms(100);
    }
}
