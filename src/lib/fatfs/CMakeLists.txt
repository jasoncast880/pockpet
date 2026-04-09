add_library(fatfs STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/ff.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/diskio.c

    ${CMAKE_CURRENT_SOURCE_DIR}/source/ffunicode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/ffsystem.c
)

target_link_libraries(fatfs
	#for diskio to access the driver file
	sd_rp2040 
)

target_include_directories(fatfs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/config #for accessing pinout.h
)
