add_library (libmypthreads mypthreads.cc mypthreads.h)

target_include_directories (libmypthreads PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(libmypthreads PROPERTIES LINKER_LANGUAGE C)
