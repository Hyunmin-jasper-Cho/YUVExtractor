cmake_minimum_required(VERSION 3.22)
project(YUV2PNG)

set(CMAKE_CXX_STANDARD 20)

find_package( OpenCV REQUIRED )
set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
