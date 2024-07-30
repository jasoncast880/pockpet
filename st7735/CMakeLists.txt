#cmake_minimum_requried(VERSION 3.10)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(st7735 STATIC
  ${SOURCES}
)

target_link_libraries( st7735 pico_stdlib hardware_spi)

target_include_directories(st7735 PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
