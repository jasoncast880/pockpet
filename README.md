# Pockpet: A Virtual Pet project based on the Raspberry Pi Pico

## Introduction
Pockpet is a Tomagachi-style device that uses the Raspberry Pi Pico and an LCD Screen to provide a seamless nostalgia experience!
This branch is the first iteration of the pockpet device, encased in a 3d printed enclosure.

To make your own pockpet, you should connect the RPi Pico to an ST7735 LCD screen, along with 3 push buttons.

## How to Install Firmware:
From this top level repository, you can go download /build/src/pockpetv1.uf2
This file can be downloaded to the rpi pico directly; Hold the BOOTSEL button on the pico board while plugging it in to USB, then drag and drop the .uf2 into the pico's file directory.

## Screenshots & Observation Log
DEMO VIDEO: https://youtu.be/JOMGvVkbFnE
![20240801_162118](https://github.com/user-attachments/assets/ae4f28c9-46b2-4414-8691-ea9767c14d8c)
### Mechanical Issues:
![image](https://github.com/user-attachments/assets/05eae27b-5ace-4164-96f0-787f80357a88)
Overall, the design of the case looks great, although i would like to have a proper clamp shell design in a later version. This version uses pressure fit pegs to make a looser fit, which will be inconvenient down the line.
Because this is mainly a personal prototype, i won't put the 3d cad files in this repo, but perhaps with cleaner (better haha) designs I will consider putting them out for public access.
### Electrical Issues:
Assembling this without shorting wires was a challenge because of how much loose wire was dangling. 
I hope that in later iterations pockpet can have a proper pcb in order to ensure that the connections between components dont short or disconnect.
For the average person this design is a huge hassle and is liable to break without proper care.
### SOFTWARE ISSUES:
The animations look great, with my main complaint being that the animation changes aren't always responding to button presses. This is likely to be solved by doing 2 things:
* Switching from button polling to interrupts
* changing the animation calling mechanics??
\nThis is a firmware-cetric project, so much of my design and engineering focus and knowledge is based around the software-firmware area. In v2_dev I have the goals i want to set aside for version 2.
