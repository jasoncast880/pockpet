set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${ProjectName}
  main.cpp
  WifiHandler.cpp
  Agent.cpp
  Transport.cpp
  TCP_Transport.cpp
  TCP_Routines.cpp

  #double_bufferDemo.cpp
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/src/include #for src header files
    ${CMAKE_CURRENT_LIST_DIR}/../lib/st7735/include
)

target_link_libraries(${ProjectName} 
    pico_stdlib 
    pico_cyw43_arch_lwip_sys_freertos
    FreeRTOS-Kernel-Heap4 
    hardware_spi
    st7735
    )

if (NOT DEFINED ENV{WIFI_SSID})
	message(FATAL_ERROR "WIFI_SSID Environment variable not defined")
endif()
if (NOT DEFINED ENV{WIFI_PASSWORD})
	message(FATAL_ERROR "WIFI_PASSWORD Environment variable not defined")
endif()


target_compile_definitions(${ProjectName} PRIVATE
    WIFI_SSID=\"$ENV{WIFI_SSID}\"
    WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\"
)

pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})
