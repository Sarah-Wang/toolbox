cmake_minimum_required(VERSION 2.8)
project(time)

include_directories(.)
LINK_DIRECTORIES(/usr/local/lib)

add_executable(time_proc time_proc.c)

