add_executable(rtos_test
    main.c
    src/network_handler.c
)
target_include_directories(rtos_test PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/freertos/include #has the freertos config
    ${CMAKE_CURRENT_LIST_DIR}/config #lwipopts inside
    ${CMAKE_CURRENT_LIST_DIR}/include 
)
target_link_libraries(rtos_test PRIVATE
    pico_cyw43_arch_lwip_sys_freertos
    #pico_lwip
    #    pico_cyw43_driver

    #pico_async_context_freertos
    FreeRTOS-Kernel-Heap4

    ili9341_pico_driver
    frame_handler

    pico_stdlib
)

target_compile_definitions(rtos_test PRIVATE
    SSID=\"${WIFI_SSID}\"
    PWD=\"${WIFI_PASSWORD}\"
)

pico_enable_stdio_usb(rtos_test 1)
pico_enable_stdio_uart(rtos_test 1)

pico_add_extra_outputs(rtos_test)
