#include <hardware/gpio.h>
#include <hardware/structs/sio.h>
#include <stdio.h>

#include "buttons.h"

void button_setup() {
		xButtonBuf = xQueueCreate( 10, sizeof(enum INPUT) );
		uint32_t gpio_mask = 0x0000;

		for(int i = INPUT_A ; i < INPUT_DOWN ; i++) {
			//isr initializer
			gpio_mask = gpio_mask << (uint32_t) ( i );
		}

		gpio_init_mask(gpio_mask);
		gpio_add_raw_irq_handler_masked( gpio_mask , button_handler );
}

void button_handler() {
	
	enum INPUT xQueueItem = sio_hw->gpio_in;
	xQueueSendFromISR(xButtonBuf, xQueueItem, 0);

}



