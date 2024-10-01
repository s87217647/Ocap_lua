cmake_minimum_required(VERSION 3.23)
project(Ocap_lua C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(testes)

add_executable(Ocap_lua

        onelua.c bool_array.h)
