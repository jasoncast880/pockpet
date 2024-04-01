#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <queue.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

datetime_t t = 
{
    .year  = 2024,
    .month = 01,
    .day   = 01,
    .dotw  = 1, // 0 is Sunday, so 5 is Friday
    .hour  = 23,
    .min   = 59,
    .sec   = 59
};

#define QUEUE_LENGTH 5

//rtc things
char datetime_buf[256];
char *datetime_str = &datetime_buf[0];

QueueHandle_t myQueue;

void init_rtc(datetime_t *t) 
{
    rtc_init();
    rtc_set_datetime(t);
}

uint32_t get_rtc()
{
    rtc_get_datetime(&t);
    return ((uint32_t)t.year * 100000000UL) + ((uint32_t)t.month * 1000000UL) +
           ((uint32_t)t.day * 10000UL) + ((uint32_t)t.hour * 100UL) +
           ((uint32_t)t.min);
}

void initQueue() 
{
    myQueue = xQueueCreate(QUEUE_LENGTH, sizeof(uint32_t));
    if(myQueue == NULL)
    {
        //queue creation has failed
        printf("Queue creation failed");
    }
}

void displayTask(void *pvParameters) //displayTask is a queue consumer
{
    uint32_t timeReceived;
    while(1) 
    {
        if(xQueueReceive(myQueue, &timeReceived, portMAX_DELAY) != pdPASS)
        {
            printf("data failed to be received");
        }
        datetime_t t = {
            .year = timeReceived / 100000000UL,
            .month = (timeReceived % 100000000UL) / 1000000UL,
            .day = (timeReceived % 1000000UL) / 10000UL,
            .hour = (timeReceived % 10000UL) / 100UL,
            .min = timeReceived % 100UL,
            .sec = 0 // assuming seconds are not part of the timeReceived
        };
        datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
        printf("time right now: %s\n", datetime_str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void rtcTask(void *pvParameters) //queue producer
{
    uint32_t timeToSend;
    while(1) 
    {
        timeToSend = get_rtc();
        if(xQueueSend(myQueue, &timeToSend, portMAX_DELAY) != pdPASS)
        {
            printf("data failed to be sent");
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Adjust delay as needed
    }
}

int main()
{
    stdio_init_all();
    init_rtc(&t);
    initQueue();
  
    xTaskCreate(displayTask, "displayTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
    xTaskCreate(rtcTask, "rtcTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);

    vTaskStartScheduler();
    return 0;
}

