#include "display_handler.h"
#include "pinout.h"

#include "ampalaya_tileset_16.h"
#include "tilemaps.h"
#include <stdexcept>
#include <vector>

//globals

    Tileset* sys_tileset;
    Tileset* jet_tileset;

    Scene* base;
    Sprite* jetsprite;
    RenderController* render;

    uint8_t* maps[4] = {&demo_spritemap_1[0], &demo_spritemap_2[0], &demo_spritemap_3[0], &demo_spritemap_4[0]};

void display_setup(){ //initialize tasks from here? perchance
    ili9341_initialize(ILI9341_CS,ILI9341_RST,ILI9341_DC); 
    
    sys_tileset = new Tileset(16, (uint16_t*)&ampalaya_tileset_16[0], 30);
    jet_tileset = new Tileset(16, (uint16_t*)&jet_tileset[0], 16);

    base = new Scene(*sys_tileset, &tile_bg_16[0]);
    jetsprite = new Sprite(30,30,2,2,*jet_tileset, maps[0]);

    render = new RenderController(*base);

    //configAssert???
    xDisplayHandlerQueue = xQueueCreate( (UBaseType_t)10, (UBaseType_t)2 );
    if (xDisplayHandlerQueue == NULL) {
        printf("Failed to create display queue!\n");
        while(1);  // trap here for debugging
    }

    xDisplaySemaphore = xSemaphoreCreateMutex();

    xTaskCreate( lcd_write_task, "lcd_write_task", 2000, NULL, 2, NULL );
    xTaskCreate( lcd_render_task, "lcd_render_task", 2000, NULL, 2, NULL );

    printf("display_setup OK\n");

    //do a test of tile/data integrity here. 
    for( int i = 0; i < 15 ; i++ ){
        for( int j = 0; j < 20 ; j++ ){ //rember tl-br
        uint32_t x0 = j*16;
        uint32_t y0 = i*16;

        Tile* tile = base->getTilemapData(i*20+j);
        uint16_t* buf = tile->getBuf();
        ili9341_setAddrWindow(x0,y0,16,16);
        ili9341_writeCommand(RAM_WR);
        ili9341_writeDataBuffer16(buf, 16*16);
        ili9341_writeCommand(NOOP);
        }
    }
}

void lcd_render_task(void* pvParameters) {
    RenderController& r = *render;
    Sprite& cursor = *jetsprite;

    r.render();
    r.sprite_add(cursor);

    for( ;; ) {
        xSemaphoreTake(xDisplaySemaphore, pdMS_TO_TICKS(100));

        printf("start render \n");
        uint16_t x = 50;
        uint16_t y = 50;
        for(int i = 0; i<30; i++) {
            for(int j = 0; j<4 ; j++) {
                r.sprite_update(cursor.getID(), x+i, y+i, maps[j]);  
            }
        } 
        xSemaphoreGive(xDisplaySemaphore);
    }
}

void lcd_write_task(void* pvParameters) {
    RenderController& r = *render;

    //reserve a max screen buffer space for writes? for now
    uint16_t* screenBuf = new uint16_t[240*320];
    uint16_t numTiles = (r.base->tiles_high*r.base->tiles_wide);

    xSemaphoreTake(xDisplaySemaphore,pdMS_TO_TICKS(20));
    for( int i = 0; i < (numTiles) ; i++ ){
            uint32_t x0 = (i%r.base->tiles_wide)*16; 
            uint32_t y0 = (i/r.base->tiles_wide)*16;
            ili9341_setAddrWindow(x0,y0,16,16);
            ili9341_writeCommand(RAM_WR);
            
            Tile* tile = r.base->getTilemapData(i);
            uint16_t* buf = tile->getBuf();

            ili9341_writeDataBuffer16(buf, 16*16);

            ili9341_writeCommand(NOOP);
    }
    xSemaphoreGive(xDisplaySemaphore);

    for( ;; ) {
        uint16_t TIME_MS_TO_TRANSMIT = 20; //change
        xSemaphoreTake(xDisplaySemaphore,pdMS_TO_TICKS(TIME_MS_TO_TRANSMIT));

        for( int i = 0; i < (r.renderedTiles.size()) ; i++ ){
            uint32_t x0 = ((r.indexList.at(i))%r.base->tiles_wide)*16; 
            uint32_t y0 = (r.indexList.at(i)/r.base->tiles_wide)*16;
            ili9341_setAddrWindow(x0,y0,16,16);
            ili9341_writeCommand(RAM_WR);
            
            Tile& tile = r.renderedTiles.at(i);
            uint16_t* buf = tile.getBuf();

            ili9341_writeDataBuffer16(buf, 16*16);

            ili9341_writeCommand(NOOP);
        }

        xSemaphoreGive(xDisplaySemaphore);
    }
}
