cmake_minimum_required(VERSION 4.1)
project(Calendar C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Wextra)

add_executable(Calendar main.c
        write_event.c
        include/write_event.h
        include/struct.h
        err_handle.c
        include/err_handle.h
        event.c
        include/event.h
)
