cmake_minimum_required(VERSION 3.13)
project(ecs_sdl_example)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-m64")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(DEFINED ENV{SDL3_DIR})
    set(SDL3_DIR $ENV{SDL3_DIR})
else()
    message(FATAL_ERROR "SDL3_DIR is not set!")
endif()

include_directories(${SDL3_DIR}/include)
include_directories("../../include")

link_directories(${SDL3_DIR}/lib)
link_directories("../../build/lib")

set(SOURCE_FILES
    main.c
    app.c
    draw.c
    character.c
    time.c
    utils/math/vec2.c
)

add_executable(ecs_sdl_example ${SOURCE_FILES})


target_link_libraries(ecs_sdl_example libpecs SDL3)

add_custom_command(TARGET ecs_sdl_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ../../../build/bin/libpecs.dll $<TARGET_FILE_DIR:ecs_sdl_example>)

add_custom_command(TARGET ecs_sdl_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${SDL3_DIR}/bin/SDL3.dll $<TARGET_FILE_DIR:ecs_sdl_example>)

