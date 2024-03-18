/* test3/main.c */
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <queue.h>
#include "pico/stdlib.h"
#include "ST7735_TFT.hpp"
#include "ST7735_TFT_Assets.hpp"
#include "semphr.h"

SemaphoreHandle_t count;

void tft_task(void *pvParameters){}
void speaker_driver_task(void *pvParameters){}
void rtc_task(void *pvParameters){}

void led_task(void *pvParameters)
{   
    const uint LED_PIN = 25; // gp25 for pico, gp100 for pico w
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) 
    {
      if(xSemaphoreTake(count, (TickType_t) 10 == pdTRUE))
      {
        gpio_put(LED_PIN, 1);
        vTaskDelay(100);
      }
      else
      {
        gpio_put(LED_PIN, 0);
        vTaskDelay(1);
      }
    }
}

void button_task(void *pvParameters) 
{
  gpio_init(20);
  gpio_set_dir(20, GPIO_IN);

  while(true)
  {
    if(gpio_get(20) !=0)
    {
      xSemaphoreGive(count);
      vTaskDelay(20);
    }
  }
}


int main()
{
    stdio_init_all();

    count = xSemaphoreCreateCounting(5,0);

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(button_task, "Button_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}
