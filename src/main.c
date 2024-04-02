#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"


void led_task()
{   
    const uint LED_PIN = 100;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        printf("onn");
        vTaskDelay(100);
        gpio_put(LED_PIN, 0);
        printf("off");
        vTaskDelay(100);
    }
}

int main()
{
    stdio_init_all();

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}
