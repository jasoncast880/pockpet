#include "networkHandler.h"

//wether to flash the cyw43 led
#ifndef USE_WLED
#define USE_WLED 1 //use this later one
#endif

//wether to busy wait in the led thread
#ifndef LED_BUSY_WAIT
#define LED_BUSY_WAIT 0
#endif

// Delay between led blinking
#define LED_DELAY_MS 2000

static async_context_freertos_t network_async_ctx;
static bool wifi_connected = false;
static TaskHandle_t network_monitor_task_h = NULL;

static void pico_set_led(bool state){
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
}

static void pico_init_led(void){
    hard_assert(cyw43_arch_init() == PICO_OK);
    pico_set_led(false); //make sure cyw43 is started
}

void blink_task(void *params){
    bool on = false;
    printf("blink_task\n");
    pico_init_led(); //!

    while(1){
        pico_set_led(on); //!
        on = !on;
        vTaskDelay(LED_DELAY_MS);
    }
}

void network_monitor_task(void* params){
    while(1){
        int status = cyw43_wifi_link_status(&cyw43_state,CYW43_ITF_STA);
        if(status!=CYW43_LINK_UP){
            printf("Wifi Disconnected. Attempting Reconnection...\n");
            
            vTaskDelay(pdMS_TO_TICKS(5000)); //retry per 5 s

            int connect = network_handler_connect("BELL873","7CAFE57A5D5D"); //!!!
            if(!connect){ 
                printf("Wifi Connected.\n"); 
                //blink the light
            }
            else{
                printf("Connect Failed: Error Code #%d\n", connect);
            }
        }
    }
}

void network_handler_init(void){
    
    /*
    async_context_freertos_init(&network_async_ctx);

    if(cyw43_arch_init_default_async_context(&network_async_ctx.core){
        while(1){sleep_ms(1000);}
    }
    */
    xTaskCreate(network_monitor_task, "NetworkMonitor",4096,NULL,tskIDLE_PRIORITY+1,&network_monitor_task_h);
    xTaskCreate(blink_task, "WLED_BlinkTask", configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY+1,NULL);
}

int network_handler_connect(const char* SSID,const char* PWD){
    int ret = cyw43_arch_wifi_connect_timeout_ms(
        SSID, PWD,
        CYW43_AUTH_WPA2_AES_PSK,
        3000
    );

    return ret;
}
