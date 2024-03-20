// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Assets.hpp"

#include <string>

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// push button
#define BUTTON1_GPIO 2
#define BUTTON2_GPIO 3
#define BUTTON3_GPIO 4

ST7735_TFT myTFT;

int myBars = 2;
void Setup(void);	// setup + user options

// open backgrounds and static components
void RenderMenu(void);

//various menu animations
void ShowBars(int myBars);
void updateButtons();
void lightButtons(int n);

int main(void) 
{
	Setup();
  //implement state machines here
  
  //RenderBg();
  //ButtonHandler();
  int buttonPress = 0;
  //implement a second core to handle the button flashing
  RenderMenu(); //render static components
  //end of initialization core0 scheme
  
  while(true)
  {
    if(!gpio_get(BUTTON1_GPIO)){
      printf("button1 pressed");
      updateButtons();
      //do gpio1 things
      buttonPress-=1;
      buttonPress = (buttonPress<=0) ? 2 : buttonPress;
      lightButtons(buttonPress);

      sleep_ms(1200);
    }
    if(!gpio_get(BUTTON2_GPIO)){
      printf("button2 pressed");
      updateButtons();
      //do gpio2 things
      buttonPress+=1;
      //use the commented out code when u ready to add more buttons
      buttonPress = (buttonPress>2) ? 1 : buttonPress;
      //buttonPress = (buttonPress>4) ? 4 : buttonPress;
      lightButtons(buttonPress);
      
      sleep_ms(1200);
    }
    if(!gpio_get(BUTTON3_GPIO)){
      printf("button3 pressed");
      myTFT.TFTfillScreen(0xa548);
      sleep_ms(1200);
      //break;
    }
  }
}

void Setup(void)
{
	stdio_init_all(); // Initialize chosen serial port
	TFT_MILLISEC_DELAY(TEST_DELAY1);
	printf("TFT :: Start\r\n");

	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi,

	if (bhardwareSPI == true)
	{								   // hw spi
		uint32_t TFT_SCLK_FREQ = 8000; // Spi freq in KiloHertz , 1000 = 1Mhz , max 62500
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
	}
	else
	{								 // sw spi
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


  //buttons setup
  gpio_init(BUTTON1_GPIO);
  gpio_set_dir(BUTTON1_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON1_GPIO);

  gpio_init(BUTTON2_GPIO);
  gpio_set_dir(BUTTON2_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON2_GPIO);
  
  gpio_init(BUTTON3_GPIO);
  gpio_set_dir(BUTTON3_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON3_GPIO);
}

void RenderBg(void)
{
  myTFT.TFTfillScreen(ST7735_WHITE);
  myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 140);
  TFT_MILLISEC_DELAY(TEST_DELAY2);
  //WalkAnimation();
}

void RenderMenu(void)
{
  printf("rendering menu");
  myTFT.TFTfillScreen(0xa548);
  myTFT.TFTdrawBitmap16Data(1, 1, (uint8_t *)poreo_portrait, 81, 97);
  myTFT.TFTdrawBitmap16Data(17, 100, (uint8_t *)pguage_bar, 62, 14);
  myTFT.TFTdrawBitmap16Data(17, 116, (uint8_t *)pguage_bar, 62, 14);
  myTFT.TFTdrawBitmap16Data(2, 100, (uint8_t *)pheart_icon, 14, 13);
  myTFT.TFTdrawBitmap16Data(2, 116, (uint8_t *)ppaw_icon, 14, 14);
  myTFT.TFTdrawBitmap16Data(0, 133, (uint8_t *)pbottom_menu, 128, 27);
  ShowBars(myBars);
  updateButtons();
}

void ShowBars(int numBars)
{
  myTFT.TFTfillRect(22,102,52,10,0xd68b);
  for (int i = 0; i < numBars; i++) {
    int offset = i*13;
    myTFT.TFTdrawBitmap16Data(22+offset, 102, (uint8_t *)pbar1,13,10);
  }
}

void updateButtons() //draw over buttons to reset blinking effect
{
  myTFT.TFTdrawBitmap16Data(84,12, (uint8_t *)phome_button1,42,27);
  myTFT.TFTdrawBitmap16Data(84,42, (uint8_t *)pfood_button1,42,27);
} 

void lightButtons(int n)
{
  if (n == 1) 
  {
    myTFT.TFTdrawBitmap16Data(84,12, (uint8_t *)phome_button2,42,27);
    //draw over 1
  }
  else if (n == 2)
  {
    myTFT.TFTdrawBitmap16Data(84,42, (uint8_t *)pfood_button2,42,27);
    //draw 2
  }
}


