cmake_minimum_required(VERSION 3.10)
project(PROJET C)

set(CMAKE_C_STANDARD 99)

add_executable(PROJET main.c)

target_link_libraries (PROJET -lmingw32 -lalleg44)
