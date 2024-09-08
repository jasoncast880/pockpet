#include "TCPHandler.h"
#include <cstring>
#include "pico/stdlib.h"
#include <errno.h>

#define MAX_RETRIES 10

TCPHandler::TCPHandler(){
  //NOP??
  //bind immediately
//  xSocketFlag = xSemaphoreCreateBinary();
}


TCPHandler::~TCPHandler(){
  //NOP
}

// using socket() and connect()
//define the server:
//direct ip connection, through port <69> (idk)
//

//int xSock = 0;  HEADER DECLARATION

//DIRECT IP TCP Connection 
//return 1, issue opening socket
//return 0, ok connect to server
//return -1, no connect to server
int TCPHandler::sockConnect(const char* ip, uint16_t port) {
  struct sockaddr_in serv_addr;

  int retries = 0;
  int delay = 1000; //ms for vTaskDelay(x)

  xSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(xSock < 0) {
    printf("Socket Creation Failed");
    return 1;
  }

  //memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  //memcpy(&serv_addr.sin_addr.s_addr, &xHost, sizeof(xHost));
  //
  inet_pton(AF_INET, ip, &serv_addr.sin_addr);
  //
  //serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  
  connect(xSock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  while(retries < MAX_RETRIES) {
    if(connect(xSock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0) {
      printf("TCP Connected Successfuly\n");
      break;
    } else {
      printf("TCP Connect Failed: %s\n", strerror(errno));
      retries++;
      if (retries < MAX_RETRIES) {
        printf("Retrying in %d ms... \n",delay);
        vTaskDelay(delay);
      } else {
        printf("Max Retries Reached. TCP Connect Failed \n");
        close(xSock);
        return -1;
      }
    }
  }
  
  return 0;
}

//const void *pBuffer, size_t bytesToRecv
int32_t TCPHandler::sockRead(const void *pBuffer, size_t bytesToRecv) {
  int32_t dataIn = 0;

  //method below writes direct to buffer space from socket's contents
  dataIn = read(xSock, (uint8_t *) pBuffer, bytesToRecv);
  //putting data from read in dataIn; filter errors and etc.
  //
  if(dataIn < 0) {
    dataIn=0;
  }

  return dataIn;
}


//get status of socket; return neg is error;
int TCPHandler::status() {
  int error = 0;
  socklen_t len = sizeof(error);
  int retVal = getsockopt(xSock, SOL_SOCKET, SO_ERROR, &error, &len);
  return retVal;
}

bool TCPHandler::sockClose(){
  closesocket(xSock);
  return true;
}
