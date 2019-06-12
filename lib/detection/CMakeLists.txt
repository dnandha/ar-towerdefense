cmake_minimum_required(VERSION 3.0.0)
project(detection VERSION 0.1.0)

option(TEST "Test" OFF)

# Setting lib dirs for windows
IF (WIN32)
set(OpenCV_DIR "/Users/Benja/Desktop/OpenCV")
set(Eigen3_DIR "/Program Files/Eigen/cmake")
ENDIF()

find_package(OpenCV REQUIRED) 
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

if(TEST)
    # Unit tests and manual tests

    include_directories(include/)
    add_executable(detection src/main.cpp src/cam.cpp src/image_processor.cpp src/path_detector.cpp)

    target_link_libraries (detection ${OpenCV_LIBS})
    target_link_libraries (detection Eigen3::Eigen)

    enable_testing()

    add_executable(test1 test/test1.cpp src/cam.cpp src/image_processor.cpp)
    target_link_libraries (test1 ${OpenCV_LIBS})
    target_link_libraries (test1 Eigen3::Eigen)

    add_test(Test test1)
endif()