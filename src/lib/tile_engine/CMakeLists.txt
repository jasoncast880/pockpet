file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(tile_engine STATIC
  FrameHandler.cpp
)

target_include_directories(tile_engine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tile_engine 
	#pico_stdlib #prob dont need this
)
