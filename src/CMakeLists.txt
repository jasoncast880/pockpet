#add_executable(tactigachi
	#main.cpp 
  #add the handlers here as needed
	#${CMAKE_CURRENT_SOURCE_DIR}/services/display.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/services/buttons.c
#)

add_executable(tactigachi dummy.cpp ) #for now

pico_generate_pio_header(tactigachi ${CMAKE_CURRENT_LIST_DIR}/services/buttons.pio)

target_sources( tactigachi PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/services/display.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/services/buttons.c
)

target_include_directories(tactigachi PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/services/include
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
	hardware_pio

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
