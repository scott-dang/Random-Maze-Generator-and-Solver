cmake_minimum_required(VERSION 3.0.0)
project(OpenCV VERSION 0.1.0)

include(CTest)
enable_testing()

find_package( OpenCV REQUIRED PATH C:\\Users\\scott\\Desktop\\Temp\\MazeProject\\opencv2 )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(MazeProject MazeProject.cpp)

target_link_libraries( MazeProject ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})