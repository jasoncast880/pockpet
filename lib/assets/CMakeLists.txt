#file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
#file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(assets_ampalaya INTERFACE)

target_include_directories(assets_ampalaya INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
