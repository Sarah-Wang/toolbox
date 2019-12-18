cmake_minimum_required(VERSION 2.8)
project(logs)

include_directories(.)
LINK_DIRECTORIES(/usr/local/lib)



add_executable(log_printf log_printf.c)
add_executable(log_complex log_complex.c)
add_executable(log_syslog log_syslog.c)
