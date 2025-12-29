add_library(tusb_device_configuration STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/usb_descriptors.c
)

target_include_directories(tusb_device_configuration PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/tusb_conig.h
)
tusb_device_configuration


