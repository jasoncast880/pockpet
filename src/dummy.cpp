#include <cstddef>
#include <hardware/spi.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "display.h"
#include "buttons.h"

#include <ampalaya_tileset_16.h>
#include <tilemaps.h>


//DUMMY.CPP PURPOSE: Test drivers, services without RTOS bloat/interference.

int main() {

	DisplayHandler& display = DisplayHandler::setup();
	button_setup();
	
	while(true) {
		sleep_ms(50);
		int8_t input_flag = btn_sample;

		//stack is setup. relatively thread-safe. display.draw_frame(Layer);
		
	}
}
