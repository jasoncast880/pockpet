# pockpet v2

18-07-24
Build compiles okay, have yet to test on physical board; I am very smart and cool.

05-08-24
new branch v2; goals are to add callback functions, lwip support, freertos integration.


goals:
 1 - write isr's for the buttons each button should set a flag that can be read in main (where we will later have tasks setup)
 2 - write callback functions that will be controlled by flags in order to control ui state, mcu functions
 3 - integrate lwip using free rtos tasks to balance processor time

- for today, builds okay on mothership; I will try on a test rig using gbd for further analysis;hopefully debugging is simple enough!!! pushing to v2.
