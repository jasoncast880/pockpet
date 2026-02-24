#ifndef USB_H
#define USB_H

#include <stdlib.h>
#include <bsp/board_api.h>
#include <tusb.h>

#include <pico/stdio.h>



#ifdef __cplusplus
extern "C" {
#endif 

void usb_setup();

#ifdef __cplusplus
}
#endif 

#endif //USB_H
