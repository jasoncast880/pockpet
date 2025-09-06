#include "display_handler.h"
#include "pinout.h"

#include "ampalaya_tileset_16.h"
#include "tilemaps.h"

//display_handler will call the lcd-related tasks


void display_setup(){
    //call the display initializers
    ili9341_initialize(SPI0_CS,ILI9341_RST,ILI9341_DC,SPI0_TX,SPI0_SCLK,SPI0_RX); 
    
    //use legitimate sprites, tilesets
    Tileset* tileset = new Tileset(16,(uint16_t*)&ampalaya_tileset_16[0],30);

    base = Base(tileset, (uint8_t*)&tile_bg_16[0]);
    //fix up

    xDisplayHandlerQueue = xQueueCreate( (UBaseType_t)10, (UBaseType_t)2 );
    if (xDisplayHandlerQueue == NULL) {
        printf("Failed to create display queue!\n");
        while(1);  // trap here for debugging
    }

    printf("display_setup OK\n");
}

void lcd_render_task(void* pvParameters) {
    for( ;; ) {
        xSemaphoreTake(xDisplaySemaphore, pdMS_TO_TICKS(100));
        printf("start render \n");

        spriteInfo recv;
        for( int i = 0; i<10; i++ ) {
            xQueueReceive( xDisplayHandlerQueue, &recv, (TickType_t)0 );
            if(recv.sprite == NULL){
                //check for recv matches sprites[x]
                for( int i = 0;i<10; i++ ) {
                    if(&sprites[i] == recv.sprite){
                        if( ( sprites[i].x!=recv.x || sprites[i].y!=recv.y && sprites[i].mapBuf==recv.tilemap)) { 
                            sprites[i].render(recv.x,recv.y);
                            break;
                        } else if( ( sprites[i].x==recv.x && sprites[i].y==recv.y) && sprites[i].mapBuf!=recv.tilemap) { 
                            sprites[i].render(recv.tilemap);
                            break;
                        } else {
                            sprites[i].render(recv.x,recv.y,recv.tilemap);
                            break;
                        }
                    }
                }
            } else {}
            
            //reset recv.
            recv.sprite = NULL; //implied rest of fields are junk value
        }

        //renders all sprites
        base.render(); 
        
        xSemaphoreGive(xDisplaySemaphore);
    
    }
} 

void lcd_write_task(void* pvParameters) {
    //do a initialization for ui, menu, whatever
    
    uint16_t numTiles = (base.tiles_wide*base.tiles_high);

    for( ;; ) {
        
        uint16_t TIME_MS_TO_TRANSMIT = 20; //change
        xSemaphoreTake(xDisplaySemaphore,pdMS_TO_TICKS(TIME_MS_TO_TRANSMIT));

        printf("start write \n");

        uint8_t tile_len = base.tileset->tile_len;
        for( int i = 0; i < (base.tiles_wide * base.tiles_high) ; i++ ){
            if(base.mapGuide[i]!=0) {
                uint32_t x0 = (i%base.tiles_wide)*tile_len;
                uint32_t y0 = (i/base.tiles_wide)*tile_len;
                ili9341_setAddrWindow(x0,y0,tile_len,tile_len);
                ili9341_writeCommand(RAM_WR);
                
                Tile* tile = base.getTilemapData(i);
                uint16_t* buf = tile->buf_ptr;

                ili9341_writeDataBuffer16(buf, tile_len*tile_len);

                base.mapGuide[i] = 0; //now the mapGuide[x] is clean!!! 
            }
            ili9341_writeCommand(NOOP);
        }

        xSemaphoreGive(xDisplaySemaphore);
    }
}
