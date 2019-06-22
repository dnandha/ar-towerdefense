cmake_minimum_required (VERSION 2.6)
project(ARTD)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()

include_directories(include/)
