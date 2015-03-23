cmake_minimum_required(VERSION 2.8)

project(SheepVsWolf)

add_definitions(-std=c++11 -Wfatal-errors)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Assimp REQUIRED)

# Pour gérer un bug a la fac, a supprimer sur machine perso:
#set(OPENGL_LIBRARIES /usr/lib/x86_64-linux-gnu/libGL.so.1)

include_directories(${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} Physics/include third-party/include Graphics/include)

add_subdirectory(Physics)
add_subdirectory(Graphics)

set(ALL_LIBRARIES Physics Graphics ${SDL2_LIBRARY} SDL_image jpeg ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} assimp)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

message("${SDL2_INCLUDE_DIR}")
#foreach(SRC_FILE ${SRC_FILES})
#    get_filename_component(FILE ${SRC_FILE} NAME_WE)
    add_executable(SheepVsWolf ${SRC_FILES})
    target_link_libraries(SheepVsWolf ${ALL_LIBRARIES})
#endforeach()
