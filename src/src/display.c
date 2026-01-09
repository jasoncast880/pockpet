#include "display.h"
#include "ili9341.h"
#include "portmacro.h"
#include "projdefs.h"
#include <hardware/dma.h>
#include <hardware/regs/dreq.h>
#include <hardware/structs/spi.h>

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

int cfg_write_block(display_item_t block) { //returns dma chan
	int data_chan = dma_claim_unused_channel(true);

	dma_channel_config data_cfg = dma_channel_get_default_config(data_chan);
	channel_config_set_transfer_data_size(&data_cfg, DMA_SIZE_16); //for sending 565-pixel data 
	channel_config_set_dreq(&data_cfg, DREQ_SPI0_TX); // confirm this
	channel_config_set_irq_quiet(&data_cfg, true);
	
	if(block.IS_CB_FORMAT){ //setup a ctl channel, use the tile engine as a ref?
		int ctl_chan = dma_claim_unused_channel(true);
		
		dma_channel_config ctl_cfg = dma_channel_get_default_config(ctl_chan);
		channel_config_set_transfer_data_size(&ctl_cfg, DMA_SIZE_32); //size of each reg. of dma csr
		channel_config_set_read_increment(&ctl_cfg, true);
		channel_config_set_read_increment(&ctl_cfg, true);
		channel_config_set_ring(&ctl_cfg, true, 2); //  1 << 2 boundary on write ptr (4 byte ring buffer)

		dma_channel_configure(ctl_chan,
			&ctl_cfg,
			&dma_hw->ch[data_chan].al3_read_addr_trig,
			const volatile void *read_addr, //!!!!!!! modify engine to make this easier TODO
			1, //just write to register
			false
			);

		channel_config_set_chain_to(&data_cfg, ctl_chan); //chain data to this control channel
	} 

	dma_channel_configure(data_chan,
		&data_cfg,
		&spi_get_hw(spi0)->dr,
		NULL, //if param is control blocks format, then the read addr will auto reconfigure.
		block.size, 
		false 
		);

	//TODO start the channels and control the interrupts...

	return data_chan;
}
