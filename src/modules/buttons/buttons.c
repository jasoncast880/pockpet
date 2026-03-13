#include <hardware/gpio.h>
#include <hardware/structs/sio.h>
#include <stdio.h>

#include "buttons.h"

void button_setup() {
	xButtonQueue = xQueueCreate( 10, sizeof(enum INPUT) );

	for(int i = INPUT_A ; i < INPUT_DOWN ; i++) {
		//isr initializer
		gpio_set_pulls(i, false, true); //set a pull-down.
		gpio_set_irq_enabled_with_callback(i, GPIO_IRQ_EDGE_FALL, true, button_handler);
	}
}

void button_handler() {
	for(int i = INPUT_A; i < INPUT_DOWN; i++) {
		if(gpio_get(i) == GPIO_IRQ_LEVEL_LOW) {
			//!!!!!
			xButtonItem = ( xButtonItem | ( 0x80 >> (i-INPUT_A) ) );
			//!!!!!
		}
	} //from the task, send out the button item, contingent on available queue space.
}

#ifdef RTOS_MODE

#define BUTTON_SUPER_LATENCY_MS 10
void button_task(void* pvParameters) {

	button_setup();

	for( ;; ) { //needs a consumer task
		if(xButtonItem) {
			xQueueSendToBack(xButtonQueue, &xButtonItem, BUTTON_SUPER_LATENCY_MS);
			xButtonItem = 0x00; //reset
		}
	}
}

#endif //RTOS_MODE
