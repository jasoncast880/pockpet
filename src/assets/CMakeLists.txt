# Create a static library for the assets
add_library(assets STATIC portraitDemo.cpp)

# Include the current directory for the header file
target_include_directories(assets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

