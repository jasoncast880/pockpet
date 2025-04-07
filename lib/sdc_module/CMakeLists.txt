#CMAKE for sdc_module/
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(sdc_driver STATIC
  ${SOURCES}
)

target_link_libraries(sdc_driver
  pico_stdlib
  hardware_spi
  hardware_gpio
)


target_include_directories(sdc_driver PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

