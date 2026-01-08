#include "display.h"
#include "ili9341.h"
#include "portmacro.h"
#include "projdefs.h"

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

void lcd_render(void* pvParameters) {
	//do tile engine initializations, protect with a semaphore.
	
	for(;;) {
		
	}
}

void lcd_write(void* pvParameters) { //queue reciever

	int dma_chan = dma_claim_unused_channel(true);
	dma_channel_set_irq0_enabled(dma_chan, true);

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
