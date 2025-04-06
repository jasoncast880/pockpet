file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(fatfs STATIC
  ${SOURCES}
)

target_link_libraries(fatfs 
    sdc_module
)

target_include_directories(fatfs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
