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

	display_setup(); //spi setup, driver initializized, dma data chan setup.
	
	//Layer things, sprite things...
	
	while(true) {
		
	}
}
