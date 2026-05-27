#include "display.h"
#include <hardware/dma.h>
#include <hardware/irq.h>
#include <hardware/structs/spi.h>

#include "graphics_conf.h"

//for assets ; 
#include "ampalaya_tileset_16.h"
#include "tilemaps.h"

uint32_t spi0_dma_chan;
uint32_t tiles_drawn = 0;
struct RenderInfo_t* r;

void display_setup() {
	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz 
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	ili9341_initialize( ILI9341_CS , ILI9341_RST , ILI9341_DC );

	spi0_dma_chan = dma_claim_unused_channel(true);
	dma_channel_config_t c = dma_channel_get_default_config(spi0_dma_chan);

	channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
	channel_config_set_dreq(&c, DREQ_SPI0_RX);
dma_channel_configure(spi0_dma_chan,
		 &c,
		 &spi0_hw->dr,
		 NULL, //set read addr in a static helper func.
		 DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*2, //every tile has DEFAULT_TILE_LEN^2, and 2 8-bit transfers per pixel-unit
		 false ); //CHANNEL CONFIGURED, DO NOT START
	dma_channel_set_irq0_enabled(spi0_dma_chan, true);

#if   DIRTY_RENDER
	irq_set_exclusive_handler(DMA_IRQ_0, tile_handler);
#elif FULSCREEN_RENDER
	irq_set_exclusive_handler(DMA_IRQ_0, frame_handler);
#elif HSCANLINE_RENDER
	static int HLINE_COUNTER = 0 ; 
	static const int HLINE_MAX = (DEFAULT_SCREEN_TILES_Y*DEFAULT_TILE_LEN)/HSCANLINE_SIZE;

	irq_set_exclusive_handler(DMA_IRQ_0, hscanline_handler);
#endif
	irq_set_enabled(DMA_IRQ_0, true);
	


#ifdef RTOS_MODE
	render_token = xSemaphoreCreateBinary();
#endif
#ifndef RTOS_MODE
	render_flag = true;
#endif 
}

volatile uint32_t tile_count = 0; //TODO: build engine api to give easy data

//for scanline reconfiguration
void hscanline_handler() {
						   
#ifdef RTOS_MODE
	//flag engine to render via semphr
	xSemaphoreGiveFromISR(render_token);
#endif

#ifndef RTOS_MODE
	//flag engine to render via gl. static flag
	render_flag = !render_flag;
#endif

//TODO note that since the dma push is done, you need to reconfigure this channel with the next data AND spi command to the next hscan block.

}

void tile_handler() { //manage a std::Vector ?? or something similar 
	/*
	//TODO: add a check to see which tile you are on.
	if(tiles_drawn<=render_ct)
	
	//TODO: tile x0, x1, AddrWindow configuration.
	dma_channel_set_read_addr(spi0_dma_chan,
		dma_hw->ch[spi0_dma_chan].read_addr+=( DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*2 ),
		true );
	*/
}


#ifdef RTOS_MODE

#if HSCANLINE_RENDER
void display_task( void* pvParameters ) { 
	
    Engine* e = engine_render(add_layer( &ampalaya_tileset_16[0], 30, &tile_bg_16[0], 320/DEFAULT_TILE_LEN,  240/DEFAULT_TILE_LEN)); 

	for(;;) {
		xSemaphoreTake(render_token, portMAX_DELAY);
		
		//reconfigure the channel read address by re-doing the buffer.
		//TODO rework to circular buffer for less configuration & CPU oversight
		dma_channel_set_read_addr(spi0_dma_chan, e., true);
		dma_channel_start(spi0_dma_chan);
	}
}
#endif

void render_task( void* pvParameters ) {
	for(;;) {
		xSemaphoreTake(render_token, portMAX_DELAY);

		//make engine object
		//engine->handle_entities();
		//engine->render();

		tight_loop_contents();

		xSemaphoreGive(render_token);
	}
}

#endif 
