#include "display.h"
#include "tile_engine.h"
#include "ili9341.h"
#include "pinout.h"

#include <cstdint>
#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/irq.h>
#include <hardware/regs/dreq.h>
#include <hardware/structs/spi.h>
#include <wchar.h>

#include "ampalaya_tileset_16.h"
#include "jet_sprite.h"
#include "tilemaps.h"

DisplayHandler& DisplayHandler::setup(Layer* base) {
	static DisplayHandler instance = DisplayHandler(base);
	return instance;
}

DisplayHandler::~DisplayHandler() {} //default ; unused

DisplayHandler::DisplayHandler(Layer* base) {
	base_layer = base;

	//SPI SETUP
	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz 
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	ili9341_initialize( ILI9341_CS , ILI9341_RST , ILI9341_DC );
	
	//DMA SETUP
    dma_channel_claim(DMA_DISPLAY_CH);
	cfg = dma_channel_get_default_config(DMA_DISPLAY_CH);
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8); 
	channel_config_set_dreq(&cfg, DREQ_SPI0_TX); 
    channel_config_set_read_increment(&cfg, true);
	
	dma_channel_configure(
		display_chan,
		&cfg,
		&spi_get_hw(spi0)->dr,
		NULL, //configure in isr
		0,
		false 
		);

	//chan will fire irq0 flag when done a block
	dma_channel_set_irq0_enabled(display_chan, true);

	//enable and cfg irq0
	irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
	irq_set_enabled(DMA_IRQ_0, true);

}

int DisplayHandler::draw_dirty_tiles(Layer *layer) {
	//draw clean tiles
	current_tile = layer->dirty_tiles.data();
	end_tile = layer->dirty_tiles.data()+( layer->dirty_tiles.size() );

	//man trigger the isr.
	dma_hw->intf0 = 1u << display_chan; 

	return 1; //idk
}

cmd_sequence_t DisplayHandler::state_fromISR() {
	switch (tiling_state) {
	case CASET_CMD: //configure data for feeding
		if(current_tile!=end_tile) {
			
			for(int i = 0 ; i<4 ; i++) {
				caset_params[i] = current_tile->display_params[i];
			}
			for(int i = 0 ; i<4 ; i++) {
				raset_params[i] = current_tile->display_params[i+4];
			}
			
			pixel_buf_16 = current_tile->get_buffer();

			gpio_put( ILI9341_DC, 0 );
			dma_channel_set_read_addr( display_chan, &caset_cmd, false );
			dma_channel_set_transfer_count( display_chan, 1, true);

            tile_count++;
			current_tile++;

		    tiling_state = CASET_DATA;
		} else { 
			dirty_flag = false;
		}

		break;

	case CASET_DATA:
		gpio_put( ILI9341_DC, 1 );
		dma_channel_set_read_addr( display_chan, &caset_params[0], false );
		dma_channel_set_transfer_count( display_chan, 4, true );

		tiling_state = RASET_CMD;
		break;

	case RASET_CMD:
		gpio_put( ILI9341_DC, 0 );
		dma_channel_set_read_addr( display_chan, &raset_cmd, false );
		dma_channel_set_transfer_count( display_chan, 1, true );

		tiling_state = RASET_DATA;
		break;

	case RASET_DATA:
		gpio_put( ILI9341_DC, 1 );
		dma_channel_set_read_addr( display_chan, &raset_params[0], false );
		dma_channel_set_transfer_count( display_chan, 4, true );

		tiling_state = RAMWR_CMD;
		break;

	case RAMWR_CMD:
		gpio_put( ILI9341_DC, 0 );
		dma_channel_set_read_addr(display_chan, &ramwr_cmd, false );
		dma_channel_set_transfer_count( display_chan, 1, true );
		//start channel

		tiling_state = PIX_BUF;
		break;
	
	case PIX_BUF: //reconfigure required before DMA chan start.
		dma_channel_set_read_addr(display_chan, &pixel_buf_16[0], false);
		dma_channel_set_transfer_count( display_chan, 256*2, true );

		tiling_state = CASET_CMD; 
		break;
	}

	return tiling_state; //unused as return value. still tho
}

void dma_handler() { //
	uint32_t status = dma_hw->ints0; //
	if( (status & (1u<<DisplayHandler::display_chan)) ) {
		dma_hw->ints0 = 1u << DisplayHandler::display_chan;

		if(DisplayHandler::dirty_flag){
			DisplayHandler::state_fromISR();
		} else {
			__breakpoint; //DONE DIRTY TILES ; need to reach here.
		}

	}	
}
