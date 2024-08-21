#include "TCPHandler.h"

#include <cstring>
#include "pico/stdlib.h"
#include <errno.h>

TCPHandler::TCPHandler(){
  //bind immediately
  xSocketFlag = xSemaphoreCreateBinary();
}

TCPHandler::~TCPHandler(){
  //NOP
}

// using socket() and connect()
//define the server:
//direct ip connection, through port <69> (idk)
//

struct sockaddr_in serv_addr;

bool TCPHandler::sockConnect(const char *host, uint16_t port){
 //bind return 0 on success, -1 to indicate an error

  xSock = socket(AF_INET, SOCK_STREAM, 0);

  if(xSock < 0) {
    printf("ERROR opening socket \n");
    return false;
  }

  //server's (ipv4) IP details; all members defined after socket(.) declaration; 
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(xPort);
  memcpy(&serv_addr.sin_addr.s_addr, &xHost, sizeof(xHost));
  
  int resp = connect(xSock,(struct sockaddr *) &serv_addr, sizeof(serv_addr));

  //check for neg
  if(resp<0) {
    printf("Socket Connection Error\n");
    return false;
  }

  printf("Socket Connection OK\n");
  return true;
}

/*
//get status of socket; return neg is error;
int TCPHandler::status(){
  int error = 0;
  socklen_t len = sizeof(error);
  int ret 
}
*/
