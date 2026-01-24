add_library(usb_dev_cdc ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c )

target_link_libraries( usb_dev_cdc PUBLIC pico_stdlib pico_unique_id tinyusb_device tinyusb_board)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(usb_dev_cdc PUBLIC ${CMAKE_CURRENT_LIST_DIR})

