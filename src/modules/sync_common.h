#ifndef SYNC_COMMON_H
#define SYNC_COMMON_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

//for the modules to see their kernel structures, for ease of sync.

//SPI0 - used by modules display/ & sdc/ (WIP)
extern xSemaphoreHandle spi0_sync_t;

#endif //SYNC_COMMON_H
