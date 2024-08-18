#include "Transport.h"
#include <stdlib.h>
#include <pico/stdlib.h>
#include <errno.h>

#include <stdio.h>
#define DEBUG_LINE 25

Transport::Transport() {
}
Transport::~Transport() {
}

void Transport::debugPrintBuffer(const char *title, const void *pBuffer, size_t bytes) {
  size_t count = 0;
  size_t lineEnd = 0;

  const uint8_t *pBuf = (uint8_t *)pBuffer;
  
  printf("Debug: %s of size %d\n", title, bytes);

  while(count < bytes) {
    lineEnd = count + DEBUG_LINE;
    if (lineEnd > bytes) {
      lineEnd = bytes;
    }

    //print hex dump
    for (size_t i = count; i<lineEnd; i++) {
      if(pBuf[i] <=0x0F) {
        printf("0%X", pBuf[i]);
      } else {
        printf("%X", pBuf[i]);
      }
    }

    //pad for short lines
    size_t pad = (DEBUG_LINE - (lineEnd - count)) * 3;
    for (size_t i = 0; i<pad ; i++) {
      printf(" ");
    }

    //plain text
    for (size_t i = count; i<lineEnd; i++) {
      if((pBuf[i] >=0x20) && (pBuf[i] <= 0x7e)){
        printf("%c", pBuf[i]);
      } else {
      printf(".");
      }
    }

    printf("\n");
    count = lineEnd;
  }
}






