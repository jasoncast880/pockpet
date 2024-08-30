set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${ProjectName}
  main.cpp
  Agent.cpp
  WifiHandler.cpp
  TCPHandler.cpp
  SocketHandler.cpp
  TCP_Routines.cpp
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
  hardware_irq
)

#idfk
target_include_directories(${ProjectName} PRIVATE ${PICO_SDK_PATH}/lib/lwip/src/include/lwip)

if (NOT DEFINED ENV{WIFI_SSID})
	message(FATAL_ERROR "WIFI_SSID Environment variable not defined")
endif()
if (NOT DEFINED ENV{WIFI_PASSWORD})
	message(FATAL_ERROR "WIFI_PASSWORD Environment variable not defined")
endif()
if (NOT DEFINED ENV{EC2_IP})
	message(FATAL_ERROR "WIFI_SSID Environment variable not defined")
endif()
if (NOT DEFINED ENV{EC2_PORT})
	message(FATAL_ERROR "WIFI_PASSWORD Environment variable not defined")
endif()


target_compile_definitions(${ProjectName} PRIVATE
    WIFI_SSID=\"$ENV{WIFI_SSID}\"
    WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\"
    EC2_IP=\"$ENV{EC2_IP}\"
    EC2_PORT=\"$ENV{EC2_PORT}\"

)

pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})
