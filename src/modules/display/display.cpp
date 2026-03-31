#include "display.h"
#include "tile_engine.h"
#include "ili9341.h"
#include "pinout.h"

#include <cstdint>
#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/irq.h>
#include <hardware/regs/dreq.h>
#include <hardware/spi.h>
#include <hardware/structs/spi.h>
#include <wchar.h>

#include "ampalaya_tileset_16.h"
#include "jet_sprite.h"
#include "tilemaps.h"

//#define DMA_DRAW 

DisplayHandler& DisplayHandler::setup(Layer* base) {
	static DisplayHandler instance = DisplayHandler(base);
	return instance;
}

DisplayHandler::~DisplayHandler() {} //default ; unused

#ifdef DMA_DRAW
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

int DisplayHandler::draw_clean_tiles(Layer *layer) {
  return 1;
}

int DisplayHandler::draw_dirty_tiles(Layer *layer) {
	int ret;

	//draw clean tiles
	current_tile = layer->dirty_tiles.data();
	end_tile = layer->dirty_tiles.data()+( layer->dirty_tiles.size() );

	//man trigger the isr.
	dma_hw->intf0 = 1u << display_chan; 

	//do a countdown, if int i = 
	for(int ct = 0 ; ct < 500; ct++) {
		sleep_ms(10);
		if(dirty_flag==-1) {
			break;
		}
	}
	ret = (dirty_flag==-1) ? -1 : 1;
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
			
			pixel_buf_8 = (uint8_t*)current_tile->get_buffer();

			gpio_put( ILI9341_DC, 0 );
			dma_channel_set_read_addr( display_chan, &caset_cmd, false );
			dma_channel_set_transfer_count( display_chan, 1, true);

			current_tile++;

			tiling_state = CASET_DATA;
		} else { 
			dirty_flag = 0;
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
		gpio_put( ILI9341_DC, 0 );
		dma_channel_set_read_addr(display_chan, &pixel_buf_8[0], false);
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
			return;
		} else {
			DisplayHandler::dirty_flag = -1; //reset the flag.
			return;
			//__breakpoint; //DONE DIRTY TILES ; need to reach here.
		}

	}	
}

#endif 

#ifndef DMA_DRAW //normal spi transmission (for testing the engine)

DisplayHandler::DisplayHandler(Layer* base) {
	base_layer = base;

	//SPI SETUP
	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz 
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
	ili9341_initialize( ILI9341_CS , ILI9341_RST , ILI9341_DC );
	//consider doing a draw on the base here..
}

int DisplayHandler::draw_clean_tiles(Layer *layer) { //this definitely will block

	spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
	for(int i = 0 ; i < layer->tiles_high ; i++) {
			for(int j = 0 ; j < layer->tiles_wide ; j++ ){

				gpio_put(ILI9341_CS,0);

				Tile* tile = layer->get_tile( i*layer->tiles_wide + j );

				uint16_t x0 = (j*DEFAULT_TILE_LEN);
				uint16_t x1 = x0+15;
				uint16_t y0 = (i*DEFAULT_TILE_LEN);
				uint16_t y1 = y0+15;
				ili9341_writeCommand(CASET);
				ili9341_writeData( x0 >> 8 );
				ili9341_writeData( x0 & 0xff );
				ili9341_writeData( x1 >> 8 );
				ili9341_writeData( x1 & 0xff );
				ili9341_writeCommand(RASET);
				ili9341_writeData( y0 >> 8 );
				ili9341_writeData( y0 & 0xff );
				ili9341_writeData( y1 >> 8 );
				ili9341_writeData( y1 & 0xff );
				ili9341_writeCommand(RAM_WR);

				/*
				spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
				ili9341_writeDataBuffer16(tile->get_buffer(), DEFAULT_TILE_LEN*DEFAULT_TILE_LEN);
				spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
				*/
					
				ili9341_writeDataBuffer8( (uint8_t*)tile->get_buffer(), 
								DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*2);

				gpio_put(ILI9341_CS,1); 
			}
	}

	return 1;
}

int DisplayHandler::draw_dirty_tiles(Layer *layer) { //this definitely will block
	current_tile = layer->dirty_tiles.data();
	end_tile = layer->dirty_tiles.data()+( layer->dirty_tiles.size() );

	while(current_tile!=end_tile) {

		gpio_put(ILI9341_CS,0);

		ili9341_writeCommand(CASET);
		for(int i = 0 ; i < 4 ; i++) {
			ili9341_writeData(current_tile->display_params[i]);
		}
		ili9341_writeCommand(RASET);
		for(int i = 4 ; i < 8 ; i++) {
			ili9341_writeData(current_tile->display_params[i]);
		}

		ili9341_writeCommand(RAM_WR);
		spi_set_format(spi0, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
		ili9341_writeDataBuffer16(current_tile->get_buffer(), DEFAULT_TILE_LEN*DEFAULT_TILE_LEN);
		spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

		current_tile++;

		gpio_put(ILI9341_CS,1); 
	}
	return 1; //idk
}

#endif //SPI DRAW

#ifdef RTOS_MODE

#include "FreeRTOS.h"
#include "task.h"

void display_task(void* pvParameters) {

	Tileset* sys_tileset = new Tileset( (uint16_t*)&ampalaya_tileset_16[0], static_cast<size_t>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*30) );
	Layer* screen = new Layer(
		static_cast<uint8_t>(DEFAULT_SCREEN_TILES_X),
		static_cast<uint8_t>(DEFAULT_SCREEN_TILES_Y),
		sys_tileset,
		&tile_bg_16[0],
		0
		); //id not relevant yet ? TODO: id handling system.

	Tileset* jet_tileset = new Tileset( (uint16_t*)&jet_sprite_16[0], static_cast<size_t>(4096) );
	uint8_t cursor = screen->sprite_add(
		2,
		2,
		jet_tileset,
		&demo_spritemap_1[0]
	);

	DisplayHandler& display = DisplayHandler::setup(screen); 

	static int x = 100;
	static int y = 100;

	for( ;; ) {
		//vTaskDelay(pdMS_TO_TICKS(500));
		xSemaphoreTake(spi0_sync_t, portMAX_DELAY);

		screen->sprite_update_by_id(cursor, x, y, demo_spritemap_1);
		screen->render();

		if(display.draw_dirty_tiles(screen) < 0 ) {
		    x--; //pos change for engine to chew on
    }
	
		xSemaphoreGive(spi0_sync_t);
	}

}

#endif

