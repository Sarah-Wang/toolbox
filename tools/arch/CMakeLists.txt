cmake_minimum_required(VERSION 2.8)
project(arch)

include_directories(.)
LINK_DIRECTORIES(/usr/local/lib)

add_executable(hw_info hw_info.c)

