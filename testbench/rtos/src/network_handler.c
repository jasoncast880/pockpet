#include "network_handler.h"

static async_context_freertos_t network_async_context;

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
    if (cyw43_arch_init()) {
        vTaskDelete(NULL);
    }

    while (1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
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

