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
    resp = cyw43_arch_wifi_connect_timeout_ms(sid, password, CYW43_AUTH_WPA2_AES_PSK, 5000); //returns 0 if successful

    if(resp) {
      printf("Reattempting Connection\n");
    } else return true;
    vTaskDelay(2000);
  }
  return false;
  //print telemetry in main.
}

bool WifiHelper::isJoined() {
  int res = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
  return(res>=0);
}

bool WifiHelper::getIPAddress(uint8_t *ip) {
  memcpy(ip, netif_ip4_addr(&cyw43_state.netif[0]), 4);
  return true;
}

bool WifiHelper::getIPAddressStr(char *ips) {
  char *s = ipaddr_ntoa(netif_ip4_addr(&cyw43_state.netif[0]));
  strcpy(ips, s);
  return true;
}

bool WifiHelper::getGWAddress(uint8_t *ip) {
  memcpy(ip, netif_ip4_gw(&cyw43_state.netif[0]), 4);
  return true;
}

bool WifiHelper::getGWAddressStr(char *ips) {
  char *s = ipaddr_ntoa(netif_ip4_gw(&cyw43_state.netif[0]));
  strcpy(ips, s);
  return true;
}

bool WifiHelper::getNetMask(uint8_t *ip) {
  memcpy(ip, netif_ip4_netmask(&cyw43_state.netif[0]), 4);
  return true;
}

bool WifiHelper::getNetMaskStr(char *ips) {
  char *s = ipaddr_ntoa(netif_ip4_netmask(&cyw43_state.netif[0]));
  strcpy(ips, s);
  return true;
}

bool WifiHelper::getMACAddressStr(char *macStr) {
  uint8_t mac[6];
  int r = cyw43_wifi_get_mac(&cyw43_state, CYW43_ITF_STA, mac);

  if(r==0) {
    for(uint8_t i = 0 ; i<6; i++) {
      if(mac[i] < 16) {
        sprintf(&macStr[i*2], "0%X", mac[i]);
      } else {
        sprintf(&macStr[i*2], "%X", mac[i]);
      }
    }
    macStr[13] = 0;
    return true;
  }
  return false;
}
