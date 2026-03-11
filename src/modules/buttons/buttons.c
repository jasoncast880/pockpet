#include <hardware/gpio.h>
#include <hardware/structs/sio.h>
#include <stdio.h>

#include "buttons.h"

void button_setup() {
	xButtonQueue = xQueueCreate( 10, sizeof(enum INPUT) );
	uint32_t gpio_mask = 0x0000;

	for(int i = INPUT_A ; i < INPUT_DOWN ; i++) {
		//isr initializer
		gpio_mask = gpio_mask << (uint32_t) ( i );
		gpio_set_pulls(i, false, true); //set a pull-down.
	}

	gpio_init_mask(gpio_mask);
	gpio_add_raw_irq_handler_masked( gpio_mask , button_handler );
}

void button_handler() {
	
	for(int i = INPUT_A; i < INPUT_DOWN; i++) {

		if(gpio_get(i) == GPIO_IRQ_LEVEL_LOW) {
			//!!!!!
			xButtonItem = ( xButtonItem | 0x80 >> (i-INPUT_A) );
			//!!!!!
		}

	} //from the task, send out the button item, contingent on available queue space.

}
