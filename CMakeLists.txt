cmake_minimum_required(VERSION 2.8.0)

project("mmm" CXX)

#
# Options
#
option(BUILD_PROJ   "build project program"     ON)
option(BUILD_TEST   "build tests"               ON)

#
# Configurations
#
include("${PROJECT_SOURCE_DIR}/cmake/configuration.cmake")

#
# Modules
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

find_package(rapidjson REQUIRED)
include_directories(SYSTEM ${rapidjson_INCLUDE})

find_package(xpack REQUIRED)
include_directories(SYSTEM ${xpack_INCLUDE})

find_package(spdlog REQUIRED)
include_directories(SYSTEM ${spdlog_INCLUDE})

#
# Sources
#
include("${PROJECT_SOURCE_DIR}/cmake/manifest.cmake")

include_directories(src)

#
# Builds
#
if (BUILD_PROJ)
    add_subdirectory(proj)
endif()

if (BUILD_TEST)
    add_subdirectory(tests)
endif()
