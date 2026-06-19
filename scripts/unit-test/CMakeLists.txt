enable_testing()

add_executable( tile_engine_test
	${CMAKE_CURRENT_SOURCE_DIR}/unit-test.cpp
)

target_link_libraries( tile_engine_test
	tile_engine
)


#now add yo tests
add_test (
	NAME tiles_test
	COMMAND ${BASH_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tiles.sh
	$<TARGET_FILE:tile_engine_test>
)
