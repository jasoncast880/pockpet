cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico_w)

# Name project
SET(ProjectName pockpet)

# Import those libraries
include(pico_sdk_import.cmake)
#include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define project
project(${ProjectName})

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(lib)
