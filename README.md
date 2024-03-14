# pockpet: tomagachi clone

Dependencies: C/C++ SDK with FREERTOS; RPI Pico ARM architecture.

Peripherals: On-board RTC Module, 128x160px LCD TFT Screen (ST7735R) via SPI

Last repo push 14-03-24 by J.Castillanes

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



