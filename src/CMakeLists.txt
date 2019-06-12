include_directories(${ARTD_SOURCE_DIR}/include)
add_library (game game.cpp player.cpp unit.cpp tower.cpp)
add_executable(artd main.cpp)
target_link_libraries(artd game)

find_package(OGRE 1.12 REQUIRED COMPONENTS Bites RTShaderSystem)

file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})

add_executable(1_FirstScene BasicTutorial1.cpp)
target_link_libraries(1_FirstScene ${OGRE_LIBRARIES})
