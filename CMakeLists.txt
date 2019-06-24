cmake_minimum_required (VERSION 2.6)
project(ARTD)
set(CMAKE_BUILD_TYPE Release)

include_directories(include/)
include_directories(lib/detection/include)
include_directories(lib/renderer/include)

add_subdirectory(lib/detection)
add_subdirectory(lib/renderer)

#link_directories(lib/detection/)
#link_directories(lib/renderer/)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
