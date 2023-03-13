add_library (libmypthreads mypthreads.c mypthreads.h)

target_include_directories (libmypthreads PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})