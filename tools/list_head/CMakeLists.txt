cmake_minimum_required(VERSION 2.8)
project(list_head)

include_directories(.)
#include_directories(/usr/src/linux-headers-4.4.0-97/include/)
include_directories(/usr/local/include/)
#LINK_DIRECTORIES(/usr/local/lib)

add_executable(list_head_test list_head_test.c)

