add_library(fatfs STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/ff.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/diskio.c
    #ffunicode.c
)

target_link_libraries(fatfs
    pico_stdlib 
    hardware_spi
)

target_include_directories(fatfs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/executable/config #for pinout
)
