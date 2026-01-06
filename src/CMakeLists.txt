add_executable(tactigachi
	#dummy.cpp #vid hardware test
	main.cpp 
  #add the handlers here as needed
	${CMAKE_CURRENT_SOURCE_DIR}/src/display.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/button_handler.c
)

target_include_directories(tactigachi PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    PROPERTIES COMPILE_FLAGS "-fno-lto -fno-inline -fno-function-sections -fno-data-sections"
)

add_subdirectory(lib) #for pulling in the core libs; fatfs, engines, etc.

target_link_libraries(tactigachi PRIVATE
	# macro config
	tactigachi_cfg

	# core HAL as required
	pico_stdlib
	hardware_irq
	hardware_gpio
	hardware_dma

	# drivers/other HAL
	ili9341_rp2040
	usb_dev_cdc

	# core dependencies
	FreeRTOS-Kernel-Heap4
	tile_engine 
	fatfs

	# misc.
	assets
)

target_compile_definitions(tactigachi PRIVATE configNUMBER_OF_CORES=1)

pico_enable_stdio_usb(tactigachi 1)
pico_enable_stdio_uart(tactigachi 0)

pico_add_extra_outputs(tactigachi)

#TODO: make some kind of message system to see how much is built, how much ram has been allocated..
