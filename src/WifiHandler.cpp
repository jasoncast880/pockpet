#include "WifiHandler.h"

#include "pico/cyw43_arch.h"

#include "FreeRTOS.h"
#include "task.h"

WifiHandler::WifiHandler() {
  //NOP
}

WifiHandler::~WifiHandler() {
  //NOP
}

//initialize the cyw43 driver, power mgmt settings
bool WifiHandler::init() {
  //Returns 0 if initialization is successful, an error code(-1) otherwise.
  int res = cyw43_arch_init();
  
  if(res){
    return false;
  }

  //cyw43 initialize ok; setup power  mgmt, return ok
  cyw43_wifi_pm(&cyw43_state,CYW43_AGGRESSIVE_PM); //no powersaving because i'm a thavage

  return true;
}

bool WifiHandler::join(const char *sid, const char *password, uint8_t retries) {
  cyw43_arch_enable_sta_mode();
  printf("Attempting to Connect to WAP: %s \n", sid);

  //cyw43 will connect (retries) no of times
  int resp = 1;
  for(int i=0;i<=retries;i++){
    resp = cyw43_arch_wifi_connect_timeout_ms(sid, password, CYW43_AUTH_WPA2_AES_PSK, 60000); //returns 0 if successful

    if(resp) {
      printf("Reattempting Connection");
    } else return true;
    vTaskDelay(2000);
  }
  return false;
  //print telemetry in main.
}


