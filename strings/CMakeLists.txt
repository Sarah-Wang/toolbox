cmake_minimum_required(VERSION 2.8)
project(strings)

include_directories(.)
LINK_DIRECTORIES(/usr/local/lib)

add_executable(4char2int 4char2int.c)
add_executable(string_array string_array.c)
add_executable(string_covert string_covert.c)
add_executable(strings_sorted strings_sorted.c)


