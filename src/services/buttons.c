#include <stdio.h>

#include <pico/types.h>
#include "buttons.h"
//assmbled program and associated initializer-c-function
#include "buttons.pio.h"

PIO pio;
uint sm;
uint offset;

void button_setup() {
	bool success = pio_claim_free_sm_and_add_program_for_gpio_range(
		&buttons_program,
		&pio,
		&sm,
		&offset,
		2,
		8,
		false
	);
	
	hard_assert(success);

	button_pio_init(pio, sm, offset);

	//pio_sm_put_blocking(); ...
	
	//never kill.
}


