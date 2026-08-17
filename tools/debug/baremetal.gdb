set pagination off
set confirm off

file build/src/tactigachi_bm.elf

target extended-remote localhost:3333

monitor reset halt
load

b main

#verify setup object is ok
b dummy.cpp:40 

continue
