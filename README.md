# Tactigachi PDA Firmware 

## Synopsis 

Tactigachi is a PDA firmware-PCB project based on the low-cost, low power RP2040 MCU solution. 
It leverages the numerous and flexible hardware interfaces of the rp2040 to provide a responsive User Interface, simple file storage, USB interface for PC application access, Simple Audio processing and audio jack output:

| Peripheral Subsystem | On-Board Hardware | Software/Drivers | 
| -------------------- | ----------------- | ---------------- |
| Display | ILI9341 Display Module => RP2040 SPI0, DMA-Driven Interrupts | Custom-made Display Driver & Tile Engine
| SD Card | SDC Module => RP2040 SPI0 | fatfs (by elmchan) & Wrapper Class |
| Audio | (WIP) | (WIP) |

Hardware Schematics, PCB Images:
<TODO add link to hardware github repo>

## Building Project
External/3rd Party Dependencies:
Ensure you have these repos cloned and their filepaths set as environment variables. In bash, this is done by adding to .bashrc file.
```bash
export <ENV_VARIABLE_NAME>="<LIB_FILEPATH>"
```

|  Library/Dependency  | Env Variable Name |  Github Link | 
| -------------------- | ----------------- |  ----------- |
| RPi Pico SDK | PICO_SDK_PATH | https://github.com/raspberrypi/pico-sdk.git | 
| FreeRTOS Kernel | FREERTOS_KERNEL_PATH | https://github.com/FreeRTOS/FreeRTOS-Kernel.git |

<TODO - Make a Docker thing for easy compilation?? >

```bash
# run from top level directory
mkdir -p build
cmake -B build

#once built, make target from list of availables (see table below)
cmake --build build -t <target_name>
```

## 'TARGET' Options:
|   NAME   | PURPOSE | 
| -------- | ------- |
| tactigachi_bm | baremetal, no rtos interference for testing. |
| tactigachi_rtos | rtos variant, for full release. |

## Debugging Reference (GDB with OpenOCD on Windows+WSL Ubuntu)
1. Ensure that tactigachi can be built. (refer to build steps above)
   GDB needs a buildable system to send a .elf file to the mcu.
2. Wire the UART & SWD according to the Official Debug Probe instructions.
3. Plug the Debug Probe into your machine's USB Port. To ensure that the probe is visible to your machine, check in Admin Powershell:
```powershell (Admin)
usbipd list
```
  You should get a message like:
|  BUSID  | VID:PID | DEVICE  | STATE |
| ------- | ------- | ------  | ----- |
| BUSID | FFFF:FFFF | CMSIS-DAP v2 Interface, USB Serial Device (COM6) | Shared |

4. After verifying your machine sees the CMSIS-DAP probe, give WSL access to the probe.
```powershell (Admin)
usbipd bind --busid <BUSID>
usbipd attach --wsl --busid <BUSID>
```
You should be able to hear windows chime, indicating windows relinquished control of the probe to the WSL VM.
5. Verify that WSL has the CMSIS probe using:
```bash
lsusb
```
6. If not done so already, power on the target system, either via adequate external power supply, or via micro-usb port.
7. Open OpenOCD Server; in WSL, go to your openocd application folder (ie, *~/embedded\_tools/openocd$*) and run the following command in terminal.
```bash
sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"
```
OR if using rtos .elf variant:
```bash
openocd \
-f interface/cmsis-dap.cfg \
-f target/rp2040.cfg \
-c "rp2040.core0 configure -rtos FreeRTOS"
```

8. In another terminal, navigate to the build folder (directory where the .elf file is generated) and run the following commands: 
```bash
gdb-multiarch ./<elf-file-name>.elf  
```

From root dir, run the preset debug script
```bash
gdb-multiarch -x ./scripts/<script-name>.gdb
```
ORRR from inside the gdb terminal interface:
```bash
target remote localhost:3333 # connect gdb to the open ocd server
load               # puts program into the mcu flash/prog space 
monitor reset halt # Resets the target and immediately halts it at the reset vector.
```
You can now set breakpoints, step through functions from this point onwards.

## Display Firmware
Tactigachi Display uses a 240 x 320 pixel res. LCD TFT Display. The biggest challenge associated with providing display for the RP2040 is managing RAM usage while still providing a way to manage entities on screen (ie; cursor, pop-up icons, simple sprites, etc) and maintain a consistent FPS.

Hardware Resources used:
DMA : Reduces overall program latency by facilitating pixel transfer from ram buf. to SPI display without requiring explicit CPU oversight.
Dedicated SPI Peripheral : RP2040 has a dedicated SPI module & associated SPI HAL functions. This means that I can push to the SPI display interface without having to resort to bit-banging.
Flash : Raw-Bitmap Tilemaps are kept in flash memory as it is cheaper than RAM and only called upon when the tile engine deems necessary. The software written to manage and blit tiles and configure their data into one cohesive RAM buffer is the core of the Display Firmware.

### Core Philosophy Behind Tile Engines

something something inspired by GB Engines/classic low-ram approach.

<Memory Heiarchy Triangle Here>

tradeoffs between different types of rendering (Fullscreen, HScanline, Tile Buffer)

Key to maintaining graphics pipeline in a multi-peripheral system (use an rtos with good synchronization, be mindful of how much ram is drawn by the render method)
That being said, what needs memory in the display? (free-store)
1) Framebuffer/HScanline (usually static, exception is a tile-based fs. buffer)
2) Entity Objects (kept in a vector field ie Layer::sprites. Pretty small compared to the buffer, reserve 1?? kB)

