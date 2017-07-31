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

target_sources(curve PUBLIC 3rdparty/glad/src/glad.c)
source_group("glade" FILES 3rdparty/glad.c)

file(GLOB libtess_headers 3rdparty/libtess/*.h)
set(libtess_c
    3rdparty/libtess/dict.c
    3rdparty/libtess/geom.c
    3rdparty/libtess/memalloc.c
    3rdparty/libtess/mesh.c
    3rdparty/libtess/normal.c
    3rdparty/libtess/priorityq.c
    3rdparty/libtess/render.c
    3rdparty/libtess/sweep.c
    3rdparty/libtess/tess.c
    3rdparty/libtess/tessmono.c)

target_sources(curve PUBLIC ${libtess_headers}
                            ${libtess_c})
source_group("libtess" FILES ${libtess_headers})
source_group("libtess" FILES ${libtess_c})

target_link_libraries(curve PRIVATE glfw)
target_include_directories(curve PRIVATE 3rdparty
                                         3rdparty/glad/include)
