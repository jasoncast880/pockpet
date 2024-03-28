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
int myBars = 2; //temp

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
//

// push buttons
#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4

int buttonPress = 0;
void MenuScreen();
void lightButtons(int n);
void TomaScreen();

/* Implement Menu Button Handler & Associated State Machine Here.
 * Declare Any Screen functionalities Above Ad Hoc. */

//Shared Flags and Vars to handle UI State

//button1 SM.
//define callback functions
void run_button1(uint gpio, uint32_t events)
{ 
  printf("button1 pressed");
  buttonPress-=1;
  buttonPress = (buttonPress<=0) ? 2 : buttonPress;
  lightButtons(buttonPress);
}

/*
//define function pointer type for callback functions
typedef void (*CallbackFunction)();

//define state variable and initial state
CallbackFunction current_callback1 = run_button1;

//interrupt handler
void button1_interrupt_handler(uint gpio, uint32_t events)
{
  buttonPress-=1;
  buttonPress = (buttonPress<=0) ? 2 : buttonPress;

  current_callback1();
  printf("button1 pressed");
  sleep_ms(1200);

  //SM Logic Here
  //current_callback1 = run_button1;
}
*/

//button2 SM
//define callback functions
void run_button2(uint gpio, uint32_t events)
{
  printf("button2 pressed");
  buttonPress+=1;
  buttonPress = (buttonPress>2) ? 1 : buttonPress;

   lightButtons(buttonPress);
}

/*
//define function pointer type for callback functions
typedef void (*CallbackFunction)();

//define state variable and initial state
CallbackFunction current_callback2 = run_button2;

//interrupt handler
void button2_interrupt_handler(uint gpio, uint32_t events)
{
  buttonPress+=1;
  buttonPress = (buttonPress>2) ? 1 : buttonPress;

  current_callback2();
  printf("button2 pressed");
  sleep_ms(1200);

  //SM Logic Here
  //current_callback; = run_button2;
}
*/

void run_button3(uint gpio, uint32_t events)
{
  TomaScreen();
}

void buttonSetup()
{
  //buttons setup
  gpio_init(BUTTON1);
  gpio_set_dir(BUTTON1, GPIO_IN);
  gpio_pull_up(BUTTON1);
  gpio_set_irq_enabled_with_callback(BUTTON1, GPIO_IRQ_EDGE_RISE, true, &run_button1);
    
  gpio_init(BUTTON2);
  gpio_set_dir(BUTTON2, GPIO_IN);
  gpio_pull_up(BUTTON2);
  gpio_set_irq_enabled_with_callback(BUTTON2, GPIO_IRQ_EDGE_FALL, true, &run_button2);

  gpio_init(BUTTON3);
  gpio_set_dir(BUTTON3, GPIO_IN);
  gpio_pull_up(BUTTON3);
  gpio_set_irq_enabled_with_callback(BUTTON3, GPIO_IRQ_EDGE_RISE, true, &run_button3);
}
//

/* Menu Screen TFT functions */
void ShowBars(int numBars);
void updateButtons() //draw over buttons to reset blinking effect
{
  myTFT.TFTdrawBitmap16Data(84,12, (uint8_t *)phome_button1,42,27);
  myTFT.TFTdrawBitmap16Data(84,42, (uint8_t *)pfood_button1,42,27);
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

void lightButtons(int n)
{
  if (n == 1) 
  {
    updateButtons();
    myTFT.TFTdrawBitmap16Data(84,12, (uint8_t *)phome_button2,42,27);
    //draw over 1
  }
  else if (n == 2)
  {
    updateButtons();
    myTFT.TFTdrawBitmap16Data(84,42, (uint8_t *)pfood_button2,42,27);
    //draw 2
  }
}
//

/* toma screen initializers */

void RenderBg(void)
{
  myTFT.TFTfillScreen(ST7735_WHITE);
  myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 140);
  TFT_MILLISEC_DELAY(TEST_DELAY2);
}
//random num hashing fxn; pass ints as floats, process, then cast it back into int afterwards
float scale(float x,float a, float b, float min, float max)
{
    return (b-a)*(x-min)/(max-min) + a;
}
//global relative dist. vars
const int xMaxVal = 65;
int xPos = 0;
void WalkAnimation() /* walk animation; random amount of distance to travel to end of screen max. 
eventually, the animation must switch directions walking. (need to make poreo4-6 facing op. dir.*/ 
{
  while(1) //temptemp
  {
    //go a random dist forward
    uint32_t r = get_rand_32();
    int dist =int(scale(r,15,65,0,UINT32_MAX));
    printf("\nfwd %d", dist);

    const uint8_t* spriteArrFwd[4] = {poreo1,poreo2,poreo1,poreo3};
    int idxF = 0; //index forward

    for(int i=0;i<=dist;i++)
    {    
      myTFT.TFTdrawBitmap16Data(xPos, 54, (uint8_t*)spriteArrFwd[idxF], 63, 53);
      idxF = idxF >= 3 ? 0 : idxF+1; 
      xPos+=1;
      if (xPos >= 65) 
      {
        printf("\nchanging dirs");
        break;
      }
      sleep_ms(350);
    }

    //stay in place for random num of cycles
    r = get_rand_32();
    dist =int(scale(r,4,12,0,UINT32_MAX));
    printf("\nStay in place for %d", dist);
    idxF = 0; //index forward
    for(int i=0;i<=dist;i++)
    {    
      myTFT.TFTdrawBitmap16Data(xPos, 54, (uint8_t*)spriteArrFwd[idxF], 63, 53);
      idxF = idxF == 1 ? 0 : idxF+1; 
      sleep_ms(500);
    }
    //

    //go a random dist back 
    r = get_rand_32();
    dist =int(scale(r,15,65,0,UINT32_MAX));
    printf("\nback %d", dist);

    const uint8_t* spriteArrBck[4] = {poreo4,poreo5,poreo4,poreo6};
    int idxB = 0; //index back
    for(int i=0;xPos<=xMaxVal;i++)
    {    
      myTFT.TFTdrawBitmap16Data(xPos, 54, (uint8_t*)spriteArrBck[idxB], 63, 53);
      idxB = idxB >= 3 ? 0 : idxB+1; 
      xPos-=1;
      if (xPos <= 0) 
      {
        printf("\nchanging dirs");
        break;
      }
      sleep_ms(350);
    }

    //stay in place for random num of cycles
    r = get_rand_32();
    dist =int(scale(r,4,12,0,UINT32_MAX));
    printf("\nstay in place for %d", dist);
    idxF = 0; //index forward
    for(int i=0;i<=dist;i++)
    {    
      myTFT.TFTdrawBitmap16Data(xPos, 54, (uint8_t*)spriteArrBck[idxB], 63, 53);
      idxB = idxB == 1 ? 0 : idxB+1; 
      sleep_ms(500);
    }
  }
}
void EatAnimation()
{
  const uint8_t* dogArr[4] = {poreo1,poreo3};
  const uint8_t* spriteArr[6] = {psteak1,psteak2,psteak3,psteak4,psteak5,psteak6};
  int idx = 0;
  for(int i=0;i<6;i++)
  {    
    myTFT.TFTdrawBitmap16Data(15, 54, (uint8_t*)dogArr[idx], 63, 53);
    myTFT.TFTdrawBitmap16Data(52, 115, (uint8_t*)spriteArr[i], 34, 21);
    idx = idx >= 1 ? 0 : idx+1; 
    sleep_ms(800);
  }
}
 
void MenuScreen();

//doing testy things here


void TomaScreen() //button3 triggers interrupt to the menu screen
{
  RenderBg(); //render static menu components

  //gpio_set_irq_enabled_with_callback(BUTTON3, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

  //EatAnimation();
  WalkAnimation();
}


/* Menu Screen main function 
 * I need to refactor to use interrupts 
 * */ 

void MenuScreen() 
{
  RenderMenu(); //render static menu components
  sleep_ms(500);
  buttonSetup();
  
}
//

int main()
{
	tftSetup();
  //run the toma screen
  //TomaScreen();
  MenuScreen();
  
  while(1)
  {
    printf("\nrunning");
    sleep_ms(2000);
  }
  return 0;
}
