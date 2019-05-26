# Author: Michael Albert

cmake_minimum_required(VERSION 3.3)
project(yntdl VERSION 0.1.0)

message(Building Executables.)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include(GNUInstallDirs)

include_directories(
        /usr/src/linux-headers-generic/
        /usr/include
        src/lib/yntdl/include
        src/lib/yntdl/objects/include
        src/lib/yntdl/parse/include
)

add_library(yntdl SHARED
		src/lib/yntdl/objects/include/ipaddr.h
		src/lib/yntdl/objects/include/node.h
		src/lib/yntdl/objects/include/iface.h
		src/lib/yntdl/objects/include/position.h
		src/lib/yntdl/objects/include/link.h
		src/lib/yntdl/objects/include/topology.h
		src/lib/yntdl/objects/include/application.h
        src/lib/yntdl/objects/include/nameable.h
        src/lib/yntdl/objects/include/additionalTags.h
		src/lib/yntdl/objects/ipaddr.cpp
        src/lib/yntdl/objects/node.cpp
		src/lib/yntdl/objects/link.cpp
        src/lib/yntdl/objects/topology.cpp
        src/lib/yntdl/objects/iface.cpp
        src/lib/yntdl/objects/application.cpp
        src/lib/yntdl/objects/position.cpp
        src/lib/yntdl/objects/additionalTags.cpp
        src/lib/yntdl/parse/include/parserTags.h
        src/lib/yntdl/parse/include/settingsParser.h
        src/lib/yntdl/parse/include/topologyParser.h
        src/lib/yntdl/parse/include/subTopologyParser.h
        src/lib/yntdl/parse/include/nodeParser.h
        src/lib/yntdl/parse/include/linkParser.h
        src/lib/yntdl/parse/include/ifaceParser.h
        src/lib/yntdl/parse/include/positionParser.h
        src/lib/yntdl/parse/include/applicationParser.h
        src/lib/yntdl/parse/include/commandParser.h
        src/lib/yntdl/parse/parserTags.cpp
        src/lib/yntdl/parse/topologyParser.cpp
        src/lib/yntdl/parse/subTopologyParser.cpp
        src/lib/yntdl/parse/settingsParser.cpp
        src/lib/yntdl/parse/nodeParser.cpp
        src/lib/yntdl/parse/linkParser.cpp
        src/lib/yntdl/parse/ifaceParser.cpp
        src/lib/yntdl/parse/positionParser.cpp
        src/lib/yntdl/parse/applicationParser.cpp
        src/lib/yntdl/parse/commandParser.cpp
)

set_target_properties(yntdl PROPERTIES 
	VERSION ${PROJECT_VERSION}
	SOVERSION 1
	PUBLIC_HEADER include/yntdl.h)

find_library(YAML_LIB yaml-cpp)

target_link_libraries(yntdl ${YAML_LIB})

target_compile_options(yntdl PUBLIC -std=c++11 -pthread -g -Wfatal-errors -pipe)

install(TARGETS yntdl
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
