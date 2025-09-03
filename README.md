# Tactigachi PDA Project

## Goals:

## Debugging Reference (GDB with OpenOCD on Windows+WSL Ubuntu)
1. Ensure that tactigachi can be built. (refer to build steps above)
   GDB needs at the very least, a buildable system to send an .elf file to the mcu.
2. Wire the UART & SWD according to the Debug Probe/Picoprobe instructions.
3. Plug the Debug Probe into your machine's USB Port. To ensure that the probe is visible to your machine, check in Admin Powershell:
```
usbipd list
```
  You should get a message like:
|  BUSID  | VID:PID | DEVICE  | STATE |
| ------- | ------- | ------  | ----- |
| <BUSID> | FFFF:FFFF | CMSIS-DAP v2 Interface, USB Serial Device (COM6) | Shared |

4. After verifying your machine sees the CMSIS-DAP probe, give WSL access to the probe.
```
usbipd bind --busid <BUSID>
usbipd attach --wsl --busid <BUSID>
```

## Devlogs

