cmake_minimum_required(VERSION 3.8)
project(hw6)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(GTest REQUIRED)
# find_package(GMock REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
# include_directories(${GMOCK_INCLUDE_DIRS})

include(CodeCoverage)

add_executable(
        RunTests
        main_test.cpp
        regex_parser.h
        test.cpp)

target_link_libraries(RunTests ${GTEST_LIBRARIES} Threads::Threads)

add_executable(main main.cpp)

setup_target_for_coverage(coverage_report RunTests coverage)
SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

enable_testing()