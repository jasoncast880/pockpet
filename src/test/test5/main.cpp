/*
//test5
#include <stdio.h>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

void tomaScreen()
{
  printf("Loading tomascreen");
  void gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

  while(1)
  {
    printf("\nrunning tomascreen event");
}
  

// Function to initialize RTC and set datetime
void initialize_RTC() {
    // Start on Friday 5th of June 2020 15:45:00
    datetime_t t = {
            .year  = 2020,
            .month = 06,
            .day   = 05,
            .dotw  = 5, // 0 is Sunday, so 5 is Friday
            .hour  = 15,
            .min   = 45,
            .sec   = 00
    };

    // Start the RTC
    rtc_init();
    rtc_set_datetime(&t);

    // clk_sys is >2000x faster than clk_rtc, so datetime is not updated immediately when rtc_get_datetime() is called.
    // The delay is up to 3 RTC clock cycles (which is 64us with the default clock settings)
    sleep_us(64);
}

// Function to print time
void display_time() {
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];

    while (true) {
        datetime_t t;
        rtc_get_datetime(&t);
        datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
        printf("\r%s      ", datetime_str);
        sleep_ms(100);
    }
}

void display_tasks() 

int main() {
    stdio_init_all();
    printf("Hello RTC!\n");

    initialize_RTC();
    print_time();

    return 0;
}
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Task function prototypes
void task1();
void task2();

// GPIO interrupt handler
void gpio_callback(uint gpio, uint32_t events) {
    // Perform context switching based on GPIO events
    if (events & GPIO_IRQ_EDGE_RISE) {
        task1();  // Switch to task 1
    } else if (events & GPIO_IRQ_EDGE_FALL) {
        task2();  // Switch to task 2
    }
}

// Task 1 function
void task1() {
    printf("Task 1 executed.\n");
    // Additional task logic here
}

// Task 2 function
void task2() {
    printf("Task 2 executed.\n");
    // Additional task logic here
}

int main() {
    stdio_init_all();

    // Initialize GPIO and set up interrupts
    gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // This point is never reached because tasks handle program flow
    return 0;
}

