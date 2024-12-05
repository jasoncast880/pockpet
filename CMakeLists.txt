cmake_minimum_required(VERSION 3.16)

set(PICO_BOARD pico_w)

# Name project
SET(ProjectName pockpet)
# Define project
project(${ProjectName} LANGUAGES C)

set(CMAKE_ASM_COMPILER arm-none-eabi-as)
set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_STANDARD 17)

# Import those libraries
include(pico_sdk_import.cmake)
#include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(lib)
