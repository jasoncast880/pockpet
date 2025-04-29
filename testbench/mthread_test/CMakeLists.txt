add_executable(mthread_test
    main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/networkHandler.c

    #
    #${CMAKE_CURRENT_SOURCE_DIR}/src/displayHandler.cpp
    #sdcHandler.cpp
    #      
)
target_include_directories(mthread_test PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/freertos/include
    #my includes are below
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(mthread_test PRIVATE
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4
    pico_stdlib
    ili9341_pico_driver
    frame_handler
)

if(PICO_CYW43_SUPPORTED)
    target_link_libraries(mthread_test PRIVATE
        pico_cyw43_arch_lwip_sys_freertos
    )
endif()
target_compile_definitions(mthread_test PRIVATE
    configNUMBER_OF_CORES=1
)

pico_enable_stdio_usb(mthread_test 1)
pico_enable_stdio_uart(mthread_test 0)

pico_add_extra_outputs(mthread_test)
