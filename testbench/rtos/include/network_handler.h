#pragma once

#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "pico/async_context_freertos.h" //for freertos compatible async worker threads

#include "FreeRTOS.h"
#include "task.h"

#define BLINK_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define BLINK_TASK_STACK_SIZE    configMINIMAL_STACK_SIZE
#define WORKER_TASK_PRIORITY     ( tskIDLE_PRIORITY + 4UL )
#define WORKER_TASK_STACK_SIZE   configMINIMAL_STACK_SIZE

// Public API
async_context_t* network_context_init(void);
void blink_task(__unused void* param);
void wifi_cnxn_task(__unused void* param);

// Declare the global async worker defined in .c
extern async_at_time_worker_t network_worker_timeout;

