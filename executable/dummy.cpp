#include <cstddef>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "ili9341.h"
#include "FrameHandler.h"

#include <ampalaya_tileset_16.h>
#include <tilemaps.h>

//test the screen hardware
int main() {
    stdio_init_all();

	sleep_ms(5000);
    printf("GO");
	sleep_ms(1000);

    spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz
    gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

    ili9341_initialize(ILI9341_CS,ILI9341_RST,ILI9341_DC);

    ili9341_setAddrWindow(30,30,30,30);
		sleep_ms(10);

    //ramwr
    ili9341_writeCommand(RAM_WR);
    for(int i = 0 ; i<30*30; i++) {
        ili9341_writeData(0xFF);
        ili9341_writeData(0xFF);
		sleep_ms(10);
        printf("draw pix %d\n", i);
    }
		printf("Simple draw OK");

		Tileset* sys_tileset = new Tileset(16, (uint16_t*)&ampalaya_tileset_16[0], 30);
		Scene* base = new Scene(*sys_tileset, &tile_bg_16[0]);
		//test these datas
}
