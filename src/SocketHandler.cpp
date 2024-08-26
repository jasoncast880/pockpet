#include "SocketHandler.h"

#include <stdlib.h>
#include "pico/stdlib.h"
#include <errno.h>


#define DEBUG_LINE 25

SocketHandler::SocketHandler() {
  //NOP
}

SocketHandler::~SocketHandler() {
  //NOP
}

/*
//when a socket connection is made, print the print and hex data in a buffer.
void SocketHandler::debugPrintBuffer(const char *title, const void *pBuffer, size_t bytes){
  
  size_t count = 0;
  size_t lineEnd = 0;
  const uint8_t *pBuf = (uint8_t *)pBuffer;

  printf("DEBUG: %s of size %d\n", title, bytes);

  while(count < bytes) {
    printf("*");
    count++;
  }

}
*/

