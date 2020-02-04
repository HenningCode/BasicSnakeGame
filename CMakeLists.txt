cmake_minimum_required(VERSION 3.15)
project(Snake)

include_directories(include)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Snake
        src/main.c)

target_link_libraries(Snake ${CURSES_LIBRARIES})