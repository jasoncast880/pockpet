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

Tile& Tile::operator=(const Tile& copySource){
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
    uint8_t* mapGuide = new uint8_t[guideLen]; //i will not release you.
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

    size_t len = (tileset->tile_len*tiles_wide)*(tileset->tile_len*tiles_high);
    //BUFPTR needs to be contiguous so that it can be handles piecewise
    bufPtr = new uint8_t[len]; 

    int counter = 0;
    int counterOffset = 0;
    int bufPtrOffset = 0;
    int tilesetOffset = 0; //poorly named vars...
    
    for(int i = 0; i<(tileset->tile_len*tiles_high); i++){ //all the rows processed in the array
        for(int j = 0; j<(tiles_wide); j++){ //loop through the entire row of a sprite
            for(int k = 0; k<(tileset->tile_len); k++){ //loop through a row of a tile
                bufPtr[bufPtrOffset+k] = *(tileset->getTileData(counter)+(tilesetOffset+k));
            }
            //increment the buffer offsets here
            counter++;
            bufPtrOffset+=tileset->tile_len;
            tilesetOffset+=(tileset->tile_len)*(i%tileset->tile_len);
        }
        if(counter%tiles_wide){
            counter-=tiles_wide;
        } 
    } //this array must be tested!!!

    //this should make bufPtr viable for use

    calcTileIndeces(); //define the appropriate base tilemap indeces
    spriteMask(); //make the appropriate masked tiles based on the sectors ...
                  
    //polymorphism: Sprite::render() will alter the tileset of Base, 
    //swap temp and original, use a identical queue in order to keep track
    //on destructor call, replace all of the things somehow...

}

Sprite::~Sprite(){} //will really need this LMAO

void Sprite::printBufPtr(){
    //testing here
    ili9341_setAddrWindow(10,10,(tileset->tile_len*tiles_wide),(tileset->tile_len*tiles_high));
    for(int i = 0; i<(tileset->tile_len*tiles_wide); i++){
        ili9341_writeColorByIndex(bufPtr[i]);
    }
    ili9341_writeData(NOOP);
}
void Sprite::calcTileIndeces(){ //ok?

    uint8_t width, height; //WIDTH HEIGHT of the blocks that sprite takes up on base
    if(!((tileset->tile_len+x)%(tileset->tile_len))){
        width = tiles_wide;
    } else{
        width = tiles_wide+1;
    }

    if(!((tileset->tile_len+y)%(tileset->tile_len))){
        height = tiles_high;
    } else{
        height = tiles_high+1;
    }

    tileIndeces = new uint8_t[width*height];

    //gives you the indeces of the tiles you must change on mapGuide, base tilemap
    uint8_t basePtrHeight = basePtr->tiles_high;
    uint8_t basePtrWidth = basePtr->tiles_wide;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            tileIndeces[(i*width)+j] = (y/tileset->tile_len + i)*basePtrWidth+(x/tileset->tile_len + j);
        }
    }
    //caveats: only works for one sprite, NO collisions allowed
}

void Sprite::spriteMask(){
    //loop through the 'rows' of bufPtr, until there are no more data in buf
    //as you increment through the necessary offsets, you can det. which tile to
    //alter, switching as you go
    
    //make a copy of the right tiles based on calcTileIndeces in heap
    //
    //alter them accordingly
    //?send the tiles to base, call base functions to alter tilemap?
}

void Sprite::render(){ //need to account for alpha processing...
    //sprite render should just be a way to reinitialize/change positions/tiles
    int counter=0;
    //'blindly' render; IF there is alpha processing or a sprite isn't rendered 
    //on a tile, then update the tile guide so base object can re-render 
    //during the rendering loop;
    
    for(int i = 0; i<tiles_high; i++){
        for(int j = 0; j<tiles_wide; j++){
               tileset->render((x+j*16),(y+i*16),mapBuf[counter]);
        }
    }
    basePtr->printMapGuide();
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
