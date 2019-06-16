include_directories(${ARTD_SOURCE_DIR}/include)
add_executable(test_game test_game.cpp)
target_link_libraries(test_game game)

add_executable(test_event test_event.cpp)
target_link_libraries(test_event game)
