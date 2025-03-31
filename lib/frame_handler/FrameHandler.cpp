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

Tile::Tile(){
    this->tile_len = 16;
    this->buf_ptr = new uint8_t[tile_len*tile_len];
} 
Tile::Tile(int tile_len, uint8_t* buf_ptr){
    this->tile_len = tile_len;
    this->buf_ptr = new uint8_t[tile_len*tile_len];
    for(int i = 0; i<tile_len*tile_len;i++){
        this->buf_ptr[i] = buf_ptr[i];
    }
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

void Tile::changePixel(uint16_t index, uint8_t color){
    this->buf_ptr[index]=color;
}

uint8_t Tile::getPixel(uint16_t x, uint16_t y){
    return buf_ptr[x+(y*tile_len)];
}

Tile::~Tile(){
    printf("tile destructor invoked ");
    delete[] buf_ptr;
}

Tileset::Tileset(){}

Tileset::Tileset(int tile_len, uint8_t* bufPtr, uint8_t numTiles) {
    this->bufPtr = bufPtr;

    this->tile_len = tile_len;
    this->numTiles = numTiles;

    //populate the Tile* tileArr
    this->tileArr = new Tile[numTiles];
    for (int i=0;i<numTiles;i++){
        tileArr[i] = Tile(tile_len,(bufPtr+(i*tile_len*tile_len)));
    }
}

Tileset::Tileset(Tile* tiles, uint8_t numTiles) { 
    //bufPtr unused 
    this->tile_len = tiles[0].tile_len; 
    this->numTiles = numTiles;

    //deepcopy
    this->tileArr = new Tile[numTiles];
    for (int i=0;i<numTiles;i++){
        tileArr[i] = tiles[i]; //call on tile ass. op
                               //tiles is safe to delete
    }
}


Tileset::Tileset(const Tileset& copySource){
    this->tile_len = copySource.tile_len;
    this->numTiles = copySource.numTiles;

    tileArr = new Tile[numTiles];
    for(int i = 0; i<numTiles; i++){
        tileArr[i] = copySource.tileArr[i];
    }
}

Tileset& Tileset::operator=(const Tileset& copySource){
    if((this!=&copySource) && (copySource.bufPtr!=NULL)){
        if (bufPtr!=NULL) 
            delete[] bufPtr;
        if (tileArr!=NULL) 
            delete[] tileArr;

        //ensure a deep copy by allocating buffer space

        tile_len = copySource.tile_len;
        numTiles = copySource.numTiles;

        tileArr = new Tile[numTiles];
        for(int i = 0; i<numTiles; i++){
            tileArr[i] = copySource.tileArr[i];
        }
    }
   
    return *this;
}

Tileset::~Tileset(){
    printf("tileset destructor invoked\n");
    delete[] tileArr;
}

Tile* Tileset::getTileData(uint8_t tileNum){
    return &tileArr[tileNum]; 
}

void Tileset::setTileData(uint8_t tileNum, Tile* tile){
    tileArr[tileNum] = *tile; //trigger Tile assignment op??
}

void Tileset::render(uint16_t x, uint16_t y, uint8_t tileNum) {
    tileArr[tileNum].render(x,y);
}

Tilemap::Tilemap(){
    printf("Tilemap def constructor invoked");
} //default constructor (not used)

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

Tile* Tilemap::getTileData(uint8_t tileNum){
    printf("Tilemap calls getTileData(%d)\n", tileNum);
    return(tileset->getTileData(mapBuf[tileNum]));
}

Tilemap::~Tilemap(){
    printf("Tilemap Destructor invoked\n");
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

Sprite::Sprite(){}

Sprite::Sprite(Base* basePtr, int x, int y, uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf){

    //assumptions:::::::
    //basePtr, points to tilemap & associated data this sprite is rendered on top of
    this->basePtr = basePtr;

    //positional data
    this->x = x;
    this->y = y;

    //tiles wide, high, dimensions of the rendered sprite
    this->tiles_wide = tiles_wide;
    this->tiles_high = tiles_high;
    this->sprite_size = tiles_wide*tiles_high;

    //tileset of sprite;; should have the pink alpha filter 
    this->tileset = tileset;

    //tilemap data, can be stored in an array in assets, (may change later)
    this->mapBuf = mapBuf;
    
    /*
    sleep_ms(2000);
    tileset_validator(tileset,tiles_wide, tiles_high);
    printf("inside sprite(..) : sprite tileset ok\n");

    sleep_ms(2000);
    tileset_validator(basePtr->tileset,6,5);
    printf("inside sprite(..) : pointer to base tileset ok\n");
    */

    getBaseTileset();
    
    getFinalTileset();

    printf("inside constructor ok\n");
}

//if yes move position, no change sprite
void Sprite::setPosition(uint16_t x, uint16_t y){ 
    this->x = x;
    this->y = y;

    getBaseTileset();
    getFinalTileset();
}


void Sprite::getBaseTileset(){
    //0x00:no move
    //0x01:x move
    //0x02:y move
    //0x03:x&y move
    //
    //
    //for now, assume no edge cases (no buffer overflow)

    //for x - width
    if(x>tileset->tile_len){
        if(x%tileset->tile_len!=0){
            //tiles_wide += 1;
            statusFlag |= 0b0001;
        } 
    } else if (tileset->tile_len>x){
        if(tileset->tile_len>x!=0){
            //tiles_wide += 1;
            statusFlag |= 0b0001;
        }
    }

    //for y - height
    if(y>tileset->tile_len){
        if(y%tileset->tile_len!=0){
            //tiles_high += 1;
            statusFlag |= 0b0010;
        } 
    } else if (tileset->tile_len>y){
        if(tileset->tile_len>y!=0){
            //tiles_high += 1;
            statusFlag |= 0b0010;
        }
    }

    printf("getDims width: %d, height: %d, hexcode: 0x%02X\n", tiles_wide, tiles_high, statusFlag);

    Tile* tiles = new Tile[sprite_size];

    //*copy* the tiles from the base tileset and prep them to be modified
    if(this->statusFlag==0x01){ //dx
        for(int i = 0;i<tiles_high;i++){
            for(int j = 0;j<tiles_wide;j++){
                printf("try %d: on base tilemap ", (i*tiles_wide+j));
                uint8_t ans = hashPos(x+j*tileset->tile_len,y+i*tileset->tile_len);
                Tile* cursor_tile = basePtr->getTileData(ans);
                Tile* cursor_tile_2 = basePtr->getTileData(ans+1);

                //here condition check based on status register (fornow assume sf = 0x02)
                //sf=0x02 implies that tile2 is the tile directly above it.
                //
                uint8_t diff = (x>tileset->tile_len)?x%tileset->tile_len:tileset->tile_len%x;
                Tile* proc_tile = spliceX(cursor_tile, cursor_tile_2, diff); 

                //proc_tile->render(x+j*tileset->tile_len, y+i*tileset->tile_len); //check
                tiles[i*tiles_wide+j] = *proc_tile;
                printf("n.%d\n", ans);
                delete proc_tile;
            }
        }
    } else if(this->statusFlag==0x02){ //dy
        for(int i = 0;i<tiles_high;i++){
            for(int j = 0;j<tiles_wide;j++){
                printf("try %d: on base tilemap ", (i*tiles_wide+j));
                uint8_t ans = hashPos(x+j*tileset->tile_len,y+i*tileset->tile_len);
                Tile* cursor_tile = basePtr->getTileData(ans);
                Tile* cursor_tile_2 = basePtr->getTileData(ans+basePtr->tiles_wide);

                //here condition check based on status register (fornow assume sf = 0x02)
                //sf=0x02 implies that tile2 is the tile directly above it.
                //
                uint8_t diff = (y>tileset->tile_len)?y%tileset->tile_len:tileset->tile_len%y;
                Tile* proc_tile = spliceY(cursor_tile, cursor_tile_2, diff); 

                //proc_tile->render(x+j*tileset->tile_len, y+i*tileset->tile_len); //check
                tiles[i*tiles_wide+j] = *proc_tile;
                printf("n.%d\n", ans);
            } //add other sf cases
        }
    } else if(this->statusFlag==0x00){ //no change
        printf("splicing skipped\n"); 
    } else{ printf("ERROR: 0x03 sf not supported\n"); }
    printf("splice ok\n");

    baseTiles = new Tileset(tiles, sprite_size);
    printf("tileset allocation ok\n");

    printf("deleting tiles tile array:\n");
    delete[] tiles;
    printf("\n");

    //tileset_validator(baseTiles, tiles_wide, tiles_high); //OK

}

void Sprite::getFinalTileset(){
    Tile* final_tiles = new Tile[sprite_size];

    for(int i = 0;i<tiles_high;i++){
        for(int j = 0;j<tiles_wide;j++){
            
            Tile* base_tile = baseTiles->getTileData(i*tiles_wide+j);
            printf("got base tile ");
            Tile* sprite_tile = tileset->getTileData(mapBuf[i*tiles_wide+j]);
            printf("got spr tile \n");

            Tile* proc_tile = merge(base_tile, sprite_tile); 
            printf("merge ok\n");

            /*
            proc_tile->render(x+j*tileset->tile_len, y+i*tileset->tile_len); //check
            sleep_ms(100);
            */
            final_tiles[i*tiles_wide+j] = *proc_tile;
            delete proc_tile;
        } 
    }

    finishedTiles = new Tileset(final_tiles, sprite_size);

    printf("deleting final_tiles tile array:\n");
    delete[] final_tiles;
    printf("\n");
    
    printf("final tileset constructed\n");
    //renderable from here
}

uint8_t Sprite::hashPos(int x_pix, int y_pix){
    return ((x_pix/tileset->tile_len)+(y_pix/tileset->tile_len)*(basePtr->tiles_wide));
    //does not account for edge cases where the sprite goes over the top-right bounds
}

Sprite::~Sprite(){
    //delete bufPtr;

    for(int i=0;i<basePtr->tiles_high;i++){
        for(int j=0;j<basePtr->tiles_wide;j++){
            basePtr->mapGuidePtr[i*tiles_wide+j]=0; //reset the things on base guide
        }
    }
} 

Tile* Sprite::spliceX(Tile* tile1, Tile* tile2, uint8_t cutoff){
    int tile_len = tile1->tile_len;
    uint8_t* tileBuf = new uint8_t[(tile_len)*(tile_len)];
    for(int i = 0;i<tile_len;i++){
        int jt=0;
        for(int j1 = 0; j1<cutoff;j1++){
            tileBuf[i*tile_len+jt]=tile1->getPixel(((tile_len-cutoff)+j1),i);
            jt++;
        }
        for(int j2 = 0; j2<(tile_len-cutoff);j2++){
            tileBuf[i*tile_len+jt]=tile2->getPixel((j2),i);
            jt++;
        }
    }

    Tile* ansTile = new Tile(tile_len, tileBuf);
    delete[] tileBuf;

    return ansTile;
}

Tile* Sprite::spliceY(Tile* tile1, Tile* tile2, uint8_t cutoff){ //means a y offset
    int tile_len = tile1->tile_len;
    uint8_t* tileBuf = new uint8_t[(tile_len)*(tile_len)];

    for(int i = 0;i<cutoff;i++){
        for(int j=0;j<tile_len;j++){
            tileBuf[i*tile_len+j]=tile1->getPixel(j,(tile_len-cutoff+i));
        }
    }

    for(int i=0;i<(tile_len-cutoff);i++){
        for(int j=0;j<tile_len;j++){
            //tile2 things
            tileBuf[(tile_len*cutoff)+i*tile_len+j] = tile2->getPixel(j,i);
        }
    }

    Tile* ansTile = new Tile(tile_len, tileBuf);
    delete[] tileBuf;

    return ansTile;
}

Tile* Sprite::merge(Tile* baseTile, Tile* spriteTile){
    int tile_len = baseTile->tile_len;
    uint8_t* tileBuf = new uint8_t[(tile_len)*(tile_len)];

    for(int i = 0;i<tile_len;i++){
        for(int j=0;j<tile_len;j++){
            if(spriteTile->getPixel(j,i)==255){
                tileBuf[i*tile_len+j]=baseTile->getPixel(j,i);
            } else {
                tileBuf[i*tile_len+j]=spriteTile->getPixel(j,i);
            }
        }
    }

    Tile* ansTile = new Tile(tile_len, tileBuf);
    delete[] tileBuf;

    return ansTile;
}

void Sprite::tileset_validator(Tileset* tiles, int w, int h){
    for(int i=0; i<h; i++){
        for(int j=0; j<w;j++){
            tiles->render(tiles->tile_len*j,tiles->tile_len*i,i*w+j);
        }
    }
}

void Sprite::render(){
    for(int i=0; i<tiles_high; i++){
        for(int j=0; j<tiles_wide;j++){
            finishedTiles->render(
                    x+tileset->tile_len*j,
                    y+tileset->tile_len*i,
                    i*tiles_wide+j);
        }
    }
}

void Sprite::render(uint8_t* spriteMapBuf){ //helper functions for the api
    this->mapBuf = spriteMapBuf;
    delete finishedTiles;

    getFinalTileset();

    for(int i=0; i<tiles_high; i++){
        for(int j=0; j<tiles_wide;j++){
            finishedTiles->render(
                    x+tileset->tile_len*j,
                    y+tileset->tile_len*i,
                    i*tiles_wide+j);
        }
    }
}

void Sprite::render(uint16_t x, uint16_t y){
    this->x = x;
    this->y = y;
    delete baseTiles;
    delete finishedTiles;

    getBaseTileset();
    getFinalTileset();

    for(int i=0; i<tiles_high; i++){
        for(int j=0; j<tiles_wide;j++){
            finishedTiles->render(
                    x+tileset->tile_len*j,
                    y+tileset->tile_len*i,
                    i*tiles_wide+j);
        }
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
