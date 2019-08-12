cmake_minimum_required (VERSION 2.6)
project(ARTD)
set(CMAKE_BUILD_TYPE Release)

# Eigen3
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIR})

include_directories(include/)       
include_directories(lib/detection/include)
include_directories(lib/renderer/include)

add_subdirectory(lib/detection)
add_subdirectory(lib/renderer) 

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
