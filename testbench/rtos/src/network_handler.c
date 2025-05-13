#include "network_handler.h"

static async_context_freertos_t network_async_context;

volatile uint32_t network_scratch_t; 
SemaphoreHandle_t network_mutex_h;

async_context_t* network_context_init(void) {
    async_context_freertos_config_t config = async_context_freertos_default_config();
    config.task_priority = WORKER_TASK_PRIORITY;
    config.task_stack_size = WORKER_TASK_STACK_SIZE;

    if (!async_context_freertos_init(&network_async_context, &config)) {
        return NULL;
    }
    return &network_async_context.core;
}

void blink_task(__unused void* param) {
    while (1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void wifi_cnxn_task(__unused void* param) {
    printf("initializing cyw43 stack:\n");
    vTaskDelay(2000);
    if (cyw43_arch_init()) {
        printf("failed to initialize\n");
        return;
        //vTaskDelete(NULL);
    }

    printf("Connecting to WiFi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(SSID, PWD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        exit(1);
    } else {
        printf("Connected.\n");
    }
    
    xTaskCreate(blink_task, "blink", BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    while(1) {
        vTaskDelay(100);
    }
}

static void network_do_work(async_context_t* context, async_at_time_worker_t* worker) {
    async_context_add_at_time_worker_in_ms(context, worker, 10000);
    static uint32_t count = 0;
    printf("Hello from worker count=%u\n", count++);
}

async_at_time_worker_t network_worker_timeout = {
    .do_work = network_do_work
};

