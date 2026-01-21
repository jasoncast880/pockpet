#include <hardware/gpio.h>
#include <hardware/timer.h>
#include <stdio.h>

#include <pico/types.h>
#include "buttons.h"
#include "pinout.h"
//assmbled program and associated initializer-c-function

uint8_t btn_sample = 0; //this flag is the only thing you need to measure user input,
												//assuming it is regularly cleared and properly handled.
uint32_t time_last_sampled;
//0b 0000 0000

void button_setup() {
	for( int i = (int)BTN_B; i <= (int)BTN_DOWN ; i++ ) {
		gpio_set_irq_enabled_with_callback(
			i,
			GPIO_IRQ_EDGE_FALL,
			true,
			system_button_handler
		);
	}

	time_last_sampled = time_us_32();
}

void system_button_handler(uint gpio, uint32_t events) {
	btn_sample = btn_sample | (1u << ((uint8_t)gpio - 1));
	time_last_sampled = time_us_32();
}




