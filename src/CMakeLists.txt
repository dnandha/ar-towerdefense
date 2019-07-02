add_library (game game.cpp scene.cpp player.cpp unit.cpp tower.cpp marker_detection.cpp)
add_executable(artd main.cpp)
target_link_libraries(artd game renderer detection)
