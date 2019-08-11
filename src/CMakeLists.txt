add_library (game game.cpp scene.cpp player.cpp unit.cpp tower.cpp marker_detection.cpp absolute_path.cpp spawner.cpp ui.cpp)
add_executable(artd main.cpp)
target_link_libraries(artd game renderer detection)
