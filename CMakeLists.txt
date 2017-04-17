cmake_minimum_required (VERSION 3.1)
project (Curve)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}../glfw_install/lib/cmake")

find_package(glfw)
add_subdirectory (src)
