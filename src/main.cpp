/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "smphr.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

/*
 * Include Later when intergrating the tft lib
#include "pico/rand.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Assets.hpp"
#include <string>
*/

// push buttons
#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4

SemaphoreHandle_t buttonSemaphore

void buttonTask(void* pvParameters) {
  gpio_init(BUTTON1);
  gpio_set_dir(BUTTON1, GPIO_IN);
  gpio_pull_up(BUTTON1);

  while(1) {
    if(gpio_get(BUTTON1) == 0){
      xSemaporeGive(BUTTON1);
      while(gpio_get(BUTTON1) == 0);
    }
  vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void speakTask(void* pvParameters) {
  while(1) {
    if(xSemaphoreTake(buttonSemaphore, portMAX_DELAY) {
      printf("Button1 Pressed!\n);
    }
  }
}

int main() {
  stdio_init_all();
  buttonSemaphore = xSemaphoreCreateBinary();
    
  xTaskCreate(buttonTask, "ButtonTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
  xTaskCreate(speakTask, "SpeakTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL);

  vTaskStartScheduler();
  return 0;
}

