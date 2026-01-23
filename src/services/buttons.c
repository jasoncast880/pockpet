#include <hardware/gpio.h>
#include <hardware/timer.h>
#include <pico/time.h>
#include <stdio.h>

#include <pico/types.h>
#include "buttons.h"
#include "pinout.h"
//assmbled program and associated initializer-c-function

volatile uint32_t time_last_sampled[8] = {};
volatile uint8_t btn_sample = 0b00000000;

void button_setup() {
	for( int i = (int)BTN_B; i <= (int)BTN_DOWN ; i++ ) {
		gpio_set_irq_enabled_with_callback(
			i,
			GPIO_IRQ_EDGE_FALL,
			true,
			system_button_handler
		);
	}
	
	struct repeating_timer timer;
	add_repeating_timer_ms(500, clear_sample_timer, NULL, &timer);
}

void system_button_handler(uint gpio, uint32_t events) {
	uint32_t last_time = time_last_sampled[gpio-2];
	if( (time_us_32()-last_time) >= DEBOUNCE_US ) {
		last_time = time_us_32();
		btn_sample = btn_sample | (1u << ((uint8_t)gpio - 1));
	} else {}
}

bool clear_sample_timer(__unused repeating_timer_t *t) {
	btn_sample = 0b00000000;
	return true;
}


