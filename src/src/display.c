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

	xDisplayQueue = xQueueCreate(10, sizeof(display_item_t));
	xDisplayMutex = xSemaphoreCreateMutex();

	xTaskCreate(lcd_write, "lcd_write_task", 2000, NULL, 2, NULL);
	xTaskCreate(lcd_render, "lcd_render_task", 2000, NULL, 2, NULL);
}

void lcd_render(void* pvParameters) { //do tile engine initializations, 
																			//protect RAM writes with a semaphore.
	Tileset* system_tiles = new Tileset(16, &ampalaya_tileset_16[0], 30);
	Tileset* jet_tiles = new Tileset(16, &jet_sprite_16[0], 16);

	Scene* base = new Scene(system_tiles, &tile_bg_16[0]);

	Sprite* cursor = new Sprite(x,y,2,2,jet_tiles, &demo_spritemap_1[0]);
	
	for(;;) { //based on input received, alter the tilemaps and pass a display item to the HW wrapper
		//no input yet, so simulate with tile engine calls.
		cursor.set_position

		if(xSemaphoreTake(xDisplayMutex, pdMS_TO_TICKS(100))==pdTRUE) {
			
			xSemaphoreGive(xDisplayMutex);
		} else { vTaskDelay( pdMS_TO_TICKS(10) ); }
	}
}

void lcd_write(void* pvParameters) { //queue reciever

	//ASSUMES SPI IS UNINITIALIZED 
	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz 
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	ili9341_initialize(ILI9341_CS, ILI9341_RST , ILI9341_DC);

	display_item_t recv;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
	for(;;) {

		if(xQueueReceive(xDisplayQueue, &recv, xTicksToWait) == pdPASS) {
			if(xSemaphoreTake(xDisplayMutex, pdMS_TO_TICKS(100))==pdTRUE) {
				cfg_write_block(recv);
				xSemaphoreGive(xDisplayMutex);
			} else { vTaskDelay( pdMS_TO_TICKS(10) ); }
		} else { vTaskDelay( pdMS_TO_TICKS(10) ); }
	}
}

int cfg_write_block(display_item_t block) { //returns dma chan
	data_chan = dma_claim_unused_channel(true);

	dma_channel_config data_cfg = dma_channel_get_default_config(data_chan);
	
	if(block.CB_NUM_TILES){ //setup a ctl channel, use the tile engine as a ref?
		int ctl_chan = dma_claim_unused_channel(true);
		
		dma_channel_config ctl_cfg = dma_channel_get_default_config(ctl_chan);
		channel_config_set_transfer_data_size(&ctl_cfg, DMA_SIZE_32); //size of each reg. of dma csr
		channel_config_set_read_increment(&ctl_cfg, true);
		channel_config_set_read_increment(&ctl_cfg, true);
		channel_config_set_ring(&ctl_cfg, true, 2); //  1 << 2 boundary on write ptr (4 byte ring buffer)

		dma_channel_configure(ctl_chan,
			&ctl_cfg,
			&dma_hw->ch[data_chan].al3_read_addr_trig,
			const volatile void *read_addr, //!!!!!!! modify engine to make this easier TODO. visualize
			1, //just write to register
			false
			);

		//conditional data channel configurations below
		channel_config_set_chain_to(&data_cfg, ctl_chan); //chain data to control channel
		channel_config_set_irq_quiet(&data_cfg, false);

		//irq sets
		dma_channel_set_irq0_enabled(data_chan, true);
		irq_set_exclusive_handler(DMA_IRQ_0, dma_tiling_isr);
		irq_set_enabled(DMA_IRQ_0, true);

	} else {
		channel_config_set_irq_quiet(&data_cfg, true); 
		//need a different/manual way to address irq once channel ends.
	} 

	channel_config_set_transfer_data_size(&data_cfg, DMA_SIZE_16); //for sending 565-pixel data 
	channel_config_set_dreq(&data_cfg, DREQ_SPI0_TX); // confirm this


	dma_channel_configure(data_chan,
		&data_cfg,
		&spi_get_hw(spi0)->dr,
		NULL, //if param is control blocks format, then the read addr will auto reconfigure.
		block.size, 
		false 
		); //WHEN this has a null trigger, write a isr to handle.

	//to start: run the data channel, the ctl (if setup) will run automatically. Ensure that CS is on.

	ili9341_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h);
	ili9341_writeCommand(RAM_WR);
	gpio_put(ILI9341_CS, 0);
	dma_channel_start(data_chan); //once the block/chain of blocks (if enabled), run an ISR to give spi
	

	return data_chan;
}

void dma_tiling_isr() { //in the event that i need to do tiling, this will reconfigure draw area
	//here run driver commands to set draw to the next tile space.
	ili9341_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h);
	
	dma_hw->ints0 = 1u << data_chan;
} 
