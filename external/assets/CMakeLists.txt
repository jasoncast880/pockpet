file(GLOB HEADERS "include")
file(GLOB SOURCES "*.cpp")

add_library(assets_lib STATIC
    ${SOURCES}
)

target_include_directories(assets_lib PUBLIC ${HEADERS})
