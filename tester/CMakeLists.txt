# Author: Michael Albert

cmake_minimum_required(VERSION 3.3)

message(Building)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "output")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "output/libyntdl")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)

set(Tester
    main.cpp
)

add_executable(yntdl_tester ${Tester})
target_compile_options(yntdl_tester PUBLIC -std=c++11 -pthread -g -Wfatal-errors -pipe)
find_library(YNTDL_LIB yntdl)
target_link_libraries(yntdl_tester ${YNTDL_LIB})