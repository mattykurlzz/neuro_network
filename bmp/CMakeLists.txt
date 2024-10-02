project(bitmap_parse)
cmake_minimum_required(VERSION 3.22)

add_library(${PROJECT_NAME} STATIC bitmap_parse.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})