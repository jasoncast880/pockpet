//all serial interfaces are OK
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
  printf("Running Test:");
  test(); //agent thread will run tcp routines through this method
  //printf("RUN %d TESTS; %d SUCCESFUL \n");

  while(true) {
    vTaskDelay(5000); //busy loop forever
  }
}

configSTACK_DEPTH_TYPE TCP_Routines::getMaxStackSize() {
  return 5000; //tweak for your purposes..
}

void TCP_Routines::test() {
  printf("CHECKING WIFI:");
  if(!testConnect()) {
    printf("CYW CONNECTION FAILED\n");
  } else {
    printf("CYW CONNECTION OK \n");
  }

  printf("CHECKING TCP:");
  if(!testSock()) {
    printf("TCP CONNECTION FAILED\n");
  } else {
    printf("TCP CONNECTION OK \n");
  }
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
  } else {
    printf("CYW Connect failed\n\r");
  }

  return false;
}

bool TCP_Routines::testSock() {
  char targetIP[] = EC2_IP; //configure target IP; should be an elastic ip if using ec2
  uint16_t targetPort = 8081;

  TCPHandler ecSock;
  //const char* host, uint16_t port
  if(ecSock.sockConnect(targetIP,targetPort)<0){ 
    printf("Socket Connect Failed"); //delegate all printfs to the thread classes, not the Reentrant functions....
  } else {
    printf("Socket Connection OK");
  }
  
  return true;
}
