cmake_minimum_required(VERSION 3.13)
project(ecs_arch)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-m64")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-delayed-template-parsing")

set(SOURCE_FILES 
    src/main.c
    src/ecs/ecs.c
    src/ecs/world.c
    src/ecs/entity.c
    src/ecs/query.c
)

add_executable(ecs_arch ${SOURCE_FILES})
