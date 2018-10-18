cmake_minimum_required(VERSION 3.10)
project(fw_lab1b)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_executable(fw_lab1b main.c splpv1.c splpv1.h)