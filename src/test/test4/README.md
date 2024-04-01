okay, so ive decided best thing to do is use a queue to handle the tasks i need.
-atomic operations
-easier to implement & debug

tasks: 
1) dispayTimeReadTask(): 
2) displayTask():
3) rtcTask():
4) sound-driverTask():
5) how to handle button Presses....

I am thinking about a lot of different problems right now.
How will i handle storing data such as time diff?
How will i handle volume and switching off the power?
How will i store display data and point from within the RTOS kernel.

i'm confused af LMAOOO

19-03-24
I have read thru documentation
This may have been easier than i made it out to be...
seems as though i can just reference a system pointer that holds the rtc data indefinitelyand just do that..
it appears that i can just reference the time on a partitioned part of memory and let the system apis and whatnot handle any of the queue implementations i had planned on performing.
it should be easier to handle.

i'm getting kind of swamped when it comes to handling the freertos functionalities on top of learning how to handle the principles of working with peripherals and interrupts, so for now i'm going to dial back the intensity by removing the freeRTOS header, along with any tasks that would normally be there, replacing them with regular functions that can call on global values by reference/ptr.
