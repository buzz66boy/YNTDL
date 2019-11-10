# Author: Michael Albert

cmake_minimum_required(VERSION 3.3)

message(Building)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "output")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)

set(Tester
    tester/main.cpp
)

set(Printer
    printer/main.cpp
)

add_executable(test_yntdl ${Tester})
add_executable(print_yntdl ${Printer})
target_compile_options(test_yntdl PUBLIC -std=c++11 -pthread -g -Wfatal-errors -pipe)
target_compile_options(print_yntdl PUBLIC -std=c++11 -pthread -g -Wfatal-errors -pipe)
find_library(YNTDL_LIB yntdl)
target_link_libraries(test_yntdl ${YNTDL_LIB})
target_link_libraries(print_yntdl ${YNTDL_LIB})