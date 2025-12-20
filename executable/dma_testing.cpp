#include <cstddef>
#include <hardware/spi.h>
#include <hardware/dma.h>
#include <hardware/irq.h>

#include <hardware/structs/spi.h>
#include <pico/stdio.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"
#include "ili9341.h"

static void dma_handler() {
	dma_hw->ints0 = 1u << dma_chan;
}

//test the screen hardware
int main() {
	stdio_init_all();

	spi_init(spi0, 8000 * 1000);
	gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
	gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);
	ili9341_initialize( ILI9341_CS, ILI9341_RST, ILI9341_DC );

	//setup channels and cfg struct
	int ctl_chan = dma_claim_unused_channel(true);
	int data_chan = dma_claim_unused_channel(true);

	dma_channel_config c = dma_channel_get_default_config(ctl_chan);
	channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
	channel_config_set_read_increment(&c, true);
	channel_config_set_write_increment(&c, true);
	channel_config_set_ring(&c, true, 3);

	dma_channel_configure(
			ctl_chan,
			&c,
			&dma_hw->ch[data_chan].al3_transfer_count,
			&buf, /* !!!TODO */ 
			2,
			false);

	c = dma_channel_get_default_config(data_chan);
	channel_config_set_transfer_data_size( &c, DMA_SIZE_16);
	channel_config_set_dreq( &c, DREQ_SPI0_TX ); // not sure if this is going right way
	channel_config_set_chain_to( &c, ctl_chan );
	channel_config_set_irq_quiet(&c, true);

	dma_channel_configure(
			ctl_chan,
			&c,
			&spi_get_hw(spi0)->dr,
			NULL, //controled by ctl chan
			0, //controlled by the ctl channel
			false);

	//define control blocks structure and assign values
	//start channels, await interrupts
	dma_start_channel_mask( 1u << ctl_chan ); //no isr necessary, all handling is handled
																						//via ctl chan
																						//write isr for handling the reconfig of control blocks??
	//do a busy loop; led blink for example
}
