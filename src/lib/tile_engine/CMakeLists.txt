add_library(tile_engine STATIC
  tile_engine.cpp
	wrapper.cpp #so i can use this in my c-based modules
)

target_include_directories(tile_engine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR} #to pull in tile_engine.h
)

target_link_libraries(tile_engine 
	#pico_stdlib #prob dont need this
)
