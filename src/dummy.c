#include <hardware/gpio.h>
#include <pico/platform/common.h>
#include <pico/stdio.h>
#include <stddef.h>
#include <hardware/spi.h>
#include <hardware/pwm.h>
#include <stdio.h>
#include "pico/stdlib.h" 
#include "pinout.h"
#include "graphics_conf.h"

#include "display.h"

//DUMMY.CPP PURPOSE: Test drivers, services without RTOS bloat/interference.

int main() {
	stdio_init_all();
	sleep_ms(5000);
	printf("GO");

	gpio_set_function(2, GPIO_FUNC_PWM);
	uint slice = pwm_gpio_to_slice_num(2);

	pwm_set_wrap(slice, 1000);              // frequency control
	pwm_set_chan_level(slice, PWM_CHAN_A, 500); // 50% duty cycle

	pwm_set_enabled(slice, true);
	
	while(1) {
		tight_loop_contents();
	}
}
