file(GLOB SOURCES "*.cpp")

add_library(assets STATIC
    ${SOURCES}
)

target_include_directories(assets PUBLIC include)
