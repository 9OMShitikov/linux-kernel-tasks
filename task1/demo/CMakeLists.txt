cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
find_package(GTest CONFIG REQUIRED)

enable_testing()
include(GoogleTest)

add_executable (mypthreadsdemo main.cc)
target_link_libraries (mypthreadsdemo LINK_PUBLIC
                       GTest::gtest_main
                       libmypthreads)

gtest_discover_tests(mypthreadsdemo)
