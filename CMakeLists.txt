cmake_minimum_required(VERSION 3.22)

project(crack-tracer)

find_package(glfw3 REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} src/entry.cpp src/entry.cpp)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
    "$<$<CONFIG:DEBUG>:-g;-Wall;-Wextra>;-Wno-missing-field-initializers;-march=native"
)

target_compile_options(
  ${PROJECT_NAME}
  PUBLIC
    "$<$<CONFIG:RELEASE>:-Ofast;-fno-signed-zeros;-flto;-Wall;-Wextra>;-Wno-missing-field-initializers;-march=native"
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
