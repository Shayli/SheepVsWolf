include_directories(include)
include_directories(../Graphics/include)
file(GLOB_RECURSE SRC_FILES *.cpp *.hpp)
add_library(Physics ${SRC_FILES})
