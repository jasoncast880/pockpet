cmake_minimum_required(VERSION 3.5)

#Run cmake -B build to generate build/compile_commands.json
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

SET(CMAKE_CXX_STANDARD 17) #baseline compatibility is c++ 11
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

project(tactigachi C CXX ASM)

set(CMAKE_C_FLAGS_DEBUG "-g -Og -fno-omit-frame-pointer") #for debugging
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -fno-omit-frame-pointer")

add_library( tactigachi_cfg INTERFACE) #holds macro config files, pinout macros
target_include_directories( tactigachi_cfg INTERFACE config )

					  #isolated from src (no ARM/Vendor specific libraries/toolchain)


if(HOST_BUILD)
	message(STATUS "HOST BUILD DETECTED : Enabling unit testing")
	enable_testing()

	add_subdirectory(tools) # paths to scripts/unit-test and adds executable
	add_subdirectory(lib) #for pulling in the core libs; fatfs, engines, etc.
else()
	message(STATUS "HOST BUILD BYPASSED : Building Firmware")

	set(PICO_BOARD pico_w)
	include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

	pico_sdk_init()

	add_subdirectory(src)
	add_subdirectory(lib) #for pulling in the core libs; fatfs, engines, etc.
	add_subdirectory(assets) # TODO delete once
endif()
