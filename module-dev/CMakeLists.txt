cmake_minimum_required(VERSION 3.12)
project(hatch2sr)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
find_package(GTest REQUIRED)
include_directories(inc)
include_directories(mock)

add_executable(${CMAKE_PROJECT_NAME} 
  test/sensor_test.cpp 
  src/hatch2sr.c 
  src/engine.c
  src/sensor.c
  src/relay.c
  stub/pwm.cpp
  stub/gpio.cpp
  stub/interrupt.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} gtest_main gmock gtest pthread)