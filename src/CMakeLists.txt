#set board type 

add_executable(${ProjectName}
    main.cpp
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../lib/st7735/include
)


target_link_libraries(${ProjectName} 
    pico_stdlib 
    pico_cyw43_arch_lwip_sys_freertos
    FreeRTOS-Kernel-Heap4 
    hardware_spi
    st7735
    )

pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})
