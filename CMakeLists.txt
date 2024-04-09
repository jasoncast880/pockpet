# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Include build functions from Pico SDK
include(pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(test1 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# turn on all compiler warnings 
add_compile_options(-Wall -Wextra)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
  #examples/ST7735_TFT_HELLO/main.cpp
  #examples/ST7735_TFT_GRAPHICS/main.cpp
  #examples/ST7735_TFT_FUNCTIONS_FPS/main.cpp
  #examples/ST7735_TFT_TESTS/main.cpp
  #examples/ST7735_TFT_BMP_DATA/main.cpp
  #examples/ANIMATION_TEST/main.cpp
  #examples/DMA_TEST/main.c
  examples/CASTICO/main.cpp
  #examples/CONTROLLER_UI_DEMO/main.cpp
  #examples/helpme/main.cpp
  #examples/MENU_TEST/main.cpp
  #examples/GPIO_TEST/main.cpp
  #examples/IRQ_TEST/main.cpp
  #examples/IRQ_TEST/button_interrupt.pio
)

#pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/examples/IRQ_TEST/button_interrupt.pio)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

add_library(pico_st7735 INTERFACE)

target_sources(pico_st7735 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT_graphics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT_Print.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT_Font.cpp
  # ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT_Bitmap_Data.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT_Assets.cpp
)

target_include_directories(pico_st7735 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} pico_stdlib pico_rand hardware_spi  pico_st7735 hardware_pio )


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
