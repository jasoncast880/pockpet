add_executable(prod
    main.cpp
)

target_include_directories(prod PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include #has class headers, ie peripheral HAL abstractions, encapsulations
)

target_link_libraries(prod
    pico_stdlib
    hardware_spi
    ili9341_pico_driver
    assets_lib
    frame_handler
)

pico_enable_stdio_usb(prod 1)
pico_enable_stdio_uart(prod 0)

pico_add_extra_outputs(prod)

