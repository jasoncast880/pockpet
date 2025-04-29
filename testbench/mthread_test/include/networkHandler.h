#pragma once

#ifdef __cplusplus
extern "C" { 
#endif

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/async_context_freertos.h"

#include "FreeRTOS.h"
#include "task.h"


void network_handler_task(void *params); //check connection && correct if needbe; !!! send blink to the led if connected!!!!
void network_handler_init(void); 
int network_handler_connect(const char* ssid, const char* pwd); //set & forget, connect to wifi

//sockets!!!!

//configure tcp socket
//transmit tcp to stream
//recieve tcp to stream

#ifdef __cplusplus
}
#endif
