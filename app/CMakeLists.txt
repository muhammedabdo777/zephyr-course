cmake_minimum_required(VERSION 3.20)

set(DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(KCONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig.app)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(led_sensor_task2)

target_include_directories(app PRIVATE drivers/led_sensor)

target_sources(app PRIVATE
    src/main.c
    drivers/led_sensor/led_sensor.c
)
