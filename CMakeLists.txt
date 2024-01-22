cmake_minimum_required(VERSION 3.10)
project(hello-ble VERSION 0.1)
add_executable(hello-ble src/hello-ble.cpp)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(simpleble REQUIRED CONFIG)
target_link_libraries(hello-ble simpleble::simpleble)

