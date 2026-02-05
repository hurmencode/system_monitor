cmake_minimum_required(VERSION 3.10)
project(system_monitor)

set(CMAKE_CXX_STANDARD 17)

add_executable(monitor
    src/main.cpp
    src/cpu.cpp
    src/ram.cpp
)

target_include_directories(monitor PRIVATE include)
