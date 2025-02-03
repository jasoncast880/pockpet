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

Tile::Tile():tile_len(0),buf_ptr(nullptr){} 
Tile::Tile(int tile_len, uint8_t* buf_ptr){
    this->tile_len = tile_len;
    this->buf_ptr = buf_ptr;
}

Tile& Tile::operator = (const Tile& copySource){
    //self-assignment check
    if(this == &copySource) return *this;
        
    delete[] buf_ptr; //free old mem
    tile_len = copySource.tile_len;

    //allocate new mem and cpy data
    buf_ptr = new uint8_t[tile_len*tile_len];
    std::memcpy(buf_ptr,copySource.buf_ptr, tile_len*tile_len);
    return *this;
}

//this renders indexed color
void Tile::render(uint16_t x, uint16_t y) {
    ili9341_setAddrWindow(x, y, (this->tile_len), (this->tile_len));
    ili9341_writeCommand(RAM_WR);
    for(int i = 0; i<((this->tile_len)*(this->tile_len));i++){
       ili9341_writeColorByIndex(*(this->buf_ptr+i));
    }
    ili9341_writeCommand(NOOP);
}

Tileset::Tileset() { //unused
}

Tileset::Tileset(int tile_len, uint8_t* bufPtr, uint8_t numTiles) {
    this->tile_len = tile_len;
    this->bufPtr = bufPtr;
    this->numTiles = numTiles;

    tileArr = new Tile[numTiles];
    for (int i=0;i<numTiles;i++){
        tileArr[i] = Tile(tile_len,(bufPtr+(i*tile_len*tile_len)));
    }
}

Tileset::~Tileset(){}

uint8_t* Tileset::getTileData(uint8_t tileNum){//return tileset's tile no. data buf-ptr
    return tileArr[tileNum].buf_ptr; //imp later
}

void Tileset::render(uint16_t x, uint16_t y, uint8_t tileNum) {
    tileArr[tileNum].render(x,y);
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

void Tilemap::render(){ 
    int counter = 0;

    for(int i = 0; i<tiles_high; i++){
        for(int j=0;j<tiles_wide; j++){
            tileset->render((x+j*tileset->tile_len),
                    (y+i*tileset->tile_len),
                    mapBuf[counter]
                    );
            counter++;
        }
    }
}

//need to account for edge case rendering
Base::Base(){}
Base::Base(Tileset* tileset, uint8_t* mapBuf){ 
    this->x=0;
    this->y=0;
    this->tiles_wide = 320/tileset->tile_len; //hardcoded screen dims!!
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

    spareTileset = new Tileset(tileset->tile_len,nullptr,20);
}

void Base::render(){
    int counter = 0;
    for(int i=0; i<tiles_high; i++){
        for(int j=0;j<tiles_wide;j++){
            if(*(mapGuidePtr+(tiles_wide*i+j))==0){
                ili9341_writeCommand(NOOP);
            }
            else{ //clean up 'dirty' tiles on a base render pass
                tileset->render((j*tileset->tile_len),
                        (i*tileset->tile_len),
                        mapBuf[counter]);
                *(mapGuidePtr+(tiles_wide*i+j))=0;
            }
            counter++;
        }
    }

    printMapGuide(); //for debug
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
//only works for 16 pixel fonts!!
//Font: Char_16 fontArr[100]
Font::Font(Tileset* tileset, char* charBuf, size_t len){
    printf("size of struct Char_16: %zu bytes\n",sizeof(struct Char_16));
    this->tileset = tileset;
    for(int i=0;i<(int)len;i++){
        size_t index = static_cast<size_t>(*charBuf);
        fontArr[index].glyph=*charBuf;
        fontArr[index].tileNum=(222-i); //specific to rook tileset
        charBuf++;
    } //hashes all of the chars.
    printf("finished loop");
}

void Font::printFont(uint8_t x, uint8_t y, std::string txt){
    for(int i=0; i<txt.size(); i++){
        uint8_t temp=fontArr[static_cast<size_t>(txt[i])].tileNum;

        tileset->render(x+(16*i)+2, y, temp);
        ili9341_writeCommand(NOOP);
        sleep_ms(250);
    }
}
