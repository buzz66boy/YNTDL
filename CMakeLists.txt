# Author: Michael Albert

cmake_minimum_required(VERSION 3.3)
project(yntdl1 VERSION 0.1.0)

message(Building)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)

include_directories(
        /usr/src/linux-headers-generic/
        /usr/include
        src/yntdl/include
)

add_library(yntdl SHARED
        src/yntdl/core/errorCode.cpp
		src/yntdl/objects/ipaddr.cpp
        src/yntdl/objects/node.cpp
		src/yntdl/objects/link.cpp
        src/yntdl/objects/topology.cpp
        src/yntdl/objects/iface.cpp
        src/yntdl/objects/application.cpp
        src/yntdl/objects/position.cpp
        src/yntdl/objects/additionalTags.cpp
        src/yntdl/parse/parserTags.cpp
        src/yntdl/parse/topologyParser.cpp
        src/yntdl/parse/subTopologyParser.cpp
        src/yntdl/parse/settingsParser.cpp
        src/yntdl/parse/nodeParser.cpp
        src/yntdl/parse/linkParser.cpp
        src/yntdl/parse/ifaceParser.cpp
        src/yntdl/parse/positionParser.cpp
        src/yntdl/parse/applicationParser.cpp
        src/yntdl/parse/commandParser.cpp
)

target_compile_options(yntdl PUBLIC -std=c++11 -pthread -g -Wfatal-errors -pipe)

set_target_properties(yntdl PROPERTIES 
	VERSION ${PROJECT_VERSION}
	SOVERSION 1
	PUBLIC_HEADER src/yntdl/include/yntdl.h
)

find_library(YAML_LIB yaml-cpp)

target_link_libraries(yntdl ${YAML_LIB})

install(TARGETS yntdl
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
