add_executable(${ProjectName}
   main.cpp
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/ili9341/include
    ${CMAKE_SOURCE_DIR}/lib/assets/include
)

target_link_libraries(${ProjectName}
    ili9341_pico_driver
    assets_ampalaya
    pico_stdlib
    hardware_spi
)

pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})

