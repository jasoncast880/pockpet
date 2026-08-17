set pagination off
set confirm off

file build/src/tactigachi_bm.elf

target extended-remote localhost:3333
load
monitor reset halt

b engine_render
#hscanline resetter
b dummy.c:43 

continue
