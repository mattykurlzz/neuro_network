project (neuromodel)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_subdirectory(bmp)

add_executable(${PROJECT_NAME} main.cpp neuronet.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/bmp)

target_link_libraries(${PROJECT_NAME}
  bitmap_parse
)