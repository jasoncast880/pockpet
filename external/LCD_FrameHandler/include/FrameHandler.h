#ifndef FRAMEHANDLER_H
#define FRAMEHANDLER_H

#include <stdexcept>
#include <cstring>
#include <string>
#include <queue>
#include <memory>
#include <vector>

#define ALPHA_CLR_565 0xF81F //a 565 magenta color
#define DEFAULT_TILE_LEN 16

struct Tile { //implement assuming indexed color
    std::unique_ptr<uint16_t[]> buf;
public:
    uint8_t tile_len = DEFAULT_TILE_LEN;

    Tile();                             //default 16x16
    Tile(int tile_len, uint16_t* srcBuf);  

    Tile(const Tile& copySrc) noexcept;
    Tile& operator=(const Tile& copySrc) noexcept; 

    Tile(Tile&&) noexcept = default;
    Tile& operator=(Tile&& moveSrc) noexcept = default;

    void changePixel(uint16_t index, uint16_t value); 
    uint16_t getPixel(uint16_t x,uint16_t y) const; 
    uint16_t* getBuf() const;
                                             
    ~Tile() = default; //allow for automatic memory free via smart pointer
};

struct Tileset{
private:
    std::unique_ptr<Tile[]> tiles;
public:
    uint8_t tile_len = DEFAULT_TILE_LEN;
    uint8_t numTiles;

		Tileset();
    Tileset(Tile* tiles, uint8_t numTiles); 
    Tileset(uint8_t tile_len, uint16_t* bufPtr, uint8_t numTiles); 

    Tileset(const Tileset& copySrc) noexcept;
    Tileset& operator=(const Tileset& copySrc) noexcept;

    Tileset(Tileset&&) noexcept = default;
    Tileset& operator=(Tileset&&) noexcept = default;

    Tile& getTilesetData(uint8_t tileNum);
    void setTileData(uint8_t tileNum, Tile* tile);
		Tile* getTiles() const;

    ~Tileset() = default;
};

/* maps to the frame, so it should be (assuming 16 pix tilemaps)
 *  .____.
 *  |    | 240:15 tiles
 *  o____.
 *   320:20 tiles
 *
 *  IF using 8 pix tilemap: 40 tiles by 30 tiles
 */ 

struct Tilemap{ 
    std::unique_ptr<uint8_t[]> map; //map tileset num. to tilespace
    uint16_t x, y;
    Tileset* tileset;
    uint8_t tiles_wide, tiles_high;

    Tilemap() = default;
    Tilemap(Tileset& tileset, uint8_t* mapBuf,uint8_t tiles_wide, uint8_t tiles_high); 
    Tilemap(int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset& tileset, uint8_t* mapBuf);

    Tile* getTilemapData(uint16_t tileNum);
    void setTilemap(uint16_t tileNum, uint16_t newTile);
}; 

class Scene: public Tilemap{ 
public:
    Scene(Tileset& tileset, uint8_t* mapBuf); 
};

class Sprite: public Tilemap{
    uint8_t sprite_id = 0;
public:

    Sprite(int x,int y,uint8_t tiles_wide, uint8_t tiles_high, Tileset& tileset, uint8_t* mapBuf);

    Sprite(const Sprite& other);
    Sprite& operator=(const Sprite& other);

    Sprite(Sprite&&) noexcept = default;
    Sprite& operator=(Sprite&&) noexcept = default;

    void setPosition(uint16_t x, uint16_t y);
    void setMap(uint8_t* mapBuf); //change the entire map
    uint8_t getID();
    void setID(uint8_t id);

    //positional context vars
    std::vector<uint16_t> map_vec; //tilemap tiles the sprite superimposes
    uint8_t x_offset = 0; //+ve offset from tile bounds
    uint8_t y_offset = 0; //+ve offset from tile bounds
                                //top and left most bounds
    void setMapVector_Offsets(Tilemap& tilemap); 
    void resetMapVector_Offsets();

    ~Sprite()=default;
};


class RenderController { //masher
     /*
    mapGuide[x] = 0: CLEAN : dont do nothing
    mapGuide[x] = 1: DIRTY : replace tile 'x' with tileset->tiles[mapBuf[x]] 
      * IMPORTANT!!!!
      * mapGuide[x] = 0 should only be called if the class 
      * user has drawn base->tileset->tileArr[x]::
      * 
    */

    uint8_t hashPos(int x_pix,int y_pix); 
    Tilemap* getBaseTilemap();  //change dat //change dat

    void writeTilesBuf(Tile& tile);

    //tempBufs for storing tileData for mashing. reset every
    //render loop iteration
    std::vector<uint16_t> spriteBuf;
    std::vector<uint16_t> baseBuf;
public:
    Scene* base;
    uint8_t* mapGuide; // helps keeps count of dirty, clean tiles
    size_t guideLen;//number of elements in mapGuide&mapBuf 

    std::vector<Sprite> sprites; 

    //resize together
    std::vector<Tile> renderedTiles;
    std::vector<uint8_t> indexList;

    void sprite_render(uint8_t id);
    RenderController(Scene& base);

		//for exposing to dislay handler SOFTWARE
    void render(); 
    void sprite_add(Sprite& sprite);
    void sprite_update(int spriteid, uint8_t* spriteMapBuf); 
    void sprite_update(int spriteid, uint16_t x, uint16_t y); 
    void sprite_update(int spriteid, uint16_t x, uint16_t y,uint8_t* spriteMapBuf);

		struct display_msg_t { 
			std::vector<Tile> *renderedTiles;
			std::vector<uint8_t> *indexList;
			size_t size;
		}
		display_msg_t giveTiles(size_t s); //for exposing to the display handler hardware; for passing to queue
};

#endif
