cmake_minimum_required(VERSION 3.12)

project(
	hello-ble
	VERSION 0.1
)
add_executable(hello-ble
	${CMAKE_CURRENT_SOURCE_DIR}/src/hello-ble.cpp)

set_target_properties(hello-ble PROPERTIES
	CMAKE_CXX_STANDARD 17
	CMAKE_CXX_STANDARD_REQUIRED YES
	VERSION 0.1
)
target_compile_features(hello-ble PUBLIC cxx_std_17)
find_package(simpleble REQUIRED CONFIG)
target_link_libraries(hello-ble simpleble::simpleble)

