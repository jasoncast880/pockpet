add_library(sdc_module STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdc_driver.c
)

target_link_libraries(sdc_module
    pico_stdlib 
    hardware_spi
)

target_include_directories(sdc_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
