cmake_minimum_required(VERSION 3.1.0)
project(ImageQuiltingAll)

find_package(OpenCV REQUIRED)
find_package(GTEST REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

message("cmake current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(status "OpenCV VERSION: ${OpenCV_VERSION}")
message(status "       libraries: ${OpenCV_LIBS}")
message(status "       libraries: ${GTEST_LIBRARIES}")

file(GLOB SRC_SOURCE RELATIVE "src" "*.cpp")
file(GLOB TEST_SOURCE RELATIVE "test" "*.cpp")
file(GLOB UTILS_SOURCE RELATIVE "utils" "*.cpp")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/utils)
