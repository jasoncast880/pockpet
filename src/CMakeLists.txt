add_executable(${ProjectName}
    main.cpp
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../st7735/include
)

target_link_libraries(${ProjectName} 
    pico_stdlib 
    hardware_spi
    pico_rand
    st7735
    #FreeRTOS-Kernel-Heap4 
    )

pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})
