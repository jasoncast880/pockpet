cmake_minimum_required(VERSION 3.16)

set(PICO_BOARD pico_w)

#Run cmake -B build to generate build/compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
set(CMAKE_BUILD_TYPE Debug)

# Name project
SET(ProjectName tactigachi)
SET(CMAKE_CXX_STANDARD 17) #baseline compatibility is c++ 11
SET(CMAKE_CXX_STANDARD_REQUIRED ON)


# Import those libraries
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${ProjectName} C CXX ASM)

pico_sdk_init()

add_subdirectory(external) 
add_subdirectory(executable)

