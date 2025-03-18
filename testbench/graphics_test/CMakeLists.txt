add_executable(graphics_test
    main.cpp
)

target_include_directories(graphics_test PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include #has class headers, ie peripheral HAL abstractions, encapsulations
)

target_link_libraries(graphics_test
    pico_stdlib
    hardware_spi
    hardware_irq
    ili9341_pico_driver
    assets_lib
    frame_handler
)

pico_enable_stdio_usb(graphics_test 1)
pico_enable_stdio_uart(graphics_test 0)

pico_add_extra_outputs(graphics_test)

