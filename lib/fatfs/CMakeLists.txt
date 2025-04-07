#CMAKE FOR fatfs lib
add_library(fatfs STATIC)

target_sources(fatfs PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/ff.c
    ${CMAKE_CURRENT_LIST_DIR}/ffsystem.c
    ${CMAKE_CURRENT_LIST_DIR}/ffunicode.c
)

target_include_directories(fatfs PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/conf
)

target_link_libraries(fatfs PUBLIC
  sdc_driver
)

