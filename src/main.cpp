#include <stdio.h>
#include "pico/stdlib.h"

//cyw and lwip includes
//#include "pico/cyw43_arch.h"
#define TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )

//#include "lwip/ip4_addr.h"
//#include "lwip/sockets.h"
//#include "lwip/dns.h"

#include "WifiHandler.h" 
#include "TCP_Routines.h" 

//freertos includes
#include "FreeRTOS.h"
#include "task.h"

//hardware includes
#include "hardware/gpio.h"
#include <string>

#define BUTTON1 2

//class WifiHandler; // Forward declaration

void runTimeStats(){
  TaskStatus_t         * pxTaskStatusArray;
  volatile UBaseType_t uxArraySize, x;
  unsigned long        ulTotalRunTime;


  /* Take a snapshot of the number of tasks in case it changes while this
  function is executing. */
  uxArraySize = uxTaskGetNumberOfTasks();
  printf("Number of tasks %d\n", uxArraySize);

  /* Allocate a TaskStatus_t structure for each task.  An array could be
  allocated statically at compile time. */
  pxTaskStatusArray = (TaskStatus_t*) pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

  if (pxTaskStatusArray != NULL){
    /* Generate raw status information about each task. */
    uxArraySize = uxTaskGetSystemState(pxTaskStatusArray,
                                       uxArraySize,
                                       &ulTotalRunTime);



    /* For each populated position in the pxTaskStatusArray array,
    format the raw data as human readable ASCII data. */
    for (x = 0; x < uxArraySize; x++){
      printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
             pxTaskStatusArray[x].xTaskNumber,
             pxTaskStatusArray[x].uxCurrentPriority,
             pxTaskStatusArray[x].uxBasePriority,
             pxTaskStatusArray[x].usStackHighWaterMark,
             pxTaskStatusArray[x].pcTaskName
      );
    }


    /* The array is no longer needed, free the memory it consumes. */
    vPortFree(pxTaskStatusArray);
  } else{
    printf("Failed to allocate space for stats\n");
  }

  HeapStats_t heapStats;
  vPortGetHeapStats(&heapStats);
  printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
         heapStats.xAvailableHeapSpaceInBytes,
         heapStats.xNumberOfFreeBlocks,
         heapStats.xNumberOfSuccessfulAllocations,
         heapStats.xNumberOfSuccessfulFrees
  );

}

void main_task(void* params) {
  printf("Main Task started\n");

  if(WifiHandler::init()) {
    printf("CYW43 Controller Initialized \n");
  } else {
    printf("Failed CYW43 Initialization \n");
    return;
  } 

  if(WifiHandler::join(WIFI_SSID, WIFI_PASSWORD,10)){
      printf("Connected to WAP : %s \n", WIFI_SSID);
  } else {
    printf("ERROR: Timeout on WAP Connection\n");
    return;
  }

  char macStr[20];
  WifiHandler::getMACAddressStr(macStr);
  printf("MAC ADDRESS: %s\n", macStr);

  //Print IP Address
  char ipStr[20];
  WifiHandler::getIPAddressStr(ipStr);
  printf("IP ADDRESS: %s\n", ipStr);


  TCP_Routines testConnect;
  testConnect.start("test", TASK_PRIORITY);

  while(true) {

    runTimeStats();

    vTaskDelay(3000);

    if(!WifiHandler::isJoined()) {

      printf("AP is down \n");

      if(!WifiHandler::join(WIFI_SSID, WIFI_PASSWORD)){
        printf("Attempt to Connect to Wifi\n");
      } else {
        printf("Failed to connect to Wifi\n");
      }
    } else {
      printf("Wifi is OK\n");
    }
  }
}

void vLaunch(void) {
  TaskHandle_t task;

  xTaskCreate(main_task,"Main_Thread", 2048, NULL, TASK_PRIORITY, NULL); //task handles (ie &main_task is set to NULL) handles are not relevant right now.

  vTaskStartScheduler();
}

int main() {
  stdio_init_all();
  sleep_ms(10000);
  printf("GO\n");

  vLaunch();

  return 0;
}

