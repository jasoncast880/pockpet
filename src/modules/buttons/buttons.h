#ifndef BUTTONS_H
#define BUTTONS_H

#include <pico/time.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "FreeRTOS.h"
#include "queue.h"

#define DEBOUNCE_US 10

//no need for cpp, just implement interrupt handlers/deffered processing as needed.

typedef enum {
	INPUT_A = BTN_A, // GPIO 02
	INPUT_B, 
	INPUT_START, 
	INPUT_SELECT, 
	INPUT_LEFT, 
	INPUT_UP, 
	INPUT_RIGHT,
	INPUT_DOWN      // GPIO 09
} INPUT;

static uint8_t byte = 0x00;
static uint32_t time_current;

static QueueHandle_t xButtonQueue; //for use in a rtos build

void buttons_setup(); //unused
void buttons_cb();

void button_task( void * pvParameters ); //implement in main

#endif //BUTTONS_H
