cmake_minimum_required(VERSION 3.13)

project(02_printf)

include(pico_sdk_import.cmake)

pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.c
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
)

pico_add_extra_outputs(${PROJECT_NAME})