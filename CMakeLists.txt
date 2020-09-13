cmake_minimum_required(VERSION 3.16.0)
project(guilib)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(
        DOWNLOAD 
            "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake" 
            TLS_VERIFY ON
    )
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(
    REQUIRES 
        glfw/3.3.2
        glew/2.1.0
	swig/4.0.1
    BASIC_SETUP
    BUILD missing
)

file(STRINGS sources.txt sources)
file(STRINGS headers.txt headers)

add_executable(test "${sources}" "${headers}")
target_include_directories(test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(test ${CONAN_LIBS})
