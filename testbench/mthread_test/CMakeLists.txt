add_executable(mthread_test
    main.cpp
)
target_include_directories(mthread_test PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/freertos/include
)
target_link_libraries(mthread_test PRIVATE
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4
    pico_stdlib
)

if(PICO_CYW43_SUPPORTED)
    target_link_libraries(mthread_test PRIVATE
        pico_cyw43_arch_none
    )
endif()
target_compile_definitions(mthread_test PRIVATE
    configNUMBER_OF_CORES=1
)

pico_enable_stdio_usb(mthread_test 1)
pico_enable_stdio_uart(mthread_test 0)

pico_add_extra_outputs(mthread_test)
