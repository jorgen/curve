set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(3rdparty/glfw)

add_executable(curve main.cpp
                     renderer.h
                     renderer.cpp
                     memberlinkedlist.h
                     solidtriangleprogram.h
                     solidtriangleprogram.cpp
                     solidtrianglerenderer.h
                     solidtrianglerenderer.cpp
                     vao.h
                     testitem.h
                     testitem.cpp
                     program.h
                     callback.h)

                 target_sources(curve PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad/src/glad.c)
source_group("glade" FILES 3rdparty/glad.c)

file(GLOB libtess_headers 3rdparty/libtess/*.h)
set(${CMAKE_CURRENT_SOURCE_DIR}/libtess_c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/dict.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/geom.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/memalloc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/mesh.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/normal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/priorityq.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/render.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/sweep.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/tess.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libtess/tessmono.c)

target_sources(curve PUBLIC ${libtess_headers}
                            ${libtess_c})
source_group("libtess" FILES ${libtess_headers})
source_group("libtess" FILES ${libtess_c})

target_link_libraries(curve PRIVATE glfw)
target_include_directories(curve PRIVATE 3rdparty
                                         3rdparty/glad/include)
