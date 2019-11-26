cmake_minimum_required(VERSION 3.2)

project(bulk)

add_executable(bulk bulk.cpp observer.cpp command_handler.cpp)


set_target_properties(bulk PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)