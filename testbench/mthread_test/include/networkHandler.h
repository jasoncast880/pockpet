#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include "pico/stdlib.h"

#include "pico/async_context_freertos.h"

#include "pico/cyw43_arch.h"
#include "lwip/netif.h"

#include "FreeRTOS.h"
#include "task.h"

class WifiHandler {
private:

    int network_handler_connect(); //set & forget, connect to wifi

    //pico w led things
    void pico_set_led(bool state);
    void pico_init_led();
    void blink();

public:
    WifiHandler();  

    void network_handler_init(void); 
    static void network_connect_task(void *params); 
    static void blink_task(void *params);
    
    //sockets!!!!

    //configure tcp socket
    //transmit tcp to stream
    //recieve tcp to stream

    //remember to add a destructor
};

#endif //NETWORKHANDLER_H
