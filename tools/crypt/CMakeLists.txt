cmake_minimum_required(VERSION 2.8)
project(crypt)

include_directories(.)
LINK_DIRECTORIES(/usr/local/lib)

add_executable(encrypt encrypt.c)
target_link_libraries(encrypt ssl crypto)

