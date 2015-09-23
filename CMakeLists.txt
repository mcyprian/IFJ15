cmake_minimum_required(VERSION 2.6)
project(Interpreter)

include_directories($(CMAKE_PROJECT_DIR)/include)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall -Wextra -pedantic -O2")
set(CMAKE_SOURCE_DIR $(CMAKE_PROJECT_DIR)/src )

add_subdirectory(src)
