#include "display.h"
#include <hardware/dma.h>
#include <hardware/irq.h>
#include <hardware/structs/spi.h>
#include "graphics_conf.h"
#include "engine_api.h"

//idea:
//use the dma to stream data from memory to the spi0 data register.
//Problems;
//need to reconfigure read address after every tile. - use an irq to set the pointer
//
uint32_t spi0_dma_chan;
uint32_t tiles_drawn = 0;
struct RenderInfo_t r;
void display_setup() {
	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz 
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	ili9341_initialize( ILI9341_CS , ILI9341_RST , ILI9341_DC );

	spi0_dma_chan = dma_claim_unused_channel(true);
	dma_channel_config_t c = dma_channel_get_default_config(spi0_dma_chan);

	//configure channel, don't start.
	channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
	channel_config_set_dreq(&c, DREQ_SPI0_RX);
	//other defaults are OK, consult the sdk sheet

	dma_channel_configure(spi0_dma_chan,
		 &c,
		 spi0_hw->dr,
		 NULL, //set read addr in a static helper func.
		 DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*2, //every tile has DEFAULT_TILE_LEN^2, and 2 8-bit transfers per pixel-unit
		 false );

	//configure isr for read address re-alignment
	dma_channel_set_irq0_enabled(spi0_dma_chan, true);
	irq_set_exclusive_handler(DMA_IRQ_0, tile_handler);
	irq_set_enabled(DMA_IRQ_0, true);
	
	// DEMO
	struct LayerHandle_t system = add_layer(uint16_t *tiles, size_t num_tiles, uint8_t *tilemap, uint8_t tiles_wide, uint8_t tiles_high);
	
	volatile uint32_t tile_count = 0; //TODO: build engine api to give easy data

	//TODO: tile x0, x1, AddrWindow configuration.
	dma_channel_set_read_addr( spi0_dma_chan, r.render_tiles, true );
	r = engine_render(system);
}

void tile_handler() {
	//TODO: add a check to see which tile you are on.
	if(tiles_drawn<=render_ct) 
	
	//TODO: tile x0, x1, AddrWindow configuration.
	dma_channel_set_read_addr(spi0_dma_chan,
		dma_hw->ch[spi0_dma_chan].read_addr+=( DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*2 ),
		true );
}


#ifdef RTOS_MODE

void display_task( void* pvParameters ) {
	for(;;) {

	}
}

#endif 
