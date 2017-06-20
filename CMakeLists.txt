cmake_minimum_required(VERSION 3.6)
project(Mosaic_cpp)

find_package(OPENCV)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp colorHistVector.cpp colorHistVector.h util.cpp util.h quadTree.cpp quadTree.h imgSegmentation.cpp imgSegmentation.h)
add_executable(Mosaic_cpp ${SOURCE_FILES})

target_link_libraries( Mosaic_cpp ${OpenCV_LIBS} )