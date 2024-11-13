file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(pico_st7735_driver STATIC
  ${SOURCES}
)

target_include_directories(pico_st7735_driver PUBLIC
    ${HEADERS}
)

target_link_libraries( pico_st7735_driver 
    pico_stdlib 
    hardware_spi
)

