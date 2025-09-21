# Tacti~Gotchi PDA Project

## Build
```bash
cd build
rm -rf ./* 
cmake ..
make -j${nproc}
```

## Debugging Reference (GDB with OpenOCD on Windows+WSL Ubuntu)
1. Ensure that tactigachi can be built. (refer to build steps above)
   GDB needs at the very least, a buildable system to send an .elf file to the mcu.
2. Wire the UART & SWD according to the Debug Probe/Picoprobe instructions.
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
7. Open OpenOCD Server; in WSL, go to your openocd application folder and run the following command in terminal.
```bash
sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" #from directory like *~/embedded\_tools/openocd$* 
```
8. In another terminal, navigate to the build folder (directory where the .elf file is generated) and run the following commands: 
```bash
gdb-multiarch ./<elf-file-name>.elf  
```
from inside the gdb interface:
```bash
target remote localhost:3333
load
monitor reset run # Resets the target and immediately lets it run.
monitor reset halt # Resets the target and immediately halts it at the reset vector.
monitor reset init # Resets the target, halts it, and then executes a board-specific initialization script (if configured in the GDB server).
```
You can now set breakpoints, step through functions from this point onwards.

## Devlogs

