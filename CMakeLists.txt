cmake_minimum_required(VERSION 3.12)

# Name project
SET(ProjectName pockpetv1)

# Set any variables required for importing libraries
#SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Import those libraries
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
#include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define project
project(${ProjectName})

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add subdirectories
add_subdirectory(src)

add_subdirectory(st7735)
