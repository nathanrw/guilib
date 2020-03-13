cmake_minimum_required(VERSION 3.16.4)
project(guilib)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup()

add_executable(test main.cpp)
target_link_libraries(test ${CONAN_LIBS})
