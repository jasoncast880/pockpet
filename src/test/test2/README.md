test2: 
try to use the rtos functionality with the lcd.
lcd is working with the rtos kernel linked in the cmake list. However I can't figure out how to access the puTTy terminal now. very weird.
This must be fixed if i want to continue debugging while being able to see the scheduler status.

After building test1 with the current cmake configuration, I saw that the puTTY functionality is still working along with LED Flashing; test1 uses solely the rtos library, so it appears that the linking changes havent affected its ability to transit to console, nor affect running freeRTOS kernel. 

This must mean that the peripheral library is affecting the terminal output somehow. Will look into this as time goes on.
