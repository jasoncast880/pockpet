#include "TCP_Routines.h"
#include "WifiHelper.h"
#include "TCPTransport.h"

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
  xTests++;
  if(!testTCP()) {
    printf("TCP CONNECTION FAILED\n");
  } else {
    xSuccessful++;
  }
}

bool TestTrans::testConnect() { //i expect that this method should pass consistently
  //check if joined already; if so, then pass this test; if no, then evaluate if canjoin, then pass or fail... simpleeeeee
  if(WifiHelper::isJoined()) {
    return true;
  } 
  if(WifiHelper::join(WIFI_SSID,WIFI_PASSWORD)){
    if(WifiHelper::isJoined()) {
      return true;
    } else {
      printf("did not join successfully\n\r");
    }
  } else {printf("Connect failed\n\r");}

  return false;
}

bool TestTrans::testTCP() {
  uint16_t targetPort = 80;
  char targetHost[]="drjonea.co.uk"; //temp
  char message[]="GET / HTTP/1.1\r\n"
    "Host: drjonea.co.uk\r\n"
    "Connection: close\r\n"
    "\r\n";
  char buf[1024]; //this appears to be the returned buffer. why size 1024??
  int32_t retVal;

  TCPTransport sockTrans;

  if(!sockTrans.transConnect(targetHost, targetPort)){
    printf("Socket Send failed\n\r");
    return false;
  }

  retVal = 1;

  while(retVal>0){
    retVal = sockTrans.transRead(buf, sizeof(buf));
    if(retVal > 0) {
      sockTrans.debugPrintBuffer("READ:", buf, retVal);
    }
  }

  sockTrans.transClose();
  
  return true;
}
