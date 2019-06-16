include_directories(${detection_SOURCE_DIR}/include)
add_executable(test_improc test_image_processor.cpp)
target_link_libraries(test_improc detection)

# copy test images

function(copyfile filename)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_images/${filename}
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

copyfile(marker0.jpg)
copyfile(no_marker.jpg)
copyfile(paper_borders.jpg)