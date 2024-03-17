16-03-24

GOAL of test3: establish using freertos kernel to control multiple peripherals and share data amongst them;

1. make 3 tasks to rep 3 peripheral tasks (Ie; screen -> update, setup, establish, rtc -> read, write
                                               speaker-driver -> write )
2. establish task scheduler concepts etc
3. establish a way to share data amongst the tasks (mutex, semaphore, flags, global vars.)

17-03-24

test3 with binary semaphores appears to be working.

here is a snippet by snippet explanation of how to use binary semaphores properly:

`SemaphoreHandle_t count;'

