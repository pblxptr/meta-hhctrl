cmake_minimum_required(VERSION 3.12)
project(hatch2sr)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
find_package(GTest REQUIRED)

set(COMPILER_CXX_FLAGS "-g -std=c++17")
set(COMPILER_FLAGS "-save-temps ")
set(CMAKE_C_FLAGS "${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMPILER_CXX_FLAGS} ${COMPILER_FLAGS}")
set(SRC_DIR src)
set(STUB_DIR stub)
set(INCLUDE_DIR inc)
set(TEST_DIR test)
set(MOCK_DIR mock)

include_directories(${INCLUDE_DIR})
include_directories(${SRC_DIR})
include_directories(${MOCK_DIR})


# Engine tests
set(ENGINE_TEST ${CMAKE_PROJECT_NAME}-engine)
add_executable(${ENGINE_TEST} 
  ${SRC_DIR}/engine.c
  ${TEST_DIR}/engine_test.cpp
  ${STUB_DIR}/pwm.cpp 
)
target_link_libraries(${ENGINE_TEST} gtest_main gmock gtest pthread)

# Sensor tests
set(SENSOR_TEST ${CMAKE_PROJECT_NAME}-sensor)
add_executable(${SENSOR_TEST} 
  ${SRC_DIR}/sensor.c
  ${TEST_DIR}/sensor_test.cpp
  ${STUB_DIR}/gpio.cpp
  ${STUB_DIR}/interrupt.cpp
)
target_link_libraries(${SENSOR_TEST} gtest_main gmock gtest pthread)

# Relay tests 
set(RELAY_TEST ${CMAKE_PROJECT_NAME}-relay)
add_executable(${RELAY_TEST} 
  ${SRC_DIR}/relay.c
  ${TEST_DIR}/relay_test.cpp
  ${STUB_DIR}/gpio.cpp 
)
target_link_libraries(${RELAY_TEST} gtest_main gmock gtest pthread)

# Hatch2sr tests
set(HATCH2SR_TEST ${CMAKE_PROJECT_NAME}-hatch2sr)
add_executable(${HATCH2SR_TEST}
  ${SRC_DIR}/hatch2sr.c
  ${TEST_DIR}/hatch2sr_general_test.cpp
  ${TEST_DIR}/hatch2sr_test_for_open.cpp
  ${TEST_DIR}/hatch2sr_test_for_close.cpp
  ${STUB_DIR}/sensor.cpp
  ${STUB_DIR}/engine.cpp
  ${STUB_DIR}/relay.cpp
)
target_link_libraries(${HATCH2SR_TEST} gtest_main gmock gtest pthread)

