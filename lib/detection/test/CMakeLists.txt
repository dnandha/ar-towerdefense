include_directories(${detection_SOURCE_DIR}/include)
add_executable(test_improc test_image_processor.cpp)
target_link_libraries(test_improc detection)
