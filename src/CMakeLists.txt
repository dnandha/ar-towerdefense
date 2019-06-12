include_directories(${ARTD_SOURCE_DIR}/include)
add_library (game game.cpp player.cpp unit.cpp tower.cpp)
add_executable(artd main.cpp)
target_link_libraries(artd game)
