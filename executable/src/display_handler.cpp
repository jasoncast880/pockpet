#include "display_handler.h"
#include "pinout.h"
#include "ili9341.h"
#include "FrameHandler.h"

#include "projdefs.h"
#include "tilemaps.h"
#include "ampalaya_tileset_16.h"

#include "semphr.h"
#include "portmacro.h"

#include <hardware/dma.h>
#include <hardware/irq.h>
#include <hardware/regs/intctrl.h>
#include <hardware/spi.h>
#include <pico/platform/common.h>
#include <stdexcept>
#include <vector>

void display_setup(){
	xDisplaySemaphore = xSemaphoreCreateBinary();
	xDisplayQueue = xQueueCreate(10, sizeof(display_msg_t));
	
	xTaskCreate( lcd_write_task, "lcd_write", 2000, NULL, 2, NULL );
	xTaskCreate( lcd_render_task, "lcd_read", 2000, NULL, 2, NULL );

	printf("display_setup done");
}

void lcd_render_task(void* pvParameters) { //SOFTWARE
	sys_tileset = new Tileset(16, (uint16_t*)&ampalaya_tileset_16[0], 30);
	jet_tileset = new Tileset(16, (uint16_t*)&jet_tileset[0], 16);
	render = new RenderController(*base);

	base = new Scene(*sys_tileset, &tile_bg_16[0]);
	jetsprite = new Sprite(30,30,2,2,*jet_tileset, maps[0]);

	for( ;; ) { //recv from display queue process appropriately
	}

}

void lcd_write_task(void* pvParameters) {  //HARDWARE
	dma_spi0_smphr = xSemaphoreCreateBinary();

	dma_chan = dma_claim_unused_channel(true);
	dma_channel_set_irq0_enabled(dma_chan, true);
	irq_set_exclusive_handler(DMA_IRQ_0, dma_irq_handler);
	irq_set_enabled(DMA_IRQ_0, true);

	spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);

	ili9341_initialize( ILI9341_CS, ILI9341_RST , ILI9341_DC );
	sleep_ms(100);

	display_msg_t q;

	for( ;; ) { //receive/read events, send to display queue 

		xQueueReceive( xDisplayQueue, &q, pdMS_TO_TICKS(100)); 

		//configure dma write, spi read
		xQueueSemaphoreTake(xDisplaySemaphore, pdMS_TO_TICKS(100)); //rand time
		ili9341_setCS_LOW();
		ili9341_writeCommand(RAM_WR);

		display_dma_transfer_blocking(const void *buf, size_t size, TickType_t timeout);
		//something like p_thing, p_size, pdMS_TO_TICKS(PortMaxDelay)

	}
}

static void display_dma_transfer(const void *buf, size_t size, TickType_t timeout) {
	volatile void *spi_tx_fifo = &spi_get_hw(spi0)->dr;
	
	dma_channel_config cfg = dma_channel_get_default_config(dma_chan);
		channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
		channel_config_set_read_increment(&cfg, true);
		channel_config_set_write_increment(&cfg, false);
		channel_config_set_irq_quiet(&cfg, true);
		channel_config_set_dreq(&cfg, spi_get_dreq(spi0, true));

	dma_channel_configure(
		dma_chan,
		&cfg,
		spi_tx_fifo,
		buf, 
		size,
		true
	); //at end of this transfer will trigger dma isr
	
}

static void dma_irq_handler(){
//keep simple; acknowledge, reset the interrupt, and give periph access for other spi user
	BaseType_t woken = pdFALSE;

	dma_hw->ints0 = 1u << dma_chan;

	xSemaphoreGiveFromISR(xDisplaySemaphore, pdFALSE); //understand this
	portYIELD_FROM_ISR(woken);
}
