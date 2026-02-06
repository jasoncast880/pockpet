add_executable(tactigachi_bm dummy.cpp ) #for now ; TODO
add_executable( tactigachi_rtos main.cpp )

add_subdirectory(lib) #for pulling in the core libs; fatfs, engines, etc.
add_subdirectory(drivers) #for pulling in the core libs; fatfs, engines, etc.

if(TARGET tactigachi_bm)
target_sources( tactigachi_bm PRIVATE
	dummy.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/modules/display.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modules/buttons.c
)

target_include_directories(tactigachi_bm PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/include
)


target_link_libraries(tactigachi_bm PRIVATE
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
	tile_engine 
	#fatfs

	# misc.
	assets
)

target_compile_definitions(tactigachi_bm PRIVATE configNUMBER_OF_CORES=1)

#pico_enable_stdio_usb(tactigachi_bm 1) #perhaps disable if not using/debugging via gdb
pico_enable_stdio_uart(tactigachi_bm 1)
pico_add_extra_outputs(tactigachi_bm)

add_custom_command(TARGET tactigachi_bm POST_BUILD # command to enable static mem/sanity check.
    COMMAND arm-none-eabi-size $<TARGET_FILE:tactigachi_bm>
    COMMENT "--- Manual Static Memory Report ---"
)

endif() #tactigachi_bm

##########################
if(TARGET tactigachi_rtos)
target_sources( tactigachi_rtos PRIVATE
	main.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/modules/display.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modules/buttons.c
)

target_include_directories(tactigachi_rtos PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/include
)


target_link_libraries(tactigachi_rtos PRIVATE
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

target_compile_definitions(tactigachi_rtos PRIVATE configNUMBER_OF_CORES=1) #perhaps add on multicore support if possible...

#pico_enable_stdio_usb(tactigachi_rtos 1) #perhaps disable if not using/debugging via gdb
pico_enable_stdio_uart(tactigachi_rtos 1)
pico_add_extra_outputs(tactigachi_rtos)

endif() #tactigachi_rtos

