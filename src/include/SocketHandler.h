/*
 * GOAL: The superclass for tcp_handler
 * implementation 
 *
 */

#ifndef _SOCKET_HANDLER_H_
#define _SOCKET_HANDLER_H_

extern "C" {
  #include <FreeRTOS.h>
  #include <task.h>
  #include <semphr.h>

  #include "lwip/ip4_addr.h"
  #include "lwip/ip_addr.h"
  #include "lwip/sockets.h"

//for now, assume to use direct IP
//#include "lwip/dns.h" 
}

class SocketHandler {
public: // when some virtual func has a = 0 assignment, its supposed to be implemented by subclass
  SocketHandler();
  virtual ~SocketHandler();

  //tcp sock connect 
  virtual bool sockConnect(const char *host, uint16_t port) = 0;

  //status of socket; neg return is an error
//  virtual int status() = 0;

  //close the socket
//  virtual bool sockClose() = 0;

  //send bytes through the socket; 
  //buffer to send from , no of bytes to send.. return the no of bytes sent.
//  virtual int32_t sockSend(const void *pBuffer, size_t bytesToSend) = 0;

  //read bytes through the sock to print them to stdio;
  //title of the print, ptr to buffer, no. bytes in the buffer .. 
//  virtual int32_t sockRead(void *pBuffer, size_t bytesToRecv) = 0;

  //print out to stdio; plain text and hex dump
  //title, buffer to print, no bytes in buffer.
  void debugPrintBuffer(const char *title, const void *pBuffer, size_t bytes);
};

#endif /* _SOCKET_HANDLER_H_ */


