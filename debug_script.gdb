set pagination off
set confirm off

file build/src/tactigachi_bm.elf

target extended-remote localhost:3333

monitor reset halt
load

#b dummy.cpp:48 
b tile_engine.cpp:127

continue
