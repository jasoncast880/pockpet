for hardware drivers using EITHER:
1) direct pico-sdk functionality for the rp2040 without any 3rd party dependencies. contain a header and a c file, if pio then an asm file also.

2) interfacing/importing into third party lib, ie tusb, fatfs. hold the descriptor/protocol file, config file.

expose the funcionality as a Cmake library, call an import from the appropriate hw interface class. for max modularty and clarity; HAL Lib functionality:
describe like "xxx-rp2040"
