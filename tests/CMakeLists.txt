set(EXECUTABLE_OUTPUT_PATH .)
##eset(CMAKE_C_FLAGS "${ CMAKE_C_FLAGS } ${ pkg-config --cflags --libs check }")

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
add_executable(dynbuff_test dynbuff_test.c ../src/dynamic_buffer.c)
target_link_libraries(dynbuff_test check)
add_test(dynamic_buffer dynbuff_test)
