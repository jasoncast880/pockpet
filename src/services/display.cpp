#include "display.h"
#include "FrameHandler.h"
#include "ili9341.h"
#include "pinout.h"
#include "portmacro.h"
#include "projdefs.h"
#include <hardware/dma.h>
#include <hardware/irq.h>
#include <hardware/regs/dreq.h>
#include <hardware/structs/spi.h>

#include "ampalaya_tileset_16.h"
#include "jet_sprite.h"
#include "tilemaps.h"

void display_setup() {

	//SPI SETUP
	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz 
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	ili9341_initialize(ILI9341_CS, ILI9341_RST , ILI9341_DC);

	//DMA SETUP
	data_chan = dma_claim_unused_channel(true);
	dma_channel_config cfg = dma_channel_get_default_config(data_chan);
	
	channel_config_set_transfer_data_size(&data_cfg, DMA_SIZE_16); 
	channel_config_set_dreq(&data_cfg, DREQ_SPI0_TX); // confirm this
	
	dma_channel_configure(
		data_chan,
		&cfg,
		&spi_get_hw(spi0)->dr,
		NULL, //isr/manual-write-trigger.
		(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN),
		false 
		);

	//data chann will fire irq0 flag when done a block
	dma_channel_set_irq0_enabled(data_chan, true);

	//enable and cfg irq0
	irq_set_exclusive_handler(DMA_IRQ_0, tiling_handler);
	irq_set_enabled(DMA_IRQ_0, true);
}

//TEMP

Layer* screen = new Layer(/**/);
uint32_t count = 0 ; //for dirty-tile element access.

//Sprites, sprite handling ...

//TEMP

void tiling_handler() { 
	if( count<(screen->dirty_tiles.size()) ){
		tile_item_t item = screen->dirty_tiles.at(count);

		ili9341_setAddrWindow(item.x, item.y, DEFAULT_TILE_LEN, DEFAULT_TILE_LEN);
		dma_channel_set_read_addr(data_chan, item.buf, true); 

		count++;
		
		dma_hw->ints0 = 1u << data_chan;
	}
}

int draw_tiles() { //returns dma chan
	//optional handling logic??
	tiling_handler(); 
	return data_chan;
}

 
