cmake_minimum_required(VERSION 2.8.0)

file(COPY ${PROJECT_SOURCE_DIR}/files/metro.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(parse_json parse_json.cpp)
