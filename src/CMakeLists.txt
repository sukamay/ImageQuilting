cmake_minimum_required(VERSION 3.10)
project(ImageQuilting)

set(CMAKE_CXX_STANDARD 17)
set(GFLAGS_INCLUDE_DIRS /usr/local/include)

find_package(OpenCV REQUIRED)
find_package(GTEST REQUIRED)
find_package(gflags REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GFLAGS_INCLUDE_DIRS})

include_directories(../utils)

file(GLOB SOURCE "*.cpp")
add_library(src STATIC ${SOURCE})

add_executable(${PROJECT_NAME} main.cpp error_code.h log.h
image_quilting.h image_quilting.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${GTEST_LIBRARIES} gflags utils)

add_definitions(-std=c++11)