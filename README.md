# Welcome to pockpet v2_dev Branch!


4.11- Thread Local Storage, Reentrancy
    - Thread Local storage: store arbitrary data which should normally be stored in a global variable by reentrant functions....
    - Reentrant Function: a function which can safely run from multiple threads safely; A thread Safe function.
    

## Our Goals:
Pockpet is my study into embedded/iot systems and the art of combining microcontroller routines
and leveraging client-server infastructure to make a robust and well-designed product.

in production branch, i've managed to fit animation frames on an lcd screen running on a loop.
While looking very clean, I know there's much more design complexity that I can add to make this 
project truly great!!!
v2 Branch's focus is on limit testing the pico w running free RTOS. As a hardware-firmware project,
there are many remaining challenges to address in order to make a flush product.

*Hardware-PCB fabrication
*IOT/Wifi handling; CLIENT (pockpet-side) and SERVER (aws???)
*Writing drivers for lcd screens, push button interfaces, and network handlers
*


### Demos:
* ST7735 (Adding the ILI Later) Sprite Handling lib
  * The Value of Adding DMA : https://www.youtube.com/watch?v=FwaqTPlaAvA
* TCP Sockets Handling Lib (THIS BRANCH FOCUS)
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

21-08-24
Build ok, BUT I had to rework pico sdk to include the directories (follow the compiler)... Don't know if this approach works long term.
I need to test with my ec2 instance, i plan a TCP socket application alongside the buffer management system.

30-08-24
I am 'supposedly' finished building out the socket read functions; 
I will have to test on my rig, which hasn't worked well in the past. Perhaps a more simplistic-non elastic ip server is more appropriate for my use case, but i will have to see when i get back to base...

07-10-24
Time to remove the agent class. Rewrite the network handling classes for easier intuition for a small scale project. Refer and understand the lwip class INSIDE of the pico-sdk.

Next up is to actually connect to the server. 
Once connected, I can use my Java Knowledge to write a Java EC2 Server. Java good for jobs

## Long Term Goals For Pockpet (by branch)
+ v1 - A simple demo prototype using *just* the lcd screen, buttons - this version used a pressure fit 3d printed case, slide-in components and crude soldering & wirings.
+ v2_(dev) - More advanced demos, drivers. Limit test the hardware capabilities of the Pico W using FreeRTOS. (CURRENT BRANCH)
+ v2_tcp - socket testing demo. Idea is to use an ec2/other cloud server to serve a pixel array to the pico (tcp client). Ideally hardcode IP credentials, but keep SSID connected via BT or other means (very optional).
+ v2_prod - Build PCB and case. Limit soldering/wiring requirements to create a seamless build experience. Build a Gameboy-Style Case with a hinge & clasp for easy iteratability. (6 months est.)
