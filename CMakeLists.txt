cmake_minimum_required (VERSION 2.6)
project(ARTD)
set(CMAKE_BUILD_TYPE Release)

include_directories(include/)
add_executable(artd src/main.cpp src/game.cpp)
