//an extension interface, or something
#include "TCP_Routines.h"

//#include <cstdio>

#include "WifiHandler.h"
#include "TCPHandler.h"

TCP_Routines::TCP_Routines() {
}
TCP_Routines::~TCP_Routines() {
}

void TCP_Routines::run() {
  test(); //agent thread will run tcp routines through this method
  printf("RUN %d TESTS; %d SUCCESFUL \n");

  while(true) {
    vTaskDelay(5000); //busy loop forever
  }
}

configSTACK_DEPTH_TYPE TCP_Routines::getMaxStackSize() {
  return 5000; //tweak for your purposes..
}

void TCP_Routines::test() {
  xTests++;
  if(!testConnect()) {
    printf("CYW CONNECTION FAILED\n");
  } else {
    xSuccessful++;
  }
  /*
  xTests++;
  if(!testTCP()) {
    printf("TCP CONNECTION FAILED\n");
  } else {
    xSuccessful++;
  }
  */
}

bool TCP_Routines::testConnect() { //i expect that this method should pass consistently
  //check if joined already; if so, then pass this test; if no, then evaluate if canjoin, then pass or fail... simpleeeeee
  if(WifiHandler::isJoined()) {
    return true;
  } 
  if(WifiHandler::join(WIFI_SSID,WIFI_PASSWORD)){
    if(WifiHandler::isJoined()) {
      return true;
    } else {
      printf("did not join successfully\n\r");
    }
  } else {printf("CYW Connect failed\n\r");}

  return false;
}

bool TCP_Routines::testSock() {
  //const chr *host, uint16_t port
  if(!TCPHandler::sockConnect(*targetIP,targetPort)){ 
    printf("Socket Connect Failed");
    return false;
  } 

  return true;
}

