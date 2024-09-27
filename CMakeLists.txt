cmake_minimum_required(VERSION 3.16)
project(snake)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}-Wall -Wextra -Werror -g")

add_subdirectory(src)
