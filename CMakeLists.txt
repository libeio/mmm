cmake_minimum_required(VERSION 2.8.0)

project("mmm", CXX)

#
# Configuration
#
include("${PROJECT_SOURCE_DIR}/cmake/configuration.cmake")

#
# Modules
#

#
# Source Files 
#
include("${PROJECT_SOURCE_DIR}/cmake/manifest.cmake")

include_directories(src)

