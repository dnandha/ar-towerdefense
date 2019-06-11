cmake_minimum_required(VERSION 3.0.0)
project(DETECTION VERSION 0.1.0)

IF (WIN32)
set(OpenCV_DIR "/Users/Benja/Desktop/OpenCV")
set(Eigen3_DIR "/Program Files/Eigen/cmake")
ENDIF()

find_package(OpenCV REQUIRED) 
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(include/)
add_executable(DETECTION src/main.cpp src/cam.cpp src/image_processor.cpp src/path_detector.cpp)

target_link_libraries (DETECTION ${OpenCV_LIBS} )
target_link_libraries (DETECTION Eigen3::Eigen)

