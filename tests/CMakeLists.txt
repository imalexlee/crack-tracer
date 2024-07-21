cmake_minimum_required(VERSION 3.22)

project(crack-tracer-tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} entry.cpp)

target_compile_options(${PROJECT_NAME}
                       PRIVATE "$<$<CONFIG:DEBUG>:-g;-march=native>")

target_include_directories(${PROJECT_NAME} SYSTEM
                           PRIVATE ../thirdparty/stb_image_write)
