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
	catch2/2.11.1
    BUILD missing
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

function(read_source_list filename outvar)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${filename} 
                   ${CMAKE_CURRENT_BINARY_DIR}/${filename})
    file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/${filename} items)
    set(${outvar} "${items}" PARENT_SCOPE)
endfunction()

read_source_list(sources.txt sources)
read_source_list(headers.txt headers)
add_library(nui "${sources}" "${headers}")
target_include_directories(nui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

read_source_list(tests.txt tests)
add_executable(tests "${tests}")
target_link_libraries(tests nui CONAN_PKG::catch2)

read_source_list(demo-sources.txt demo_sources)
add_executable(demo "${demo_sources}")
target_link_libraries(demo CONAN_PKG::glfw CONAN_PKG::glew nui)
