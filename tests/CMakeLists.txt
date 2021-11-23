cmake_minimum_required(VERSION 2.8.0)

#
# Options
#
option(BUILD_XPACK   "build xpack test"     OFF)
option(BUILD_TEMPL   "build template test"  ON)
option(BUILD_MYLOG   "build mylog test"     OFF)
option(BUILD_MGRAPH  "build mgraph test"    ON)

file(COPY ${PROJECT_SOURCE_DIR}/files/metro.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#
# Tests
#

if (BUILD_XPACK)
    add_executable(xpack xpack.cpp)
endif()

if (BUILD_TEMPL)
    add_executable(templ templ.cpp)
endif()

if (BUILD_MYLOG)
    add_executable(mylog mylog.cpp)
endif()

if (BUILD_MGRAPH)
    add_executable(tmgraph tmgraph.cpp)
endif()
