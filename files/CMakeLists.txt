cmake_minimum_required(VERSION 2.8)
project(files)

include_directories(.)
#LINK_DIRECTORIES(/usr/local/lib)

add_executable(check_dir check_dir.c)
add_executable(file_io file_io.c)
add_executable(genfile genfile.c)
add_executable(rw_conf rw_conf.c)
add_executable(struct2file struct2file.c)

