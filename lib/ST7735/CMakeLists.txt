# Set library name
set(LIB_NAME st7735)

# Add header files
include_directories(include)

# Add source files
file(GLOB SOURCE_FILES "src/*.cpp")

# Create the library
add_library(${LIB_NAME} ${SOURCE_FILES})

# Set include directories
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIB_NAME} 
                      pico_stdlib 
                      hardware_spi
                      )

