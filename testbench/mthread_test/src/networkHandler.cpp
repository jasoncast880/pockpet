#include "networkHandler.h"
#include "secrets.h"

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

static bool wifi_connected = false;
static TaskHandle_t network_connect_task_h = NULL;

const char *SSID;
const char *PWD;

WifiHandler::WifiHandler(){
}

void WifiHandler::pico_set_led(bool state){
    cyw43_arch_lwip_begin();
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
    cyw43_arch_lwip_end();
}

void WifiHandler::pico_init_led(void){
    pico_set_led(false); //make sure cyw43 is started
}

void WifiHandler::blink(){
    bool on = false;
    printf("blink_task\n");
    pico_init_led(); //!

    while(1){
        pico_set_led(on); //!
        on = !on;
        vTaskDelay(LED_DELAY_MS);
    }
}

void WifiHandler::blink_task(void *params){
    static_cast<WifiHandler*>(params)->blink();
}

void WifiHandler::network_connect_task(void* params){
    static_cast<WifiHandler*>(params)->network_handler_connect();
}

void WifiHandler::network_handler_init(void){
    cyw43_arch_lwip_begin();
    // values for ssid-pwd pair in 'secrets/secrets.h'
    printf("handler 1\n");
    SSID = WIFI_SSID; 
    PWD = WIFI_PASSWORD;

    printf("handler 2\n");
    SSID = WIFI_SSID; 
    if (SSID != NULL) {
        printf("SSID: %s\n", SSID);
    } else {
        printf("SSID environment variable not found.\n");
    }

    if (PWD != NULL) {
        printf("PASSWORD: %s\n", PWD);
    } else {
        printf("PWD environment variable not found.\n");
    }
    printf("handler 3\n");
    cyw43_arch_lwip_end();


    printf("handler 4\n");
    xTaskCreate(WifiHandler::network_connect_task, "NetworkMonitor",4096,NULL,tskIDLE_PRIORITY+1,&network_connect_task_h);
    //xTaskCreate(WifiHandler::blink_task, "WLED_BlinkTask", configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY+1,NULL);
}

int WifiHandler::network_handler_connect(){
    while(1){
        cyw43_arch_lwip_begin();
        int status = cyw43_wifi_link_status(&cyw43_state,CYW43_ITF_STA);
        cyw43_arch_lwip_end();

        if(status!=CYW43_LINK_UP){
            printf("Wifi Disconnected. Attempting Reconnection...\n");
            
            vTaskDelay(pdMS_TO_TICKS(5000)); //retry per 5 s

            cyw43_arch_lwip_begin();
            int connect = cyw43_arch_wifi_connect_timeout_ms(
                SSID, PWD,
                CYW43_AUTH_WPA2_AES_PSK,
                30000
            );
            cyw43_arch_lwip_end();

            if(!connect){ 
                printf("Wifi Connected.\n"); 
                vTaskDelay(pdMS_TO_TICKS(250));
                //blink the light
            }
            else{
                printf("Connect Failed: Error Code #%d\n", connect);
            }
        } else { //status == CYW43_LINK_UP
            printf("Wifi Connection Stable\n");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}
