cmake_minimum_required(VERSION 4.1)
project(Calendar C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Wextra)

add_executable(Calendar main.c
        write_event.c
        write_event.h
        struct.h
        err_handle.c
        err_handle.h
        event.c
        event.h
)
