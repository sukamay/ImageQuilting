cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
 
project(test)
 
find_package(OpenCV REQUIRED)
find_package(GTEST REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(../src)
include_directories(../utils)
file(GLOB SOURCE "*.cpp")
add_executable(${PROJECT_NAME} main.cpp test_image_quilting.h)
 
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${GTEST_LIBRARIES} src utils)

