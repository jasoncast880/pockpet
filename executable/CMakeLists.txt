add_executable(tactigachi
    main.cpp
    #add the handlers here as needed
    ${CMAKE_CURRENT_SOURCE_DIR}/src/display_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/button_handler.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdc_handler.cpp
)

target_include_directories(tactigachi PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/freertos_config/include #theres nothing in here..
    ${CMAKE_SOURCE_DIR}/secrets

    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    PROPERTIES COMPILE_FLAGS "-fno-lto -fno-inline -fno-function-sections -fno-data-sections"
)

target_link_libraries(tactigachi PRIVATE
    pico_stdlib

    FreeRTOS-Kernel-Heap4
    pico_cyw43_arch_lwip_sys_freertos
    hardware_irq
    hardware_gpio

    ili9341_pico_driver
    frame_handler 
    assets_lib
    fatfs

)

target_compile_definitions(tactigachi PRIVATE configNUMBER_OF_CORES=1)

pico_enable_stdio_usb(tactigachi 1)
pico_add_extra_outputs(tactigachi)
