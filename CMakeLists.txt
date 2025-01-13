cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)

project(thermostat C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_executable(thermostat
    main.cpp
)

# Adding all .hpp and .h files to the project
target_include_directories(thermostat PUBLIC 
    ./include/
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
    ${PICO_SDK_PATH}/lib/lwip/src/include/lwip/apps/httpd
)

#################################
# INCLUDE and SRC folder
# Adding .cpp and .c files in /src/
add_library(program_libs
    src/lcd.cpp
    # src/hc_sr04.cpp
    src/button.cpp
    src/ds18b20.cpp
    # src/menu.cpp
    # src/pump_driver.cpp
    src/bme680.cpp
    src/bmp280.cpp
    src/peltier_driver.cpp
    # src/wifi.c
)

target_link_libraries(program_libs
    pico_stdlib 
    pico_multicore
    hardware_i2c
    hardware_sync
    hardware_adc
    pico_rand
    # pico_cyw43_arch_lwip_poll
    pico_cyw43_arch_lwip_threadsafe_background
    # pico_cyw43_arch_lwip
    # pico_cyw43_arch_none
    pico_lwip_http
    pico_lwip_mdns
    pico_lwip 
    # pico_lwip_mbedtls
    # pico_mbedtls
    # mbedtls
)

# Ensure program_libs can find Pico SDK headers
target_include_directories(program_libs PUBLIC 
    ./include/
    # ${PICO_SDK_PATH}/lib/lwip/src/include
    # ${PICO_SDK_PATH}/lib/cyw43-driver/include
)
#################################

# Pull in common dependencies
target_link_libraries(thermostat 
    program_libs
    pico_stdlib 
    pico_multicore
    hardware_i2c
    hardware_sync
    hardware_adc
    pico_rand
    # pico_cyw43_arch_lwip_poll
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
    pico_lwip_mdns
    pico_lwip 
    # pico_lwip_mbedtls
    # pico_mbedtls
    # mbedtls
)

pico_enable_stdio_usb(thermostat 1)
pico_enable_stdio_uart(thermostat 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(thermostat)

# Add URL via pico_set_program_url
# example_auto_set_url(thermostat)