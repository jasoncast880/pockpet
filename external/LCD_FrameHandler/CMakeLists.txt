file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
#file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

#IMPORTANT: rewrite to be hardware agnostic. write directly to a struct or something;
# make it so this is easily integrated into freertos code, task creation, async contexts

add_library(frame_handler STATIC
    #pico_stdlib 
    #hardware_spi
  FrameHandler.cpp
)

target_link_libraries(frame_handler 
)

target_include_directories(frame_handler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
