#include <cstddef>
#include <hardware/spi.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "ili9341.h"
#include "FrameHandler.h"

#include <ampalaya_tileset_16.h>
#include <tilemaps.h>

void drawTilemap(Scene* p) {
    for(int i = 0; i < p->tiles_high; i++){
        for(int j = 0; j < p->tiles_wide; j++){
            int tile_idx = i*p->tiles_wide+j;
            printf("draw tilemap tile no. %d\n", tile_idx);
            Tile* tile_ = p->getTilemapData(tile_idx);
            uint16_t* buf = tile_->getBuf();

            ili9341_setAddrWindow(j*DEFAULT_TILE_LEN,i*DEFAULT_TILE_LEN,16,16);
            ili9341_writeCommand(RAM_WR);
            ili9341_writeDataBuffer16( buf, DEFAULT_TILE_LEN*DEFAULT_TILE_LEN );
        }
    }
    ili9341_writeCommand(NOOP);
}

//test the screen hardware
int main() {
    stdio_init_all();

	sleep_ms(5000);
    printf("GO");
	sleep_ms(1000);

    spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz
		spi_set_format(spi0, 16, 0, 0, SPI_MSB_FIRST);
    gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

    ili9341_initialize(ILI9341_CS,ILI9341_RST,ILI9341_DC);

    ili9341_setAddrWindow(30,30,30,30);
		sleep_ms(10);

    //ramwr
    ili9341_writeCommand(RAM_WR);
    for(int i = 0 ; i<30*30; i++) {
				uint16_t dummy = 0xFFE0;
        ili9341_writeDataBuffer16(&dummy,1);
				sleep_ms(1);
        printf("draw pix %d\n", i);
    }
    ili9341_writeCommand(NOOP);
    printf("Simple draw OK");

    Tileset* sys_tileset = new Tileset(16, (uint16_t*)&ampalaya_tileset_16[0], 30);
    Scene* base = new Scene(*sys_tileset, &tile_bg_16[0]);
    //seems ok, test 2: draw the tiles from the structures in heap
    //
    drawTilemap(base);
}
