#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pio.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
extern PIO pio;
extern uint sm;
extern uint offset;
*/

void button_setup();
void system_button_handler(); //for special system-level interrupts: start/sel buttons
void button_handler(); //global 


#ifdef __cplusplus
}
#endif 

#endif //BUTTONS_H
