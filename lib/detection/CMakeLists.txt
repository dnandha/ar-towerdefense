cmake_minimum_required(VERSION 3.0.0)
project(detection VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Release)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test (NAME CamTest COMMAND test_cam)
add_test (NAME ImprocTest COMMAND test_improc)

include_directories(include/)
