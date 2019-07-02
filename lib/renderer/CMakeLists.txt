cmake_minimum_required(VERSION 3.0.0)
project(renderer VERSION 0.1.0)

find_package(OpenCV REQUIRED) 
include_directories(${OPENCV_INCLUDE_DIR})

include_directories(include/)
add_library(renderer src/renderer.cpp)

target_link_libraries (renderer ${OpenCV_LIBS})
