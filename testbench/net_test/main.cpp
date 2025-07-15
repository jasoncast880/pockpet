#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/ip4_addr.h"

#include "FreeRTOS.h"
#include "task.h"

#include "secrets.h"

#ifndef PING_ADDR
#define PING_ADDR "142.251.35.196"
#endif

#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define MAIN_TASK_STACK_SIZE    (1024)

void ping_init(const ip_addr_t *addr);

void main_task(__unused void *params) {
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        exit(1);
    } else {
        printf("Connected.\n");
    }

    while(true) {
        // not much to do as LED is in another task, and we're using RAW (callback) lwIP API
        vTaskDelay(100);
    }

    cyw43_arch_deinit();
}

void vLaunch( void) {
    //TaskHandle_t task;
    xTaskCreate(main_task, "TestMainThread", configMINIMAL_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL);

    //!!
    vTaskStartScheduler();
}

int main( void )
{
    stdio_init_all();

    sleep_ms(10000);
    printf("GO\n");

//    vLaunch();

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }

    while(1) { tight_loop_contents(); }

    return 0;
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // Optional: print task name
    printf("Stack overflow in task: %s\n", pcTaskName);

    // Optional: halt system
    while (true) {
        __asm volatile ("bkpt #0");  // break into debugger if attached
    }
}
