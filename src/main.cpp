#include "FreeRTOS.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "task.h"
#include "queue.h"
#include <cstring>

#define BUTTON_PIN_1 2
#define BUTTON_PIN_2 3
#define BUTTON_PIN_3 4

QueueHandle_t button_queue;

void button_task(void *pvParameters)
{
  while(1)
  {
    if(gpio_get(BUTTON_PIN_1) == 0)
    {
      xQueueSend(button_queue, "\nButton 1 Pressed", portMAX_DELAY);
    }
    if(gpio_get(BUTTON_PIN_2) == 0)
    {
      xQueueSend(button_queue, "\nButton 2 Pressed", portMAX_DELAY);
    }
    if(gpio_get(BUTTON_PIN_3) == 0)
    {
      xQueueSend(button_queue, "\nButton 3 Pressed", portMAX_DELAY);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

int menu_value = 0;

void menu_task(void *pvParameters) {
  char button_event[20];
  while (1) {
    if (xQueueReceive(button_queue, &button_event, portMAX_DELAY)) {
      printf("Received event: %s\n", button_event);
      
      // Handle menu navigation based on button events
      if (strcmp(button_event, "\nButton 1 Pressed") == 0)
      {
        menu_value--;
        printf("Menu Value: %d\n", menu_value);
      }

      if (strcmp(button_event, "\nButton 2 Pressed") == 0)
      {
        menu_value++;
        printf("Menu Value: %d\n", menu_value);
      }
      // Update display accordingly
    }
  }
}

int main()
{
  stdio_init_all();

  button_queue = xQueueCreate(10, sizeof(char[20]));

  //buttons init
  gpio_init(BUTTON_PIN_1);
  gpio_set_dir(BUTTON_PIN_1, GPIO_IN);
  gpio_pull_up(BUTTON_PIN_1);

  gpio_init(BUTTON_PIN_2);
  gpio_set_dir(BUTTON_PIN_2, GPIO_IN);
  gpio_pull_up(BUTTON_PIN_2);

  gpio_init(BUTTON_PIN_3);
  gpio_set_dir(BUTTON_PIN_3, GPIO_IN);
  gpio_pull_up(BUTTON_PIN_3);

  xTaskCreate(button_task, "Button Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(menu_task, "Menu Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  vTaskStartScheduler();

  while(1)
  {
    //should not reach here
  };
  
  return 0;
}
