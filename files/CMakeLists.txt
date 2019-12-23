cmake_minimum_required(VERSION 2.8)
project(files)

include_directories(.)
#LINK_DIRECTORIES(/usr/local/lib)

add_executable(struct2file struct2file.c)
add_executable(rw_conf rw_conf.c)
add_executable(check_dir check_dir.c)
#target_link_libraries(time_proc pthread)
#install(TARGETS  RUNTIME DESTINATION ./target)

