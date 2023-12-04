cmake_minimum_required(VERSION 3.26)
project(rlib C)

set(CMAKE_C_STANDARD 17)

SET(string_src rlib/string.h rlib/string.c)
SET(file_src rlib/file.h rlib/file.c)

SET(rlib_src ${string_src} ${file_src})

add_library(rlib STATIC ${rlib_src})