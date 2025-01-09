file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
#file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(frame_handler STATIC
  FrameHandler.cpp
)

target_link_libraries(frame_handler 
    pico_stdlib 
    hardware_spi
    ili9341_pico_driver
)

target_include_directories(frame_handler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
