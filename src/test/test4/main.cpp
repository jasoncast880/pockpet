#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <queue.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

//make a superloop that can read through your thingies without using rtos kernel.
//


// Define a structure to hold RTC time and any other relevant data
typedef struct {
    datetime_t datetime; // Using datetime_t type from pico/util/datetime.h
    // Add any other relevant fields here
} RTCData;

// Sender task function
void senderTask(void *pvParameters) {
    QueueHandle_t rtcQueue = (QueueHandle_t)pvParameters;

    while(1) {
        // Fetch current RTC time
        datetime_t currentTime;
        rtc_get_datetime(&currentTime);

        // Create a structure to hold RTC data
        RTCData rtcData;
        rtcData.datetime = currentTime;

        // Send the RTC data through the queue
        xQueueSend(rtcQueue, &rtcData, portMAX_DELAY);

        // Delay or do other tasks as needed
        vTaskDelay(pdMS_TO_TICKS(1000)); // Example: delay for 1 second
    }
}

// Receiver task function
void receiverTask(void *pvParameters) {
    QueueHandle_t rtcQueue = (QueueHandle_t)pvParameters;

    while(1) {
        RTCData rtcData;

        // Receive data from the queue
        if(xQueueReceive(rtcQueue, &rtcData, portMAX_DELAY) == pdPASS) {
            // Process the received RTC data
            datetime_t receivedTime = rtcData.datetime;

            // Do something with the received time
            char datetimeStr[20]; // Adjust the size as needed
            datetime_to_str(datetimeStr, sizeof(datetimeStr), &receivedTime);
            printf("Received time: %s\n", datetimeStr);
        }
    }
}

int main() {
    // Initialize RTC
    stdio_init_all();
    rtc_init();
    

    // Create a queue to pass RTC data
    QueueHandle_t rtcQueue = xQueueCreate(10, sizeof(RTCData));

    // Create sender and receiver tasks
    xTaskCreate(senderTask, "Sender", configMINIMAL_STACK_SIZE, (void *)&rtcQueue, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(receiverTask, "Receiver", configMINIMAL_STACK_SIZE, (void *)&rtcQueue, tskIDLE_PRIORITY + 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    return 0;
}

