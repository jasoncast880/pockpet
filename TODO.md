28-12-25 TODO

GENERAL/STRUCTURE:
refactor, structure so that flags can be set for debug modes, for batt/cable modes; modular building philosophy.

USB CONTROLLER:
Research endpoint documentation and prform static analysis on the rp2040 port. Should be relatively easy to set up device mode for debugging/testing purposes.

FS/SDC CONTROLLER:
Structure & Clean task/semphr management
Still needs lots of rough-out

DISPLAY CONTROLLER:
    1') Structure/refactoring bulk
    1) Ensure No Data Leakeage/Corruption
    2) Build in a way that doesn't compromise gdb; volatile keyword can be problematic.
    3) (system/general issue also): Write a hashmap/array assigning scheme for system font/text.

HID:
    1) Button int. handler (simple)
