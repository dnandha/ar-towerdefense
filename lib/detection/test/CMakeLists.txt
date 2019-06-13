include_directories(${detection_SOURCE_DIR}/include)
add_executable(test_cam test_cam.cpp)
target_link_libraries(test_cam detection)
