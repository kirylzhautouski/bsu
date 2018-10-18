cmake_minimum_required(VERSION 3.10)
project(dns-lab1a)

include_directories(.)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_executable(dns-lab1a
        dns/dns.c
        dns/dns.h
        main.c hash_table/hash_table.h hash_table/hash_table.c)
