# pockpet: tomagachi clone

Dependencies: C/C++ SDK with FREERTOS; RPI Pico ARM architecture.

Peripherals: On-board RTC Module, 128x160px LCD TFT Screen (ST7735R) via SPI

repo push 14-03-24 by J.Castillanes

14-03-24
I have organized the directory like so: 

|
|--lib|
|     |-FreeRTOS-Kernel/...
|     |-ST7735-Driver/...
|
|--src|
      |-test|
      |     |-test1/main.cpp
      |-CMakeLists.txt
      |...

This allows for modular testing and will assist me in trying to build with peripherals and task timings.

General Rules to follow:
libraries for peripherals (LCD/TFT, RTC Hardware, etc) should be called as deep into the project tree structure as possible, usually in the src CMakesList.


Last repo push 01-04-24 by J.Castillanes
I have come back to attempt freeRTOS functionalities; I think its time to commit to this format. Refer to the cpp-sdk branch to see any progress made in the past couple weeks.
Today I will step back and try to do a clean setup of this project;
Hopefully i can integrate the semphr.h and task.h files seamlessly to handling button polling.

