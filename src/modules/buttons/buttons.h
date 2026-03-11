#ifndef BUTTONS_H
#define BUTTONS_H

#include <pico/time.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "FreeRTOS.h"
#include "queue.h"

#define DEBOUNCE_US 10

#ifdef __cplusplus
extern "C" {
#endif 

enum INPUT{
	INPUT_A = BTN_A, 
	INPUT_B, 
	INPUT_START, 
	INPUT_SELECT, 
	INPUT_LEFT, 
	INPUT_UP, 
	INPUT_RIGHT,
	INPUT_DOWN
};

static QueueHandle_t xButtonQueue; //for use in a rtos build
static uint8_t xButtonItem; 

void button_setup();
void button_task( void * pvParameters ); //implement in main

void button_handler();

#ifdef __cplusplus
}
#endif 

#endif //BUTTONS_H
