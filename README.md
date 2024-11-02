# Pockpet: A Virtual Pet project based on the Raspberry Pi Pico

## Introduction
Pockpet is a Tomagachi-style device that uses the Raspberry Pi Pico and an LCD Screen to provide a seamless nostalgia experience! <br/>
This branch is the first iteration of the pockpet device, encased in a 3d printed enclosure. <br/>
To make your own pockpet, you should connect the RPi Pico to an ST7735 LCD screen, along with 3 push buttons. <br/>

![image](https://github.com/user-attachments/assets/3d48ecf2-d453-4647-ab5c-97c5d59acf03)

| PICO-GPIO | Component Pin |
| ----------- | ----------- |
| GP16 | ST7735_RST |
| GP17 | ST7735_DC |
| GP18 | ST7735_CLK |
| GP19 | ST7735_SDIN |
| GP21 | ST7735_CS |
| GP2 | BUTTON1 |
| GP3 | BUTTON2 |
| GP4 | BUTTON3 |
---
## Installing Firmware

From this top-level repository, you can go download `/build/src/pockpetv1.uf2`.  
This file can be downloaded to the RPi Pico directly. Hold the BOOTSEL button on the Pico board while plugging it into USB, then drag and drop the `.uf2` file into the Pico's file directory.

---
## Screenshots & Observation Log

DEMO VIDEO: https://youtu.be/JOMGvVkbFnE  

![20240801_162118](https://github.com/user-attachments/assets/ae4f28c9-46b2-4414-8691-ea9767c14d8c) <br/>
### Mechanical Issues:
![image](https://github.com/user-attachments/assets/36948a9e-9aa2-4d26-9014-eb960dd905f9)<br/>
Overall, the design of the case looks great, although i would like to have a proper clamp shell design in a later version. This version uses pressure fit pegs to make a looser fit, which will be inconvenient down the line. <br/>
Because this is mainly a personal prototype, i won't put the 3d cad files in this repo, but perhaps with cleaner (better haha) designs I will consider putting them out for public access. <br/>
### Electrical Issues:
Assembling this without shorting wires was a challenge because of how much loose wire was dangling. <br/>
I hope that in later iterations pockpet can have a proper pcb in order to ensure that the connections between components don't short or disconnect. <br/>
### SOFTWARE ISSUES:
The animations look good, with my main complaint being that the animation changes aren't always responding to button presses. This is likely to be solved by doing 2 things: <br/>
* Switching from button polling to interrupts
* changing the animation calling mechanics?? 
<br/>This is a firmware-cetric project, so much of my design and engineering focus and knowledge is based around the software-firmware area. In v2_dev I have the goals i want to set aside for version 2.
