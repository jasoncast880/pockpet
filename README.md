# Welcome Pockpet-v2 Branch!

## Our Goals:
v2 Branch's focus is on limit testing the pico w running free RTOS, setup in various lib folders in order to increase modularity and increase visibility with the scheduler: 

### Demos:
* ST7735 (Adding the ILI Later) Sprite Handling lib
  * The Value of Adding DMA : https://www.youtube.com/watch?v=FwaqTPlaAvA
* TCP Sockets Handling Lib
* BLE/BT Stack for Getting wifi Credentials.
* Button Handler Lib.
* Various Frame Buffer/Memory Management units...

The prevailing idea being each of these demos will represent some form(s) of task, which can be combined in FreeRTOS; eventually leading to a cohesive user experience on an IOT/embedded device!


## Current Testing Rig (As of 15-08-24)
A Simple SPI Connection to the proper SPI GPIO Ports. VCC and Backlight are connected to VSYS, as shown below.

![image](https://github.com/user-attachments/assets/e7b783ed-5451-4845-b30a-2069199bcdb8)

Configuration Information (Pins, GPIO Modes) can be seen from tftSetup() in main.cpp

19-08-24
This thing sucks lmaooo; i hate debugging this


## Long Term Goals For Pockpet (by branch)
+ v1 - A simple demo prototype using *just* the lcd screen, buttons - this version used a pressure fit 3d printed case, slide-in components and crude soldering & wirings.
+ v2_(dev) - More advanced demos, drivers. Limit test the hardware capabilities of the Pico W using FreeRTOS. (CURRENT BRANCH)
+ v2_prod - Build PCB and case. Limit soldering/wiring requirements to create a seamless build experience. Build a Gameboy-Style Case with a hinge & clasp for easy iteratability. (6 months est.)
