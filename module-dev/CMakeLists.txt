cmake_minimum_required(VERSION 3.12)
project(hatch2sr)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
find_package(GTest REQUIRED)
include_directories(inc)
include_directories(mock)

set(COMPILER_CXX_FLAGS "-g -std=c++17")
set(COMPILER_FLAGS "-save-temps ")
set(CMAKE_C_FLAGS "${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMPILER_CXX_FLAGS} ${COMPILER_FLAGS}")

# Engine tests
set(ENGINE_TEST ${CMAKE_PROJECT_NAME}-engine)
add_executable(${ENGINE_TEST} 
  test/engine_test.cpp
  src/engine.c
  stub/pwm.cpp 
)
target_link_libraries(${ENGINE_TEST} gtest_main gmock gtest pthread)

# Sensor tests
set(SENSOR_TEST ${CMAKE_PROJECT_NAME}-sensor)
add_executable(${SENSOR_TEST} 
  test/sensor_test.cpp
  src/sensor.c
  stub/gpio.cpp
  stub/interrupt.cpp
)
target_link_libraries(${SENSOR_TEST} gtest_main gmock gtest pthread)

# Relay tests 
set(RELAY_TEST ${CMAKE_PROJECT_NAME}-relay)
add_executable(${RELAY_TEST} 
  test/relay_test.cpp
  src/relay.c
  stub/gpio.cpp 
)
target_link_libraries(${RELAY_TEST} gtest_main gmock gtest pthread)

# Hatch2sr tests
set(HATCH2SR_TEST ${CMAKE_PROJECT_NAME}-hatch2sr)
add_executable(${HATCH2SR_TEST}
  test/hatch2sr_general_test.cpp
  test/hatch2sr_test_for_open.cpp
  test/hatch2sr_test_for_close.cpp
  src/hatch2sr.c
  stub/sensor.cpp
  stub/engine.cpp
  stub/relay.cpp
)
target_link_libraries(${HATCH2SR_TEST} gtest_main gmock gtest pthread)

