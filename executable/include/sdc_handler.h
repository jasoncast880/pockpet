//consider renaming to filesystem_handler.h
#ifndef  SDCHANDLER_H
#define  SDCHANDLER_H

#include "ff.h"

#include "pinout.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#include <stdexcept>

#ifdef __cplusplus
extern "C" {
#endif

static SemaphoreHandle_t xSDC_semphr;
static QueueHandle_t xFileOps_Queue;

void sdc_read_task(void* pvParameters);
void sdc_access_file_task(void* pvParameters);

#ifdef __cplusplus
}
#endif

void sdc_setup();

#endif  //SDCHANDLER_H
