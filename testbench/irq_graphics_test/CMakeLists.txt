add_executable(irq_graphics_test
    main.cpp
    FrameHandler.cpp
)

target_include_directories(irq_graphics_test PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/ili9341/include
    ${CMAKE_CURRENT_LIST_DIR}/include #has class headers, ie peripheral HAL abstractions, encapsulations
    ${CMAKE_CURRENT_LIST_DIR}/assets/include #has all of the converted bmp->headers from 'ampalaya-tools'
)

target_link_libraries(irq_graphics_test
    ili9341_pico_driver
    pico_stdlib
    hardware_spi
)

pico_enable_stdio_usb(irq_graphics_test 1)
pico_enable_stdio_uart(irq_graphics_test 0)

pico_add_extra_outputs(irq_graphics_test)

