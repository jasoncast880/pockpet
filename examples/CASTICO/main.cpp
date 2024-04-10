#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/rand.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Assets.hpp"
#include <string>

#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

/* global vars, stored data*/
ST7735_TFT myTFT;

/* setup fxn for TFT screen */
void tftSetup(void)
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
}

void RenderBg()
{
  myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 140);
  sleep_ms(500);
  //TFT_MILLISEC_DELAY(TEST_DELAY2);
}
//random num hashing fxn; pass ints as floats, process, then cast it back into int afterwards
float scale(float x,float a, float b, float min, float max)
{
    return (b-a)*(x-min)/(max-min) + a;
}
//global relative dist. vars
const int xMaxVal = 65;
int xPos = 0;

#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4

void EatAnimation();

void pollButton3()
{
  if(gpio_get(BUTTON3)==0)
  {
    printf("b3 pressed\n"); 
    EatAnimation();  
  }
  else
  {
    printf("b3 np\n"); 
  }
}
void WalkAnimation() 
{
  RenderBg();
  while(1)
  {
    const uint8_t* spriteArrFwd[4] = {poreo1,poreo2,poreo1,poreo3};
    const uint8_t* spriteArrBck[4] = {poreo4,poreo5,poreo4,poreo6};
    int idx = 0; //index for sprite arrays 

    //go a random dist forward
    uint32_t r = get_rand_32();
    int dist =int(scale(r,15,65,0,UINT32_MAX));
    printf("\nfwd %d", dist);
    for(int i=0;i<=dist;i++)
    {    
      if(gpio_get(BUTTON3)==0)
      {
        printf("b3 pressed\n"); 
        EatAnimation();  
      }
      myTFT.TFTdrawBitmap16Data(xPos, 54, (uint8_t*)spriteArrFwd[idx], 63, 53);
      idx = idx >= 3 ? 0 : idx+1; 
      xPos+=1;
      if (xPos >= 65) 
      {
        printf("\nchanging dirs back");
        break;
      }
      sleep_ms(350);
    }

    idx = 0;
    //stay in place for random num of cycles
    r = get_rand_32();
    dist =int(scale(r,4,12,0,UINT32_MAX));
    printf("\nStay in place for %d", dist);
    for(int i=0;i<=dist;i++)
    {    
      if(gpio_get(BUTTON3)==0)
      {
        printf("b3 pressed\n"); 
        EatAnimation();  
      }
      myTFT.TFTdrawBitmap16Data(xPos, 54, (uint8_t*)spriteArrBck[idx], 63, 53);
      idx = idx == 1 ? 0 : idx+1; 
      sleep_ms(500);
    }
    //

    idx = 0;
    //go a random dist back 
    r = get_rand_32();
    dist =int(scale(r,15,65,0,UINT32_MAX));
    printf("\nback %d", dist);

    for(int i=0;xPos<=xMaxVal;i++)
    {    
      if(gpio_get(BUTTON3)==0)
      {
        printf("b3 pressed\n"); 
        EatAnimation();  
      }
      myTFT.TFTdrawBitmap16Data(xPos, 54, (uint8_t*)spriteArrBck[idx], 63, 53);
      idx = idx >= 3 ? 0 : idx+1; 
      xPos-=1;
      if (xPos <= 10) 
      {
        printf("\nchanging dirs to forward");
        break;
      }
      sleep_ms(350);
    }

    idx = 0;
    //stay in place for random num of cycles
    r = get_rand_32();
    dist =int(scale(r,4,12,0,UINT32_MAX));
    printf("\nstay in place for %d", dist);
    for(int i=0;i<=dist;i++)
    {    
      if(gpio_get(BUTTON3)==0)
      {
        printf("b3 pressed\n"); 
        EatAnimation();  
      }
      myTFT.TFTdrawBitmap16Data(xPos, 54, (uint8_t*)spriteArrFwd[idx], 63, 53);
      idx = idx == 1 ? 0 : idx+1; 
      sleep_ms(500);
    }
  }
}

void EatAnimation()
{
  RenderBg();
  const uint8_t* dogArr[4] = {poreo1,poreo3};
  const uint8_t* spriteArr[6] = {psteak1,psteak2,psteak3,psteak4,psteak5,psteak6};
  int idx = 0;
  for(int i=0;i<6;i++)
  {    
    //pollButton3(1);
    printf("running eat animations");
    myTFT.TFTdrawBitmap16Data(15, 54, (uint8_t*)dogArr[idx], 63, 53);
    myTFT.TFTdrawBitmap16Data(52, 115, (uint8_t*)spriteArr[i], 34, 21);
    idx = idx >= 1 ? 0 : idx+1; 
    sleep_ms(500);
  }
  WalkAnimation();
}

int main()
{
  stdio_init_all();
	tftSetup();
  myTFT.TFTfillScreen(ST7735_WHITE);

  //button setup
  gpio_init(BUTTON1);
  gpio_set_dir(BUTTON1, GPIO_IN);
  gpio_pull_up(BUTTON1);
    
  gpio_init(BUTTON2);
  gpio_set_dir(BUTTON2, GPIO_IN);
  gpio_pull_up(BUTTON2);

  gpio_init(BUTTON3);
  gpio_set_dir(BUTTON3, GPIO_IN);
  gpio_pull_up(BUTTON3);

  //run the toma screen
  EatAnimation();
  
  while(1)
  {
    //should never get here
    printf("\n running");
    sleep_ms(2000);
  }
  return 0;
}
