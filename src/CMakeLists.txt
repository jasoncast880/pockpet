#add source files
file(GLOB SOURCES "*.cpp")

add_executable(${ProjectName}
              test/test1/main.cpp
              )

target_link_libraries(${ProjectName} 
                      pico_stdlib 
                      FreeRTOS-Kernel-Heap4 
                      st7735
                      )

pico_add_extra_outputs(${ProjectName})
