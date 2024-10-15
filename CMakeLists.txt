cmake_minimum_required(VERSION 3.13)
project(pecs)

set(LIBRARY_VERSION "1.0")

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-m64")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-delayed-template-parsing")

file(GLOB SOURCES "src/*.c")

include_directories(include)

add_library(pecs SHARED ${SOURCES})

set_target_properties(pecs PROPERTIES VERSION ${LIBRARY_VERSION} SOVERSION 1)

set_target_properties(pecs PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if (WIN32) 
    target_compile_definitions(pecs PRIVATE EXPORT_SYMBOLS)
endif()
