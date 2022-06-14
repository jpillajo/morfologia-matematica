cmake_minimum_required(VERSION 3.5)

project(Proyecto LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(OpenMP REQUIRED)
add_executable(Proyecto main.cpp)
target_link_libraries( Proyecto ${OpenCV_LIBS} OpenMP::OpenMP_CXX)


