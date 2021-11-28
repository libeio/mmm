cmake_minimum_required(VERSION 2.8.0)

#
# Options
#
option(BUILD_XPACK          "build xpack test"              OFF)
option(BUILD_TEMPL          "build template test"           OFF)
option(BUILD_MYLOG          "build mylog test"              OFF)
option(BUILD_MG_BASE        "build mgraph base test"        OFF)
option(BUILD_MG_TRAVERSE    "build mgraph bfs and dfs test" OFF)
option(BUILD_MG_PRIM        "build mgraph prim test"        ON)
option(BUILD_MG_KRUSKAL     "build mgraph kruskal test"     ON)
option(BUILD_MG_SHORTEST    "build mgraph shortest test"    ON)

set(CMAKE_BUILD_TYPE Debug)

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

if (BUILD_MG_BASE)
    add_executable(mg_base mg_base.cpp)
endif()

if (BUILD_MG_TRAVERSE)
    add_executable(mg_traverse mg_traverse.cpp)
endif()

if (BUILD_MG_PRIM)
    add_executable(mg_prim mg_prim.cpp)
endif()

if (BUILD_MG_KRUSKAL)
    add_executable(mg_kruskal mg_kruskal.cpp)
endif()

if (BUILD_MG_SHORTEST)
    add_executable(mg_shortest mg_shortest.cpp)
endif()
