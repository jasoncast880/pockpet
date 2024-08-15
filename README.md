# Welcome Pockpet-v2 Branch!

## Our Goals:
v2 Branch's focus is on limit testing the pico w running free RTOS, setup in various lib folders in order to increase modularity and increase visibility with the scheduler: 

### Demos:
* ST7735 (Adding the ILI Later) Sprite Handling lib
* TCP Sockets Handling Lib
* BLE/BT Stack for Getting wifi Credentials.
* Button Handler Lib.
* Various Frame Buffer/Memory Management units...

The prevailing idea being each of these demos will represent some form(s) of task, which can be combined in FreeRTOS; eventually leading to a cohesive user experience on an IOT/embedded device!


## Current Testing Rig (As of 08-15-24)
A Simple SPI Connection to the proper SPI GPIO Ports. VCC and Backlight are connected to VSYS, as shown Below.
[image](https://github.com/user-attachments/assets/b71d5637-150d-457b-9781-a66b4ad1f5e2)

