add_executable(irq_graphics_test
    main.cpp
)

target_include_directories(irq_graphics_test PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include #has class headers, ie peripheral HAL abstractions, encapsulations
)

target_link_libraries(irq_graphics_test
    pico_stdlib
    hardware_spi
    ili9341_pico_driver
    assets_lib
    frame_handler
)

pico_enable_stdio_usb(irq_graphics_test 1)
pico_enable_stdio_uart(irq_graphics_test 0)

pico_add_extra_outputs(irq_graphics_test)

