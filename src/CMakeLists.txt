add_executable(${ProjectName}
   main.c
)

add_subdirectory(assets)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/ili9341/include
)

target_link_libraries(${ProjectName}
    pico_stdlib
    hardware_spi
    ili9341_pico_driver
    #assets
)

pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})

