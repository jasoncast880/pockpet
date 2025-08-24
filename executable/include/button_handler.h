#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "hardware/gpio.h"
#include "hardware/irq.h"

#include "FreeRTOS.h"
#include "queue.h"


#ifdef __cplusplus
extern "C" {
#endif 

//SETUP FUNCTION

#define NUM_BUTTONS 8
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {BTN_A, BTN_B, BTN_START, BTN_SELECT, BTN_LEFT, BTN_UP, BTN_RIGHT, BTN_DOWN};

void button_setup();


extern QueueHandle_t xButtonQueue;

void gpio_irq_handler(void );
void buttons_queue_task( void *pvParameters );

#ifdef __cplusplus
}
#endif 

#endif //BUTTONHANDLER_H
