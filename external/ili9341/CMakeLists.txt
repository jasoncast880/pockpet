file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
#file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(ili9341_pico_driver STATIC
  ${SOURCES}
)

target_link_libraries(ili9341_pico_driver PUBLIC
    pico_stdlib 
    hardware_spi
)

target_include_directories(ili9341_pico_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(ili9341_pico_driver PRIVATE -mcpu=cortex-m0plus -mthumb)
