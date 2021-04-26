cmake_minimum_required(VERSION 3.10)
project(utils)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB SOURCE "*.cpp")
add_library(utils STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

add_definitions(-std=c++11)