

find_package(OpenCV REQUIRED) 
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(${detection_SOURCE_DIR}/include)
add_library (detection cam.cpp image_processor.cpp path_detector.cpp path_finder.cpp paths_initializer.cpp mesh.cpp)

# executable for manual testing
add_executable(detection_mtest main.cpp)

target_link_libraries(detection_mtest detection)
target_link_libraries (detection ${OpenCV_LIBS})
target_link_libraries (detection Eigen3::Eigen)
