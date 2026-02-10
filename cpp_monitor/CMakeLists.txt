cmake_minimum_required(VERSION 3.10)
project(system_monitor)

set(CMAKE_CXX_STANDARD 17)

add_executable(monitor
    src/cpu.cpp
    src/json_output.cpp
    src/main.cpp
    src/ram.cpp
    src/uptime.cpp
)

target_include_directories(monitor PRIVATE include)
