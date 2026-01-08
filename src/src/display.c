#include "display.h"
#include "ili9341.h"
#include "portmacro.h"
#include "projdefs.h"
#include <hardware/dma.h>

void display_setup() {

	xDisplayQueue = xQueueCreate(10, sizeof(display_item_t));
	xDisplayMutex = xSemaphoreCreateMutex();

	xTaskCreate(lcd_write, "lcd_write_task", 2000, NULL, 2, NULL);
	xTaskCreate(lcd_render, "lcd_render_task", 2000, NULL, 2, NULL);
}

void control_block_handler() { 
	//assign tiles from the engine to write from.
	//BLOCKER: build out the write code first..

}

void lcd_render(void* pvParameters) { //do tile engine initializations, protect with a semaphore.
	
	for(;;) {
		
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
					if(recv.IS_CB_FORMAT) { control_block_handle(recv); } else {
						std_draw();
					}
				xSemaphoreGive(xDisplayMutex);
			} else { }
		} else { }
	}
}

void control_block_hanandl

int write_block(uint16_t* buf, size_t size) {
	int data_chan = dma_claim_unused_channel(true);

	//channel configure
	dma_channel_config c = dma_channel_get_default_config(data_chan);
	channel_config_set_transfer_data_size(&c, DMA_SIZE_16); //for sending 565-pixel data 
	channel_config_set_dreq(&c, uint dreq); //
	//chain dependent on the display item; IS IT TILED OR IS IT PURE BUFFER?

	channel_config_set_chain_to(dma_channel_config_t *c, uint chain_to); ///WRAP IN CONDITIONALS
																																			 
	channel_config_set_irq_quiet(&c, true);

	dma_channel_configure(data_chan,
		&c,
		volatile void *write_addr,
		const volatile void *read_addr,
		0,
		true //will start the channel imediately
		);
	//note this i within thread
}
