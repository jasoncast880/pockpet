add_library(fatfs 
    ${CMAKE_CURRENT_SOURCE_DIR}/source/ff.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/diskio.c
    #ffunicode.c
)

target_include_directories(fatfs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)
