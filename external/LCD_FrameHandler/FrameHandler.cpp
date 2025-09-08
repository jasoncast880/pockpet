#include "include/FrameHandler.h"

/*
 * @brief: this class will handle all of the buffer rendering *before* the sprites are added. tilemaps & sets used to reduce wram size.
 *
 * */


Tile::Tile(){
    this->tile_len = 16;
    this->buf_ptr = new uint16_t[tile_len*tile_len];
} 
Tile::Tile(int tile_len, uint16_t* buf_ptr){
    this->tile_len = tile_len;
    this->buf_ptr = new uint16_t[tile_len*tile_len];
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
    buf_ptr = new uint16_t[tile_len*tile_len];
    std::memcpy(buf_ptr,copySource.buf_ptr, tile_len*tile_len);
    return *this;
}

void Tile::changePixel(uint16_t index, uint16_t value){
    this->buf_ptr[index]=value;
}

uint16_t Tile::getPixel(uint16_t x, uint16_t y){
    return buf_ptr[x+(y*tile_len)];
}

Tile::~Tile(){
    printf("tile destructor invoked ");
    delete[] buf_ptr;
}

Tileset::Tileset(){}

//for working w raw assets, stored in formatted style
Tileset::Tileset(int tile_len, uint16_t* bufPtr, uint8_t numTiles) { 
    this->bufPtr = bufPtr;
    this->tile_len = tile_len;
    this->numTiles = numTiles;

    //populate the Tile* tileArr
    this->tileArr = new Tile[numTiles];
    for (int i=0;i<numTiles;i++){
        tileArr[i] = Tile(this->tile_len,this->bufPtr+(i*tile_len*tile_len));
    }
}

//for working with established tile arrays, converting tilesets
Tileset::Tileset(Tile* tiles, uint8_t numTiles) { 
    //bufPtr unused 
    this->bufPtr = NULL;
    this->tile_len = tiles[0].tile_len; 
    this->numTiles = numTiles;

    //deepcopy
    this->tileArr = new Tile[numTiles];
    for (int i=0;i<numTiles;i++){
        tileArr[i] = tiles[i]; 
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

Tile* Tileset::getTilesetData(uint8_t tileNum){
    return &tileArr[tileNum]; 
}

void Tileset::setTileData(uint8_t tileNum, Tile* tile){
    tileArr[tileNum] = *tile;
}

Tilemap::Tilemap(){} 

Tilemap::Tilemap(Tileset* tileset, uint8_t* mapBuf, uint8_t tiles_wide, uint8_t tiles_high){
    this->tileset = tileset; //ensure that the tileset's constructor makes space for
                             // extra tiles for les sprites!!!!!
    this->mapBuf = mapBuf;
    //HARDCODED SCREEN DIMS, add err handling just in case later.
    this->x=0;
    this->y=0;
    this->tiles_wide = tiles_wide;
    this->tiles_high = tiles_high;
}

Tile* Tilemap::getTilemapData(uint8_t tileNum){
    printf("Tilemap calls getTilesetData(mapBuf[%d])\n", tileNum);
    return(tileset->getTilesetData(mapBuf[tileNum]));
}

Tilemap::~Tilemap(){
    printf("Tilemap Destructor invoked\n");
}

void Tilemap::alterTile(uint16_t tile_idx, uint8_t newTile) { 
    this->mapBuf[tile_idx] = newTile;
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
    mapGuide = new uint8_t[guideLen]; //dirty versus. clean tiles guide 

    printf("from Base constructor:");
    printf("0x%p\n", &mapGuide[0]);
    /*
    mapGuide[x] = 0: CLEAN : dont do nothing
    mapGuide[x] = 1: DIRTY : replace tile 'x' with tileset->tiles[mapBuf[x]] 
    */
    for(int i=0; i<guideLen; i++){
        mapGuide[i]=1;
    }

    copyTiles = this->tileset; //clean, untouched
    Sprite* spriteArr = new Sprite[10];

}

void Base::render(){
    for(int i = 0; i < 10 ; i++) { //idk how many sprites i got / can have
        spriteArr[i].render();
    }
}

void Base::printMapGuide(){
    //debug function
    printf("Tileguide:\n");
    int j=0;
    for(int i=0; i<guideLen; i++){ 
        printf("%d, ",(*mapGuide+(i)));
        j++;
        if(j==tiles_wide){
            j=0;
            printf("\n");
        }
    }
    printf("EOA\n");
}

Sprite::Sprite(){}

Sprite::Sprite(Base* base, int x, int y, uint8_t tiles_wide, uint8_t tiles_high, Tileset* tileset, uint8_t* mapBuf){

    //assumptions:::::::
    //base, points to tilemap & associated data this sprite is rendered on top of
    this->base = base;

    //positional data
    this->x = x;
    this->y = y;

    //tiles wide, high, dimensions of the rendered sprite
    this->tiles_wide = tiles_wide;
    this->tiles_high = tiles_high;

    this->sprite_size = tiles_wide*tiles_high;

    this->final_tiles_wide = tiles_wide;
    this->final_tiles_high = tiles_high;

    //tileset of sprite;; background-alpha layer is defined in header
    this->tileset = tileset;

    this->mapBuf = mapBuf;
    
    /*
    sleep_ms(2000);
    tileset_validator(tileset,tiles_wide, tiles_high);
    printf("inside sprite(..) : sprite tileset ok\n");

    sleep_ms(2000);
    tileset_validator(base->tileset,6,5);
    printf("inside sprite(..) : pointer to base tileset ok\n");
    */

    getBaseTilemap();
    
    getFinalTileset();

    printf("inside constructor ok\n");
}

Tilemap* Sprite::getBaseTilemap(){
    //for x - width
    if( x < tileset->tile_len && x % tileset->tile_len != 0) {
        final_tiles_wide++;
    } else if ( x > tileset->tile_len && tileset->tile_len % x != 0) {
        final_tiles_wide++;
    }
    
    //for y - width
    if( y < tileset->tile_len && y % tileset->tile_len != 0) {
        final_tiles_high++;
    } else if ( y > tileset->tile_len && tileset->tile_len % y != 0) {
        final_tiles_high++;
    }

    Tile* tiles = new Tile[final_tiles_wide*final_tiles_high];

    for(int i = 0;i<final_tiles_high;i++){
        for(int j = 0;j<final_tiles_wide;j++){
            int idx = base->hashPos((x+j*this->tileset->tile_len),y+i*this->tileset->tile_len);

            Tile* tile = base->getTilemapData(idx);
            tiles[(i*final_tiles_wide)+j] = *tile;
            
            base->mapGuide[idx] = 1;
        }
    }

    Tileset* baseTiles = new Tileset(tiles, sprite_size);
    printf("tileset allocation ok\n");

    printf("deleting tiles tile array:\n");
    delete[] tiles;
    printf("\n");

    Tilemap* baseTilemap = new Tilemap(baseTiles, NULL,final_tiles_wide,final_tiles_high);

    return baseTilemap;
    //tileset_validator(baseTiles, tiles_wide, tiles_high); //OK
}


struct Sprite::hashedPix {
    //position in context of a single tile
    int x_pix, y_pix;
};


Tileset* Sprite::getFinalTileset(){

    Tilemap* base_tilemap = getBaseTilemap();

    //extract the pixels from the tileset
    size_t buf_len = tiles_high*tiles_wide*(tileset->tile_len*tileset->tile_len);
    uint16_t* pix_buf = new uint16_t[buf_len];

    // //can move this portion to tileset class later 
    uint16_t counter;
    for( int i = 0 ; i < tiles_high ; i++ ) {
        for(int a = 0; a < tileset->tile_len ; a++ ) {
            for(int j = 0; j < tiles_wide ; j++){
                int curr_idx = hashPos(j*tileset->tile_len,i*tileset->tile_len);
                Tile* tile = this->getTilemapData(curr_idx);
                for( int b ; b < tileset->tile_len ; b++ ) {
                    pix_buf[counter] = tile->getPixel(a,b);
                    counter++;
                }
            }
        }
    }
    // now i have pix_buf

    //now loop through each tile and update base_tiles to filter on all thhe pixels..
    uint16_t x_offset = (x+tileset->tile_len)%tileset->tile_len;
    uint16_t y_offset = (y+tileset->tile_len)%tileset->tile_len;
    //map this onto the base tiles
    for(int i = 0 ; i < buf_len ; i++) {
        if(pix_buf[i]!=ALPHA_CLR_565){
            uint16_t n = base_tilemap->hashPos(i+x_offset,
                    i/((this->tiles_wide)*(tileset->tile_len))); //index in tile arr
            
            int x = (x_offset+i)+tileset->tile_len%tileset->tile_len;
            int y = (i/((this->tiles_wide)*(tileset->tile_len)))+tileset->tile_len%tileset->tile_len;
            uint16_t idx = x+y*tileset->tile_len; //index in the tile's buffer

            base_tilemap->tileset->tileArr[n].changePixel(idx,pix_buf[i]);
        }
    }

    Tileset* final_tileset = base_tilemap->tileset;
    return final_tileset;
}

uint8_t Tilemap::hashPos(int x_pix, int y_pix){
    return ((x_pix/this->tileset->tile_len)+(y_pix/this->tileset->tile_len)*(this->tiles_wide));
}

Sprite::~Sprite(){
    //prob gonna have to delete a lot more of dynamically allocated mem
    delete this->finishedTiles;

    for(int i = 0;i<final_tiles_high;i++){
        for(int j = 0;j<final_tiles_wide;j++){
            int idx = base->hashPos((x+j*this->tileset->tile_len),y+i*this->tileset->tile_len);

            base->tileset->setTileData(idx,base->copyTiles->getTilesetData(idx));
            base->mapGuide[idx] = 1;
        }
    }
} 

//try: frame data renders, sends its tiles to base. base uses linked list to update clean to dirty, update sprited tiles to dirty.
//
//make dirty, or remove from dirty if changing position!
void Sprite::render() { //only relevant subsequent to instantiation; 
                        //on pos/tiles update use the other 3

    this->finishedTiles = getFinalTileset();
    
    for(int i = 0;i<final_tiles_high;i++){
        for(int j = 0;j<final_tiles_wide;j++){
            int idx = base->hashPos((x+j*this->tileset->tile_len),y+i*this->tileset->tile_len);

            base->tileset->setTileData(idx,finishedTiles->getTilesetData(i+j*final_tiles_wide));
            base->mapGuide[idx] = 1;
        }
    }

}

void Sprite::render(uint8_t* spriteMapBuf){ 
    this->mapBuf = spriteMapBuf;
    delete finishedTiles;

    this->finishedTiles = getFinalTileset(); //revise this method

    for(int i = 0;i<final_tiles_high;i++){
        for(int j = 0;j<final_tiles_wide;j++){
            int idx = base->hashPos((x+j*this->tileset->tile_len),y+i*this->tileset->tile_len);

            base->tileset->setTileData(idx,finishedTiles->getTilesetData(i+j*final_tiles_wide));
            base->mapGuide[idx] = 1;
        }
    }
}

void Sprite::render(uint16_t x, uint16_t y){

    //clean the tiles on base that it used to inhabit
    for(int i = 0;i<final_tiles_high;i++){
        for(int j = 0;j<final_tiles_wide;j++){
            int idx = base->hashPos((x+j*this->tileset->tile_len),y+i*this->tileset->tile_len);

            base->tileset->setTileData(idx, base->copyTiles->getTilesetData(idx));
            base->mapGuide[idx] = 1;
        }
    }

    this->x = x;
    this->y = y;
    delete finishedTiles;

    this->finishedTiles = getFinalTileset();

    for(int i = 0;i<final_tiles_high;i++){
        for(int j = 0;j<final_tiles_wide;j++){
            int idx = base->hashPos((x+j*this->tileset->tile_len),y+i*this->tileset->tile_len);

            base->tileset->setTileData(idx,finishedTiles->getTilesetData(i+j*final_tiles_wide));
            base->mapGuide[idx] = 1;
        }
    }
}

//convenience mash of the two
void Sprite::render(uint16_t x, uint16_t y, uint8_t* spriteMapBuf){
    //
    //clean the tiles on base that it used to inhabit
    for(int i = 0;i<final_tiles_high;i++){
        for(int j = 0;j<final_tiles_wide;j++){
            int idx = base->hashPos((x+j*this->tileset->tile_len),y+i*this->tileset->tile_len);

            base->tileset->setTileData(idx, base->copyTiles->getTilesetData(idx));
            base->mapGuide[idx] = 1;
        }
    }

    this->x = x;
    this->y = y;
    this->mapBuf = spriteMapBuf;

    delete finishedTiles;

    this->finishedTiles = getFinalTileset();

    for(int i = 0;i<final_tiles_high;i++){
        for(int j = 0;j<final_tiles_wide;j++){
            int idx = base->hashPos((x+j*this->tileset->tile_len),y+i*this->tileset->tile_len);

            base->tileset->setTileData(idx,finishedTiles->getTilesetData(i+j*final_tiles_wide));
            base->mapGuide[idx] = 1;
        }
    }
}
