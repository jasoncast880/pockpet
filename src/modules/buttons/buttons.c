#include <hardware/gpio.h>
#include <hardware/structs/sio.h>
#include <stdio.h>

#include "buttons.h"
#include "portmacro.h"
#include "projdefs.h"

#ifdef RTOS_MODE

void buttons_cb() {
	for(int i = INPUT_A; i < INPUT_DOWN; i++) {
		if(gpio_get(i) == GPIO_IRQ_LEVEL_LOW) {
			byte = ( byte | ( 0x80 >> (i-INPUT_A) ) );
		}
	} 
}

void button_task(void* pvParameters) { 
	xButtonQueue = xQueueCreate( 10, sizeof(uint8_t) );

	for(int i = INPUT_A ; i < INPUT_DOWN ; i++) {
		//isr initializer
		gpio_set_pulls(i, false, true); //set a pull-down.
		gpio_set_irq_enabled_with_callback(i, GPIO_IRQ_EDGE_FALL, true, buttons_cb);
	}

	for( ;; ) { 

		vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_US));

		if(byte != 0x00) {
			xQueueSendToBack(xButtonQueue, &byte, portMAX_DELAY); //verify its deep copied
			byte = 0x00; //reset
		} else {}

	}
}

#endif //RTOS_MODE
