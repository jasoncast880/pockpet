#include <cstddef>
#include <hardware/gpio.h>
#include <hardware/regs/intctrl.h>
#include <hardware/spi.h>
#include <hardware/dma.h>
#include <hardware/irq.h>
#include <hardware/gpio.h>
#include <hardware/regs/io_bank0.h>

#include <hardware/structs/spi.h>
#include <pico/stdio.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"
#include "ili9341.h"

typedef enum{
	SYS_A = 2,
	SYS_B,
	SYS_START,
	SYS_SEL,
	SYS_LEFT,
	SYS_UP,
	SYS_RIGHT,
	SYS_DOWN
} btn_layout_t;

static void button_handler( void ) {
	IO_BANK0->INTR[i] = pending_bits;
}

static void dma_handler( void ) {
	dma_hw->ints0 = 1u << dma_chan; //
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
	dma_start_channel_mask( 1u << ctl_chan ); 

	//do a busy loop; led blink for example
	gpio_init(PICO_DEFAULT_LED_PIN_INVERTED);
	gpio_set_dir(PICO_DEFAULT_LED_PIN_INVERTED, GPIO_OUT);
	bool status = true;

	//buttons
	irq_set_excusive_handler(IO_IRQ_BANK0, button_handler);
	irq_set_enabled(IO_IRQ_BANK0, true);

	while(1) {
		gpio_put(PICO_DEFAULT_LED_PIN_INVERTED, status);
		sleep_ms(100);

		status = !status;
	}
}
