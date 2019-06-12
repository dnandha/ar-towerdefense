include_directories(${ARTD_SOURCE_DIR}/include)
add_executable(test_game test_game.cpp)
target_link_libraries(test_game game)
