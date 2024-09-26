project (neuromodel)
cmake_minimum_required(VERSION 3.22)
add_executable(${PROJECT_NAME} main.cpp neuronet.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})