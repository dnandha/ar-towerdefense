add_test (NAME GameTest COMMAND test_game)
add_test (NAME UnitTest COMMAND test_unit)
add_test (NAME EventTest COMMAND test_event)

include_directories(${ARTD_SOURCE_DIR}/include)
add_executable(test_game test_game.cpp)
target_link_libraries(test_game game)

add_executable(test_unit test_unit.cpp)
target_link_libraries(test_unit game)

add_executable(test_event test_event.cpp)
target_link_libraries(test_event game)
