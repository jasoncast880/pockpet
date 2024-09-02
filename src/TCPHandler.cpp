#include "TCPHandler.h"
#include <cstring>
#include "pico/stdlib.h"
#include <errno.h>

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

struct sockaddr_in serv_addr;

//DIRECT IP TCP Connection.....
int TCPHandler::sockConnect(const char* host, uint16_t port) {
  //bind return 0 on success, -1 to indicate an error
  //xSock - type int
  printf("1\n");
  xSock = socket(AF_INET, SOCK_STREAM, 0);
  printf("1.1\n");

  if(xSock < 0) {
    printf("2\n");
    return xSock;
  }

  printf("3\n");
  //server's (ipv4) IP details; all members defined after socket(.) declaration; 
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(xPort);
  memcpy(&serv_addr.sin_addr.s_addr, &xHost, sizeof(xHost));
  
  int resp = connect(xSock,(struct sockaddr *) &serv_addr, sizeof(serv_addr));

  //check for neg
  if(resp<0) {

    printf("3\n");
    int ret = -1;//temp
    return ret;
  }

  printf("4\n");
  return xSock;
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
