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
#include "ST7735_TFT.hpp"
#include "ST7735_TFT_Assets.hpp"
#include <string>

#define RED    0xf800
#define GREEN  0x07e0

#define FILTER_BG 0xf81f //pure 565 purple is the filter bg

#define BUTTON_LEFT 2
#define BUTTON_DOWN 3
#define BUTTON_UP   4
#define BUTTON_RIGHT 5

//globals for isr signaling
bool leftState = false;
bool downState = false;
bool upState   = false;
bool rightState = false;

ST7735_TFT myTFT;

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
  myTFT.TFTdrawBitmap16Data(0,0,(uint8_t*)pRed,128,160); //160 x 128
}

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

 bool testVar = false;
  while(true) {

    runTimeStats(); 

    vTaskDelay(3000);

    if(!testVar) {
      myTFT.TFTfillScreen(RED);
      testVar = !testVar;
      vTaskDelay(500);
    } else {
      myTFT.TFTfillScreen(GREEN);
      testVar = !testVar;
      vTaskDelay(500);
    }
  }
}

void display_task(void* params) {
  if(leftState){ printf("L\n"); }
  if(downState){ printf("D\n"); }
  if(upState){ printf("U\n"); }
  if(rightState){ printf("R\n"); }

  leftState = false;
  downState = false;
  upState = false;
  rightState = false;
}

void vLaunch(void) {
  TaskHandle_t task;

  //task handles (ie &main_task is set to NULL) handles are not relevant right now.
  xTaskCreate(main_task,"Main_Task", 2048, NULL, TASK_PRIORITY, NULL); 
  xTaskCreate(display_task,"Display_Task", 2048, NULL, TASK_PRIORITY-1, NULL);

  vTaskStartScheduler();
}

void leftButton_isr(uint gpio, uint32_t events) {leftState = true;}
void downButton_isr(uint gpio, uint32_t events) {downState = true;} 
void upButton_isr(uint gpio, uint32_t events) {upState = true;} //try without sw timers
void rightButton_isr(uint gpio, uint32_t events) {rightState = true;}

int main() {
  //initialization routines
  stdio_init_all();
  tftSetup();

  gpio_init(BUTTON_LEFT);
  gpio_set_dir(BUTTON_LEFT,GPIO_IN);
  gpio_pull_up(BUTTON_LEFT);

  gpio_init(BUTTON_DOWN);
  gpio_set_dir(BUTTON_DOWN,GPIO_IN);
  gpio_pull_up(BUTTON_DOWN);
  
  gpio_init(BUTTON_UP);
  gpio_set_dir(BUTTON_UP,GPIO_IN);
  gpio_pull_up(BUTTON_UP);
 
  gpio_init(BUTTON_RIGHT);
  gpio_set_dir(BUTTON_RIGHT,GPIO_IN);
  gpio_pull_up(BUTTON_RIGHT);
  
  gpio_set_irq_enabled_with_callback(BUTTON_LEFT, GPIO_IRQ_EDGE_FALL, true, &leftButton_isr);
  gpio_set_irq_enabled_with_callback(BUTTON_DOWN, GPIO_IRQ_EDGE_FALL, true, &downButton_isr);
  gpio_set_irq_enabled_with_callback(BUTTON_UP, GPIO_IRQ_EDGE_FALL, true, &upButton_isr);
  gpio_set_irq_enabled_with_callback(BUTTON_RIGHT, GPIO_IRQ_EDGE_FALL, true, &rightButton_isr);

  sleep_ms(5000);
  printf("GO\n");

  vLaunch();

  return 0;
}

