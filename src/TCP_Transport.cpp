#include "TCP_Transport.h"

#include <stdlib.h>
#include "pico/stdlib.h"
#include <errno.h>

TCP_Transport::TCP_Transport() {
  xHostDNSFound = xSemaphoreCreateBinary();
}

TCP_Transport::~TCP_Transport() {
}

int32_t TCP_Transport::transSend(const void *pBuffer, size_t bytesToSend) {
  uint32_t dataOut;

  dataOut = write(xSock,(uint8_t *) pBuffer, bytesToSend);
  if(dataOut != bytesToSend) {
    printf("Send Failed %d\n", dataOut);
  }
  return dataOut;
}

int32_t TCP_Transport::transRead(void * pBuffer, size_t bytesToRecv){
	int32_t dataIn=0;

	dataIn = read(xSock, (uint8_t *)pBuffer, bytesToRecv);

	if (dataIn < 0){
		if (errno == 0){
			dataIn = 0;
		}
	}

	//printf("Read(%d)=%d\n", bytesToRecv, dataIn);

	//printf("transRead(%d)=%d\n", bytesToRecv, dataIn);
	return dataIn;
}

bool TCP_Transport::transConnect(const char * host, uint16_t port){
	err_t res = dns_gethostbyname(host, &xHost, TCPTransport::dnsCB, this);

	strcpy(xHostName, host);
	xPort = port;

	if (xSemaphoreTake(xHostDNSFound, TCP_TRANSPORT_WAIT) != pdTRUE){
		printf("DNS Timeout on Connect: %s, %d", host, res);
		//return false;
	}

	return transConnect();
}

bool TCP_Transport::transConnect(){
	struct sockaddr_in serv_addr;


	xSock = socket(AF_INET, SOCK_STREAM, 0);
	if (xSock < 0){
		printf("ERROR opening socket\n");
		return false;
	}


	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(xPort);
	memcpy(&serv_addr.sin_addr.s_addr, &xHost, sizeof(xHost));

	int res = connect(xSock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if (res < 0){
		char *s = ipaddr_ntoa(&xHost);
		printf("ERROR connecting %d to %s port %d\n",res, s, xPort);
		return false;
	}


	printf("Connect success\n");
	return true;
}

int TCP_Transport::transClose() {
  closesocket(xSock);
  return true;
}

int TCP_Transport::status() {
  int error = 0;
  socklen_t len = sizeof(error);
  int retval = getsockopt(xSock, SOL_SOCKET, SO_ERROR, &error, &len);
  return error;
}

void TCP_Transport::dnsCB(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
  TCP_Transport *self = (TCP_Transport *) callback_arg;
  self->dnsFound(name, ipaddr, callback_arg);
}

void TCP_Transport::dnsFound(const char *name, const ip_addr_t *ipaddr, void *callback_arg){
	memcpy(&xHost, ipaddr, sizeof(xHost));

	printf("DNS Found %s copied to xHost %s\n", ipaddr_ntoa(ipaddr), ipaddr_ntoa(&xHost));
	xSemaphoreGiveFromISR(xHostDNSFound, NULL );
}
