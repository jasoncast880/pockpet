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
    return (bufPtr+((16*16*2)*tileNum));
}

void Tileset::render(uint16_t x, uint16_t y, uint8_t tileNum) {
    ili9341_setAddrWindow(x, y, (this->tile_len), (this->tile_len));
    ili9341_writeCommand(RAM_WR);
    ili9341_writeDataBuffer(this->bufPtr+(tileNum*tile_len*tile_len*2), (tile_len)*(tile_len)*(2)); 
    ili9341_writeCommand(NOOP);
    //sleep_ms(250);// temp for timing purposes. tweak later.
    //revert to original bufPtr
}

Tilemap::Tilemap(){} //default constructor (not used)

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

void Tilemap::render(){ 
    int counter = 0;
    for(int i=0; i<tiles_high; i++){
        for(int j=0;j<tiles_wide;j++){
            tileset->render((x+j*tileset->tile_len),(y+i*tileset->tile_len),mapBuf[counter]);
            counter++;
        }
    }
}

Base::Base(){}
Base::Base(Tileset* tileset, uint8_t* mapBuf){ //assuming a 16 pixel tileset
    this->x=0;
    this->y=0;
    this->tiles_wide = 320/tileset->tile_len;
    this->tiles_high = 240/tileset->tile_len;

    this->tileset = tileset;
    this->mapBuf = mapBuf;

    this->guideLen = tiles_wide*tiles_high; //size of arrays
    uint8_t* mapGuide = new uint8_t[guideLen]; //i will never release you.
    this->mapGuidePtr=&mapGuide[0];

    printf("from Base constructor:");
    printf("0x%d\n", mapGuidePtr);
    //populate the mapGuide: 1 is base's default tiles, 0 is sprite's tiles; 
    //compare the map arrays in compare_guides to see which tiles 
    //to re-render in a Base:render() pass
    for(int i=0; i<guideLen; i++){
        mapGuide[i]=1;
    }
}

void Base::render(){
    int counter = 0;
    for(int i=0; i<tiles_high; i++){
        for(int j=0;j<tiles_wide;j++){
            if(*(mapGuidePtr+(tiles_wide*i+j))==0){
                ili9341_writeCommand(NOOP);
            }
            else{ //clean up 'dirty' tiles on a base render pass
                tileset->render((j*tileset->tile_len),(i*tileset->tile_len),mapBuf[counter]);
                *(mapGuidePtr+(tiles_wide*i+j))=0;
            }
            counter++;
        }
    }

    printMapGuide();
}

void Base::printMapGuide(){
    //debug function
    printf("Tileguide:\n");
    int j=0;
    for(int i=0; i<guideLen; i++){ 
        printf("%d, ",*(mapGuidePtr+(i)));
        j++;
        if(j==tiles_wide){
            j=0;
            printf("\n");
        }
    }
    printf("EOA\n");
}

Sprite::Sprite(Base* basePtr, int x, int y, uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf){
    this->basePtr = basePtr;
    this->x = x;
    this->y = y;
    this->tiles_wide = tiles_wide;
    this->tiles_high = tiles_high;
    this->tileset = tileset;
    this->mapBuf = mapBuf;

    this->guidePtr=basePtr->mapGuidePtr;
}

void Sprite::set_position(int x0, int y0){
    //check to see if the new position frees up base tiles 
    //to be rendered out; 'dirty tiles'
    int dx, dy;
    //todo: make a getDiff func
    if ((x0-x)>=16){ //only works for sprites moving up, left
        dx=(x0-x);
    }

    if ((y0-y)>=16){ //only works for sprites moving up, left
        dy=(y0-y);
    }

    //rounding up formula: (a+b-1)/b

    if(dx&&!dy){
        dx=(dx+16-1)/16;
        mask_on_mapGuide(x/16,y/16,dx,tiles_high);
    }
    else if(!dx&&dy){
        dy=(dy+16-1)/16;
        mask_on_mapGuide(x/16,y/16,tiles_wide,dy);
    }
    else{ //diagonal (ish) motion
        printf("wip"); //i nono wanna
    }

    this->x = x0;
    this->y = x0;
    
}

void Sprite::render(){ //need to account for alpha processing...
    int counter=0;
    //'blindly' render; IF there is alpha processing or a sprite isn't rendered 
    //on a tile, then update the tile guide so base object can re-render 
    //during the rendering loop;
    
    for(int i = 0; i<tiles_high; i++){
        for(int j = 0; j<tiles_wide; j++){
            //please assume a 16-length tileset
            if(mapBuf[counter]==255){
                counter++;
                mask_on_mapGuide(((x/16)+j),((y/16)+i),1,1);
                //edge case to implement: off-tiles.
                //this implementation assumes that the 
                //sprite is squarely on a tile!!!!
            }
            else {
                tileset->render((x+j*16),(y+i*16),mapBuf[counter]);
                counter++;
            }
        }
    }
    basePtr->printMapGuide();
}

//!!! @param : by tiles, not by pixel!!!
void Sprite::mask_on_mapGuide(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height){
    //@brief: params: tileGuide's x coord, tileGuide's y coord, tiles wide, height
    //make 1's to 0's on the appropriate mask guide tiles
    //any sprite tile with alpha processing that moves,
    //or is removed from a guide-tile entirely

    uint8_t* temp = basePtr->mapGuidePtr+(y0*20)+x0;
    printf("0x%d\n", temp);
    for(uint8_t i=y0;i<height+y0;i++){
        for(uint8_t j=x0;j<width+x0;j++){
            *temp=1;
            temp++;
        }
        temp+=(20-width);
    }
}

Font::Font(){}

//Font: Char_16 fontArr[100]
Font::Font(Tileset* tileset, char* charBuf, size_t len){
    printf("size of struct Char_16: %zu bytes\n",sizeof(struct Char_16));
    for(int i=0;i<(int)len;i++){
        size_t index = static_cast<size_t>(*charBuf);
        fontArr[index].glyph=*charBuf;
        fontArr[index].bufPtr=tileset->getTileData(222-i);
        charBuf++;
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
        sleep_ms(250);
    }
}
