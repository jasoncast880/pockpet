#include "display_handler.h"
#include "ampalaya_tileset_16.h"

//display_handler will call the lcd-related tasks

DisplayHandler::DisplayHandler(uint8_t mode) {
    //call the display initializers
    ili9341_initialize(17,20,21,19,18,16);
    
    //needs an initial tileset; assets can provide
    Tileset* tileset = new Tileset(16,&ampalaya_tileset_16[0],40);

    int mapBuf[] = { 1 ,2, 3, 4, 5,67};
    base = new Base(tileset, &mapBuf[0]);
}
//how will i add sprites? how do i access this class without 
//overcomplicating, etc.

// define the globals here..
// PARTIAL SCREEN UPDATES!!!

void lcd_render_task(void* pvParameters) {
    for( ;; ) {
        xSemaphoreTake(xDisplaySemaphore, pdMS_TO_TICKS(67));
        //add a queue here for i.p.c!!!
        //so that program tasks can seamlessly/easily account for position, layering, spr. changes
        //without thinking about underlying api
        
        base.render(); 
        // base will already go through and render all sprites internally
        
        xSemaphoreGive(xDisplaySemaphore);
    }
} 

//....
//IMPORTANT::::
//by this method call, there is already established data in the buffers of this class, all need to do is to print them blocking via spi; all peripheral
//
// no ipc, just read from class objects' members and draw via spi

void lcd_write_task(void* pvParameters) {
    DisplayHandler& display_handler = DisplayHandler::GetInstance();
    //do a initialization for ui, menu, whatever
    
    uint16_t numTiles = (base.tiles_wide*base.tiles_high);

    for( ;; ) {
        //use the DisplayHandler class here; assume mode 0, partial screen render
        //explanation: the base's tile mapguide is what controls which tiles get rendered..
        
        
        uint16_t TIME_MS_TO_TRANSMIT = 20; //change
        xSemaphoreTake(xDisplaySemaphore,pdMS_TO_TICKS(TIME_MS_TO_TRANSMIT));

        uint8_t tile_len = base.tileset->tile_len;
        for( int i = 0; i < (base.tiles_wide * base.tiles_high) ; i++ ){
            if(base.mapGuide[i]!=0) {
                uint32_t x0 = (i%base.tiles_wide)*tile_len;
                uint32_t y0 = (i/base.tiles_wide)*tile_len;
                ili9341_setAddrWindow(x0,y0,tile_len,tile_len);
                ili9341_writeCommand(RAM_WR);
                
                Tile tile = base.tileset->getTilesetData(base.getTilemapData(i));
                uint16_t* buf = tile.buf_ptr;


                ili9341_writeDataBuffer16(buf, tile_len*tile_len);

                base.mapGuide[i] = 0; //now the mapGuide[x] is clean!!! 
            }
            ili9341_writeCommand(NOOP);

        }

        xSemaphoreGive(xDisplaySemaphore);
    }
}
