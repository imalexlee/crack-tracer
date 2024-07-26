cmake_minimum_required(VERSION 3.22)

project(crack-tracer)

find_package(glfw3 REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} src/entry.cpp src/render.cpp src/sphere.cpp)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
    "$<$<CONFIG:DEBUG>:-g;-Wall;-Wextra>;-Wno-missing-field-initializers;-march=native"
)

target_compile_options(
  ${PROJECT_NAME}
  PUBLIC
    "$<$<CONFIG:RELEASE>:-Ofast;-g;-fno-signed-zeros;-flto;-Wall;-Wextra>;-Wno-missing-field-initializers;-march=native"
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_include_directories(${PROJECT_NAME} SYSTEM
                           PRIVATE thirdparty/stb_image_write)
