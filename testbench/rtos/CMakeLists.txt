add_executable(rtos_test
    main.c
)
target_include_directories(rtos_test PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/freertos/include #has the freertos config
    ${CMAKE_CURRENT_LIST_DIR}/config #lwipopts inside
)
target_link_libraries(rtos_test PRIVATE
    pico_stdlib
    pico_cyw43_arch_none
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4

    ili9341_pico_driver
    frame_handler
)

pico_enable_stdio_usb(rtos_test 1)
pico_enable_stdio_uart(rtos_test 1)

pico_add_extra_outputs(rtos_test)
