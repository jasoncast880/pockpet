#ifndef BUTTONS_H
#define BUTTONS_H

#include <pico/time.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "hardware/timer.h"

#define DEBOUNCE_US 10

#ifdef __cplusplus
extern "C" {
#endif 

extern volatile uint32_t time_last_sampled[8];
extern volatile uint8_t btn_sample;

void button_setup();
void system_button_handler(uint gpio, uint32_t events); 
bool clear_sample_timer(__unused repeating_timer_t *t);

#ifdef __cplusplus
}
#endif 

#endif //BUTTONS_H
