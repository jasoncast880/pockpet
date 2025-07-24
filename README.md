# Welcome to the Ampalaya Branch!!!!!

## Our Goals:
Ampalaya is my study into embedded/iot systems and the art of combining microcontroller routines
and leveraging client-server infastructure to make a robust and well-designed product.

Ampalaya is still in breadboard phase. Once the firmware for all of my peripherals is working to my satisfaction, I'll make a test pcb.

*Hardware-PCB fabrication
*IOT/Wifi handling; CLIENT (pockpet-side) and SERVER (aws???)
*Writing drivers for lcd screens, push button interfaces, and network handlers
*An opportunity to learn how to make pixel art



-should be:
debuggable
all of the peripherals should be thread safe, easily portable to other similar type systems.
should have a robust and flexible power system
should have robust error handling

notes on the hardware:
-bad.
-needs to have a seperate components for lcd tft, sdc in the future? 
    -so i can have an easier time routing the spi bus
    -so i dont have to worry about dependencies if this particular pcb were to go out of stock
    -just better craftsmanship and form in general 
-bad emi, need an actual electrical engineer to help
-debugging and testing needed on power delivery system.
-in the future, sound should be delivered via audio jack; however this poses different techinical challenges entirely due to the fact that the rp2040 lacks a dac converter...

