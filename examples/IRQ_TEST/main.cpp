#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "button_interrupt.pio.h" // This header is generated when you compile your PIO assembly code

#define BUTTON_PIN 2

volatile bool buttonPressed = false;

void handle_button_interrupt() {
    buttonPressed = true;
}

int main() {
    stdio_init_all();

    // Initialize PIO
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &button_interrupt_program);
    uint sm = pio_claim_unused_sm(pio, true);
    button_interrupt_program_init(pio, sm, offset, BUTTON_PIN);

    // Set up interrupt handler
    irq_set_exclusive_handler(PIO_IRQ0_BASE + sm, handle_button_interrupt);
    irq_set_enabled(PIO_IRQ0_BASE + sm, true);

    while (true) {
        if (buttonPressed) {
            printf("Button pressed!\n");
            buttonPressed = false; // Reset flag
        }

        sleep_ms(100); // Sleep for a short time to reduce CPU usage
    }

    return 0;
}

