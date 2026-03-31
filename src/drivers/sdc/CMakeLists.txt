file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(sd_rp2040 STATIC
  ${SOURCES}
)

target_link_libraries(sd_rp2040 PUBLIC
    pico_stdlib 
    hardware_spi
)

target_include_directories(sd_rp2040 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

