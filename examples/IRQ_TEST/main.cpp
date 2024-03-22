#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BUTTON_PIN 2
#define DEBOUNCE_DELAY_MS 20 // Adjust debounce delay as needed

void handle_button_press() {
    printf("Button pressed!\n");
    // Add your code here to handle button press
}

void handle_button_release() {
    printf("Button released!\n");
    // Add your code here to handle button release
}

int main() {
    stdio_init_all();

    // Initialize GPIO
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN); // Enable pull-down resistor

    printf("Hello GPIO Debounce\n");

    // Wait for pull-down to take effect
    sleep_ms(100);

    bool is_button_pressed = false;

    while (true) {
        if (gpio_get(BUTTON_PIN)) {
            // Button is pressed
            if (!is_button_pressed) {
                is_button_pressed = true;
                handle_button_press();
                sleep_ms(DEBOUNCE_DELAY_MS); // Debounce delay
            }
        } else {
            // Button is released
            if (is_button_pressed) {
                is_button_pressed = false;
                handle_button_release();
                sleep_ms(DEBOUNCE_DELAY_MS); // Debounce delay
            }
        }
    }

    return 0;
}

