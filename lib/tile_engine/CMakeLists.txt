add_library(tile_engine STATIC
  tile_engine.cpp
)

target_include_directories(tile_engine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR} #to pull in tile_engine.h, api.h
    ${CMAKE_SOURCE_DIR}/config #for graphics_conf
)

target_link_libraries(tile_engine 
	#pico_stdlib #prob dont need this
)
