#include <stdio.h>
#include "pico/stdlib.h"

//cyw and lwip includes
#include "pico/cyw43_arch.h"
//#include "lwip/sockets.h"
#define TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )

#include "lwip/ip4_addr.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"

#include "WifiHandler.h" 
#include "TCP_Routines.h" 

//freertos includes
#include "FreeRTOS.h"
#include "task.h"

//hardware includes
#include "hardware/gpio.h"
#include "ST7735_TFT.hpp"
#include <string>

#define BUTTON1 2

//class WifiHandler; // Forward declaration
ST7735_TFT myTFT;

void BoolStatusUpdate();

void tftSetup(void) {
	stdio_init_all(); // Initialize chosen serial port
	TFT_MILLISEC_DELAY(1000);
	printf("TFT :: Start\r\n");

	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi,

	if (bhardwareSPI == true) {								   // hw spi
		uint32_t TFT_SCLK_FREQ = 8000; // Spi freq in KiloHertz , 1000 = 1Mhz , max 62500
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
	}
	else {								 // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.TFTInitSPIType(SWSPICommDelay);
	}
	//**********************************************************

	// ******** USER OPTION 1 GPIO *********
	// NOTE if using Hardware SPI clock and data pins will be tied to
	// the chosen interface (eg Spi0 CLK=18 DIN=19)	int8_t SDIN_TFT = 19; int8_t SCLK_TFT = 18;
  int8_t SDIN_TFT = 19;
  int8_t SCLK_TFT = 18;
	int8_t DC_TFT = 17;
	int8_t CS_TFT = 21;
	int8_t RST_TFT = 16;
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ******
	uint8_t OFFSET_COL = 2;	   // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 1;	   // 3, with manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735S_Black); // pass enum,4 choices,see README
	//**********************************************************

  sleep_ms(500);
}

//globs
bool button_pressed = true;

void lcd_task(void *pvParameters){
  while(1){
    if(button_pressed){
      char teststr1[] = "Hello";
      char teststr2[] = "World";
      char teststr3[] = "v2_1";
      
      myTFT.TFTfillScreen(ST7735_BLACK);
      myTFT.TFTFontNum(myTFT.TFTFont_Default);
      myTFT.TFTdrawText(15, 15, teststr1, ST7735_WHITE, ST7735_BLACK, 2);
      myTFT.TFTdrawText(15, 35, teststr2, ST7735_WHITE, ST7735_BLACK, 2);
      myTFT.TFTdrawText(15, 55, teststr3, ST7735_WHITE, ST7735_BLACK, 2);
    }
    else {
      char teststr1[] = "Goodbye";
      char teststr2[] = "World";
      
      myTFT.TFTfillScreen(ST7735_BLACK);
      myTFT.TFTFontNum(myTFT.TFTFont_Default);
      myTFT.TFTdrawText(15, 15, teststr1, ST7735_WHITE, ST7735_BLACK, 2);
      myTFT.TFTdrawText(15, 35, teststr2, ST7735_WHITE, ST7735_BLACK, 2);
    }
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

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


  TCP_Routines testTrans;
  testTrans.start("test", TASK_PRIORITY);

  while(true) {

    runTimeStats();

    vTaskDelay(3000);

    if(!WifiHandler::join(WIFI_SSID, WIFI_PASSWORD)){
      printf("Connect to Wifi\n");
    } else {
      printf("Failed to connect to Wifi\n");
    }
  }

}

void vLaunch(void) {
  TaskHandle_t task;

  xTaskCreate(main_task,"Main_Thread", 2048, NULL, TASK_PRIORITY, NULL); //task handles (ie &main_task is set to NULL) handles are not relevant right now.

  vTaskStartScheduler();
}

int main()
{
  stdio_init_all();
  sleep_ms(10000);
  printf("GO\n");

  vLaunch();

  return 0;
}

