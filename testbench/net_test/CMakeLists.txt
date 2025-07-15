add_executable(net_test
    main.cpp

    #
    #${CMAKE_CURRENT_SOURCE_DIR}/src/displayHandler.cpp
    #sdcHandler.cpp
    #${CMAKE_CURRENT_LIST_DIR}/src/networkHandler.cpp
    #      
)

target_include_directories(net_test PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/secrets
    #my includes are below
    #${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(net_test PRIVATE
    pico_stdlib
    pico_cyw43_arch_lwip_sys_freertos

    FreeRTOS-Kernel-Heap4
)

target_compile_definitions(net_test PRIVATE
    configNUMBER_OF_CORES=1
)

pico_enable_stdio_usb(net_test 1)
pico_enable_stdio_uart(net_test 0)

pico_add_extra_outputs(net_test)
