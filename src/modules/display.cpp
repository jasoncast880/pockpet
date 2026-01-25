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

DisplayHandler::~DisplayHandler() {}

DisplayHandler::DisplayHandler(Layer* base) {
	base_layer = base;

	//SPI SETUP
	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz 
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	ili9341_initialize( ILI9341_CS , ILI9341_RST , ILI9341_DC );
	
	//DMA SETUP

	cmd_chan = dma_claim_unused_channel(true); 
	pixel_chan = dma_claim_unused_channel(true);

	cmd_sequence_t tiling_state = CASET_CMD; 
	dma_channel_config pixel_cfg = dma_channel_get_default_config(pixel_chan);

	channel_config_set_transfer_data_size(&pixel_cfg, DMA_SIZE_16); 
	channel_config_set_dreq(&pixel_cfg, DREQ_SPI0_TX); // confirm this
	//irq quiet by default is false. irq will fire every transfer-count-cycle.
	
	dma_channel_configure(
		pixel_chan,
		&pixel_cfg,
		&spi_get_hw(spi0)->dr,
		NULL, 
		(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN),
		false 
		);

	//data chann will fire irq0 flag when done a block
	dma_channel_set_irq0_enabled(pixel_chan, true);

	dma_channel_config cmd_cfg = dma_channel_get_default_config(cmd_chan);

	channel_config_set_transfer_data_size(&cmd_cfg, DMA_SIZE_8); 
	channel_config_set_dreq(&cmd_cfg, DREQ_SPI0_TX); // confirm this
	
	dma_channel_configure(
		cmd_chan,
		&cmd_cfg,
		&spi_get_hw(spi0)->dr,
		NULL, //configure in isr
		0,
		false 
		);

	//chann will fire irq0 flag when done a block
	dma_channel_set_irq0_enabled(cmd_chan, true);

	//enable and cfg irq0
	irq_set_exclusive_handler(DMA_IRQ_0, cmd_handler);
	irq_set_enabled(DMA_IRQ_0, true);

}

uint32_t max_count, count;
bool dirty_flag;
//TEMP

int DisplayHandler::draw_frame(Layer *layer) {
	dirty_flag = false;
	//draw clean tiles
	max_count = DEFAULT_SCREEN_TILES_X*DEFAULT_SCREEN_TILES_Y;
	count = 0;
	tiling_state = PIX_BUF;
	
	//man trigger the isr.
	dma_hw->intf0 = 1u << cmd_chan; //starts with clean tiles, auto goes to dirty via 
																	//isr + flags.
	return 1; //idk
}

cmd_sequence_t DisplayHandler::state_fromISR(cmd_sequence_t state) {
	switch (tiling_state) {
	case CASET_CMD:
		gpio_put( ILI9341_DC, 1 );
		dma_channel_set_read_addr( cmd_chan, &raset_cmd, false );
		dma_channel_set_transfer_count( cmd_chan, 1, true );

		tiling_state = CASET_CMD;
		break;

	case CASET_DATA:
		gpio_put( ILI9341_DC, 0 );
		dma_channel_set_read_addr( cmd_chan, &raset_cmd, false );
		dma_channel_set_transfer_count( cmd_chan, 1, true );

		tiling_state = RASET_CMD;
		break;

	case RASET_CMD:
		gpio_put( ILI9341_DC, 1 );
		dma_channel_set_read_addr( cmd_chan, &raset_params[0], false );
		dma_channel_set_transfer_count( cmd_chan, 4, true );

		tiling_state = RASET_DATA;
		break;

	case RASET_DATA:
		dma_channel_set_read_addr(cmd_chan, &ramwr_cmd, false );
		dma_channel_set_transfer_count( cmd_chan, 1, true );
		//start channel

		tiling_state = RAMWR_CMD;
		break;
	
	case RAMWR_CMD:

		dma_channel_set_read_addr(pixel_chan, &pixel_buf_16[0], true);

		tiling_state = PIX_BUF; 
		break;

	case PIX_BUF: //key-section
		dma_hw->ints0 = 1u << pixel_chan;
		if(dirty_flag) { // DRAW DIRTY TILE PIXELS
			if(count<max_count) {
				Tile* tile = &(DisplayHandler::base_layer->dirty_tiles.at(count));
				uint16_t x0 = tile->x;
				uint16_t y0 = tile->y;

				caset_params[0]= x0>>8;
				caset_params[1]= x0&0xff;
				caset_params[2]= (x0+15)>>8;
				caset_params[3]= (x0+15)&0xff;

				raset_params[0] = y0>>8;
				raset_params[1] = y0&0xff;
				raset_params[2] = (y0+15)>>8;
				raset_params[3] = (y0+15)&0xff;
				
				pixel_buf_16 = tile->get_buffer();

				gpio_put( ILI9341_DC, 0 );
				dma_channel_set_read_addr( cmd_chan, &caset_cmd, false );
				dma_channel_set_transfer_count( cmd_chan, 1, true);

				tiling_state = CASET_CMD;
			} else { 
				/*
				 * once this section is reached, the layer is done rendering. 
				 * TODO:try writing an isr handler and assigning irq for 'vsync'-like functionality
				*/
			}

		} else if (!dirty_flag) { // DRAW CLEAN TILE PIXELS
			if(count<max_count) {
				Tile* tile = DisplayHandler::base_layer->tileset->get_tile(count);

				uint16_t x0 = count%DEFAULT_SCREEN_TILES_X;
				uint16_t y0 = count/DEFAULT_SCREEN_TILES_Y;

				caset_params[0]= x0>>8;
				caset_params[1]= x0&0xff;
				caset_params[2]= (x0+15)>>8;
				caset_params[3]= (x0+15)&0xff;

				raset_params[0] = y0>>8;
				raset_params[1] = y0&0xff;
				raset_params[2] = (y0+15)>>8;
				raset_params[3] = (y0+15)&0xff;
				
				pixel_buf_16 = tile->get_buffer();

				gpio_put( ILI9341_DC, 0 );
				dma_channel_set_read_addr( cmd_chan, &caset_cmd, false );
				dma_channel_set_transfer_count( cmd_chan, 1, true);

				tiling_state = CASET_CMD;
			} else { dirty_flag = !dirty_flag; }
		}
	}

	return tiling_state;
}

void cmd_handler() {
	dma_hw->ints0 = 1u << DisplayHandler::cmd_chan; 
	
	DisplayHandler::state_fromISR(DisplayHandler::tiling_state);

}


