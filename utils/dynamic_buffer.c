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
    if (b == NULL || initial_length <= 0)
        return 1;
    b->buffer = malloc(initial_length);
    if (b->buffer == NULL) {
        fprintf(stderr, "Failed to alloc initial memory for buffer\n");
        return 1;
    }
    b->length = initial_length;
    b->writing_index = 0;
    b->reading_index = 0;
    b->buffer[b->writing_index] = '\0';
    return 0;
}

int realloc_buffer(TDynamic_buffer *b, size_t size) {
    if (b == NULL)
        return 1;
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
    if (b == NULL || c == '\0')
        return 1;
    if (b->writing_index + 2 > b->length) 
        if (realloc_buffer(b, 2))
            return 1;
    b->buffer[b->writing_index] = c;
    b->writing_index++;
    b->buffer[b->writing_index] = '\0';
    return 0;
}

int add_str(TDynamic_buffer *b, char* word) {
    if (b == NULL || word == NULL)
        return 1;
    unsigned len = strlen(word);
    if (b->writing_index + len + 1 > b->length)
        if (realloc_buffer(b, len))
            return 1;
    strcpy(&b->buffer[b->writing_index], word);
    b->writing_index += len;
    return 0;
}

char *read_buffer(TDynamic_buffer *b) {
    if (b == NULL)
        return NULL;
    return b->buffer;
}

char get_char(TDynamic_buffer *b) {
    if (b == NULL)
        return 1;
    if (b->reading_index == b->length)
        return -1;
    else 
        b->reading_index++;
    return b->buffer[b->reading_index - 1];
}

char *get_str(TDynamic_buffer *b, unsigned num) {
    if (b == NULL || num <= 0)
        return NULL;
    if (b->reading_index + num > b->length)
        return NULL;
    else
        b->reading_index += num;
    return (b->buffer + b->reading_index - num);
}

int empty_buffer(TDynamic_buffer *b) {
    if (b == NULL)
        return 1;
    b->writing_index = 0;
    b->buffer[b->writing_index] = '\0';
    return 0;
}
