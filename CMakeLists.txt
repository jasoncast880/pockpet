cmake_minimum_required(VERSION 3.16)

set(PICO_BOARD pico_w)

# Name project
SET(ProjectName ampalaya)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Import those libraries
include(pico_sdk_import.cmake)
#include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define project
project(${ProjectName} C CXX)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

#add_subdirectory(src)     #during testing, testbenches is the new src.
add_subdirectory(lib) 

#!!!!testbenches, comment out the one that is testing!!!!!
#add_subdirectory(testbench/graphics_test)
add_subdirectory(testbench/irq_graphics_test)
