add_executables(${ProjectName}
   main.cpp
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/st7735/include

pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})

