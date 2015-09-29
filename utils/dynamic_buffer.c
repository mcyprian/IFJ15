/**
 * @file dynamic_buffer.c
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Automaticly reallocated string buffer, including functions to initialize
 * buffer, add character or string, empty buffer and free block of memory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_buffer.h"

int init_buffer(TDynamic_buffer *b, size_t initial_length) {
    b->buffer = malloc(initial_length);
    if (b->buffer == NULL) {
        fprintf(stderr, "Failed to alloc initial memory for buffer\n");
        return 1;
    }
    b->length = initial_length;
    b->writing_index = 0;
    b->reading_index = 0;
    return 0;
}

int realloc_buffer(TDynamic_buffer *b, size_t size) {
    char *tmp;
    b->length *= 2;    // TODO: Use function to get new size?
    b->length += size;
    tmp = realloc(b->buffer, b->length);
    if (tmp == NULL) {
        fprintf(stderr, "Failed to realloc buffer memory\n");
        return 1;
    }
    b->buffer = tmp;
    return 0;
}

void free_buffer(TDynamic_buffer *b) {
    free(b->buffer);
}

int add_char(TDynamic_buffer *b, char c) {
    if (b->writing_index + 2 > b->length) 
        if (realloc_buffer(b, 2))
            return 1;
    b->buffer[b->writing_index] = c;
    b->writing_index++;
    b->buffer[b->writing_index] = '\0';
    return 0;
}

int add_str(TDynamic_buffer *b, char* word) {
    unsigned len = strlen(word);
    if (b->writing_index + len + 1 > b->length)
        if (realloc_buffer(b, len))
            return 1;
    strcpy(&b->buffer[b->writing_index], word);
    b->writing_index += len;
    return 0;
}

char *read_buffer(TDynamic_buffer *b) {
    return b->buffer;
}

char get_char(TDynamic_buffer *b) {
    if (b->reading_index == b->length)
        return -1;
    else 
        b->reading_index++;
    return b->buffer[b->reading_index - 1];
}

char *get_str(TDynamic_buffer *b, unsigned num) {
    if (b->reading_index + num > b->length)
        return NULL;
    else
        b->reading_index += num - 1;
    return (b->buffer + b->reading_index - num+ 1);
}

int empty_buffer(TDynamic_buffer *b, size_t initial_length) {
    char *tmp;
    tmp = realloc(b->buffer, initial_length);
    if (tmp == NULL) {
        fprintf(stderr, "Failed to realloc buffer memoty\n");
        return 1;
    }
    b->buffer = tmp;
    b->length = initial_length;
    b->writing_index = 0;
    b->reading_index = 0;
    return 0;
}
