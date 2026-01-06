#include "display.h"
#include "ili9341.h"
#include "portmacro.h"

void display_setup() {

	xDisplay_queue = xQueueCreate(10, sizeof(display_item_t));
	xDisplay_mutex = xSemaphoreCreateMutex();

	xTaskCreate(lcd_write, "lcd_write_task", 2000, NULL, 2, NULL);
	xTaskCreate(lcd_render, "lcd_render_task", 2000, NULL, 2, NULL);
}

void control_block_handler() { 
	//assign tiles from the engine to write from.
	//BLOCKER: build out the write code first..

}

void lcd_render(void* pvParameters) {
	//do tile engine initializations, protect with a semaphore.
	
	for(;;) {
		
	}
}

void lcd_write(void* pvParameters) {

	int dma_chan = dma_claim_unused_channel(true);
	dma_channel_set_irq0_enabled(dma_chan, true);

	//ASSUMES SPI IS UNINITIALIZED 
	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz 
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	ili9341_initialize( ILI9341_CS, ILI9341_RST , ILI9341_DC );

	for(;;) {
		xSemaphoreTake
	
	}
}
