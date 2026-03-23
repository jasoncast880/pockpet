#ifndef USB_H
#define USB_H

#include "class/cdc/cdc_device.h"
#include <cstdint>
#include <stdlib.h>
#include <bsp/board_api.h>
#include <tusb.h>

#include <pico/stdio.h>


void usb_setup();

//weak-linked from descriptors
void tud_cdc_rx_cb(uint8_t itf);
void tud_cdc_tx_complete_cb(uint8_t itf);
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts);

#ifdef RTOS_MODE

#include <FreeRTOS.h>
#include "task.h"
#include "semphr.h"
#include "queue.h"

#endif

//TODO: diagnostics/benchmarking class for each of the main modules. to be outputted on usb task. (ACM 1)
//TODO: shell rough-out (ACM 0)

void banner(int argc, char** argv);
void demo_display(int argc, char** argv);

#ifdef __cplusplus
extern "C" {
#endif 

void usb_task( void* pvParameters );
void shell_task( void* pvParameters );

#ifdef __cplusplus
}
#endif 

#endif //USB_H
