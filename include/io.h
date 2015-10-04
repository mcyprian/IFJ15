#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <error_macros.h>
#include <dynamic_buffer.h>


int read_from_file(FILE * f, TDynamic_buffer * buff);

#endif
