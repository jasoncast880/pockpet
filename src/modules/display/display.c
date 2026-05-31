#include "display.h"
#include <hardware/dma.h>
#include <hardware/irq.h>
#include <hardware/structs/spi.h>

#include "graphics_conf.h"

//for assets ; 
#include "ampalaya_tileset_16.h"
#include "tilemaps.h"

static uint32_t spi0_dma_chan; //hardware

void display_setup() {
e = engine_init(add_layer( &ampalaya_tileset_16[0], 30, &tile_bg_16[0], 320/DEFAULT_TILE_LEN,  240/DEFAULT_TILE_LEN)); //pass by pointer

#if   DIRTY_RENDER
	irq_set_exclusive_handler(DMA_IRQ_0, tile_handler);
#elif FULSCREEN_RENDER
	irq_set_exclusive_handler(DMA_IRQ_0, frame_handler);
#elif HSCANLINE_RENDER
	irq_set_exclusive_handler(DMA_IRQ_0, hscanline_handler);
#endif

	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz 
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	ili9341_initialize( ILI9341_CS , ILI9341_RST , ILI9341_DC );

	spi0_dma_chan = dma_claim_unused_channel(true);
	dma_channel_config_t c = dma_channel_get_default_config(spi0_dma_chan);

	channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
	channel_config_set_dreq(&c, DREQ_SPI0_RX);
	//channel_config_set_ring(&c, false, e->render_buf_size); //projected buffer size is not aligned to ring configuration
	dma_channel_configure(spi0_dma_chan,
		 &c,
		 &spi0_hw->dr,
		 NULL, //set read addr in a static helper func.
		 DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*2, //every tile has DEFAULT_TILE_LEN^2, and 2 8-bit transfers per pixel-unit
		 false ); //CHANNEL CONFIGURED, DO NOT START
	dma_channel_set_irq0_enabled(spi0_dma_chan, true);


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
						   
	ili9341_writeCommand(NOOP);
	ili9341_setCS_HI();
#ifdef RTOS_MODE
	//flag engine to render via semphr
	xSemaphoreGiveFromISR(render_token);
#endif

#ifndef RTOS_MODE
	//flag engine to render via gl. static flag
	render_flag = !render_flag;
#endif

	dma_channel_set_read_addr(
		spi0_dma_chan, 
		e->render_data,
		false
	)

}

#ifdef RTOS_MODE

void push_pixels( void* pvParameters ) { 
	for(;;) {
		xSemaphoreTake(render_token, portMAX_DELAY);
		ili9341_setCS_LO();

		//reconfigure the display draw area
		y0 = h_scanline_counter * HSCANLINE_SIZE
		ili9341_setAddrWindow(0,y0,DEFAULT_TILE_LEN*DEFAULT_SCREEN_TILES_X, HSCANLINE_SIZE);
		ili9341_writeCommand(RAM_WR);

		dma_channel_start(spi0_dma_chan); //dma isr will return the semphr, channel is on ring so dont worry about resetting the pointer.
		//refer to the handler/isr for more detail

#if HSCANLINE_RENDER
		e->h_scanline_counter++;
#endif
	}
}

void render( void* pvParameters ) {
	for(;;) {
		xSemaphoreTake(render_token, portMAX_DELAY);

#if HSCANLINE_RENDER
		if( e->h_scanline_counter < HSCANLINE_MAX ) {
			engine_render(e); //TODO how much time does this take?
			h_scanline_counter++;
		} else {
			h_scanline_counter = 0; //TODO give update entities access to run
		}
#endif

		xSemaphoreGive(render_token);
	}
}

void update_entities(void* pvParams ) {
	for(;;) {
	}
}

#endif 
