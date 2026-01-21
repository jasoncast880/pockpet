#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pio.h"

#define DEBOUNCE_US 10

#ifdef __cplusplus
extern "C" {
#endif 

extern uint8_t btn_sample;

void button_setup();
void system_button_handler(uint gpio, uint32_t events); //for special system-level interrupts: start/sel buttons


#ifdef __cplusplus
}
#endif 

#endif //BUTTONS_H
