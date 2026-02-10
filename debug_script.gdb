set pagination off
set confirm off

file build/src/tactigachi_bm.elf

target extended-remote localhost:3333

monitor reset init
load
break main
continue
