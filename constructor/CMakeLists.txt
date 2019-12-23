cmake_minimum_required(VERSION 2.8)
project(attribute)

include_directories(.)
#LINK_DIRECTORIES(/usr/local/lib)

add_executable(constructor constructor_func.c constructor_main.c)
#target_link_libraries(time_proc pthread)
#install(TARGETS  RUNTIME DESTINATION ./target)

