/* test4/main.c */
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
            .hour  = 59,
            .min   = 59,
            .sec   = 59
};
  
#define QUEUE_LENGTH 5
#define ITEM_SIZE sizeof(t)

//rtc things
char datetime_buf[256];
char *datetime_str = &datetime_buf[0];

QueueHandle_t myQueue;

void init_rtc(&t) //for now the timeset is in the initializer; 
{
  rtc_init();
  rtc_set_datetime(&t);
}
uint32_t get_rtc()
{
  rtc_get_datetime(&t);
  return datetime_tostr(datetime_str, sizeof(datetime_buf), &t);
}

void initQueue() 
{
  myQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);
  if(myQueue == NULL)
  {
    //queue creation has failed
    printf("Queue creation failed");
  }
}
/*
void buttonTask(){}
void sound_driverTask(){}
*/
void displayTask(void *pvParameters) //displayTask is a queue consumer
{
  int timeReceived;
  while(1) 
  {
    if(xQueueReceive(myQueue, &timeReceived, portMAX_DELAY) != pdPASS)
    {
      printf("data failed to be received");
    }
    printf("time right now: %d\n", timeReceived);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void rtcTask(void *pvParameters) //queue producer
{
  uint32_t timeToSend = get_rtc();
  while(1) 
  {
    if(xQueueReceive(myQueue, &timeToSend, portMAX_DELAY) != pdPASS)
    {
      printf("data failed to be sent");
    }
  }
 
}

int main()
{
  stdio_init_all();
  init_rtc(0);

  myQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);
  
  xTaskCreate(displayTask, "displayTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
  xTaskCreate(rtcTask, "rtcTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);

  vTaskStartScheduler();
  return 0;
}

/*
 * Expected output: time right now: (some number)
*/
