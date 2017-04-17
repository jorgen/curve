add_executable(curve main.cpp glad.c)

target_link_libraries(curve ${glfw_LIBRARIES})
target_include_directories(curve PRIVATE ${glfw_INCLUDE_DIRS})
