/*
 * GOAL: Encapsulate the transport layer into a class, 
 * then use the object in the agent class. 
 * 
 */

#ifndef _TCP_HANDLER_H_
#define _TCP_HANDLER_H_

#define TCP_WAIT 10000

#include "SocketHandler.h"

extern "C" { //assume direct ip connection
  #include <FreeRTOS.h>
  #include <task.h>
  #include <semphr.h>

  #include "lwip/ip4_addr.h"
  #include "lwip/ip_addr.h"
  #include "lwip/sockets.h"
  #include "lwip/inet.h"

  #include <cstring>
}

class TCPHandler : public SocketHandler {
public:
  TCPHandler();
  virtual ~TCPHandler();

  int sockConnect(const char * ip, uint16_t port);

  int status();
  
  bool sockClose();

//  int32_t sockSend(const void *pBuffer, size_t bytesToSend);

  int32_t sockRead(const void *pBuffer, size_t bytesToRecv);

private:
  
//  bool sockConnect();

  int xSock = 0; //socket no.

  uint16_t xPort = 80;//port to connect to
                      //

  //ip_addr_t xHost = EC2_IP; //remote server ip to connect to

  SemaphoreHandle_t xSocketFlag;
  //SemaphoreHandle_t xHostDNSFound; // semaphore for waiting on dns
};

#endif /* _TCP_HANDLER_H_ */
