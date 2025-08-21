cmake_minimum_required(VERSION 3.16)

set(PICO_BOARD pico_w)

#Run cmake -B build to generate build/compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

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

add_subdirectory(external) 
add_subdirectory(executable)

#!!!!testbenches, comment out the one that is testing!!!!!
#these won't work anymore...
#add_subdirectory(testbench/graphics_test)
#add_subdirectory(testbench/mthread_test)
#add_subdirectory(testbench/net_test)


#IMPORTANT:only works because i have LCD_FrameHandler in ./~
add_subdirectory("../LCD_FrameHandler" "${CMAKE_BINARY_DIR}/LCD_FrameHandler_build")
