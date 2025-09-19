cmake_minimum_required(VERSION 3.16)

set(PICO_BOARD pico_w)

#Run cmake -B build to generate build/compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
set(CMAKE_BUILD_TYPE Debug)

# Name project
SET(ProjectName ampalaya)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Import those libraries
include(pico_sdk_import.cmake)

# Define project
project(${ProjectName} C CXX )

pico_sdk_init()

add_subdirectory(external) 
add_subdirectory(executable)

#removed because i interated it internally
#add_subdirectory("../LCD_FrameHandler" "${CMAKE_BINARY_DIR}/LCD_FrameHandler_build")
