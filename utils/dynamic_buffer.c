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
#include <dynamic_buffer.h>
#include <error_macros.h>
#include <debug.h>

int init_buffer(TDynamic_buffer *b, size_t initial_length) {
    args_assert(b != NULL && initial_length > 0, INTERNAL_ERROR);
    b->buffer = malloc(initial_length);
    catch_internal_error(b->buffer, NULL, "Failed to allocate memory for buffer."); 
    b->length = initial_length;
    b->writing_index = 0;
    b->reading_index = 0;
    b->buffer[b->writing_index] = '\0';
    return 0;
}

int realloc_buffer(TDynamic_buffer *b, size_t size) {
    args_assert(b != NULL, INTERNAL_ERROR);
    char *tmp = NULL;
    b->length *= 2;
    b->length += size;
    tmp = realloc(b->buffer, b->length);	
	catch_internal_error(tmp, NULL, "Failed to realloc memory for buffer.");
	b->buffer = tmp;
    return 0;
}

void free_buffer(TDynamic_buffer *b) {
    if(b != NULL)
		free(b->buffer);
}

int add_char(TDynamic_buffer *b, char c) {
    args_assert(b != NULL && c != '\0', INTERNAL_ERROR);
    if (b->writing_index + 2 > b->length) 
        catch_internal_error(realloc_buffer(b, 2), INTERNAL_ERROR, "Failed to realloc buffer.");
    b->buffer[b->writing_index] = c;
    b->writing_index++;
    b->buffer[b->writing_index] = '\0';
    return 0;
}

int add_str(TDynamic_buffer *b, char* word) {
    args_assert(b != NULL && word != NULL, INTERNAL_ERROR);
    unsigned len = strlen(word);
    if (b->writing_index + len + 1 > b->length)
        catch_internal_error(realloc_buffer(b, len), INTERNAL_ERROR, "Failed to realloc buffer.");
    strcpy(&b->buffer[b->writing_index], word);
    b->writing_index += len;
    return 0;
}

char *read_buffer(TDynamic_buffer *b) {
    args_assert(b != NULL, NULL);
    return b->buffer;
}

char get_char(TDynamic_buffer *b) {
    args_assert(b != NULL, EOF);
    if (b->reading_index == b->length)
        return EOF;
    b->reading_index++;
    return b->buffer[b->reading_index - 1];
}

char *get_str(TDynamic_buffer *b, unsigned num) {
    args_assert(b != NULL && num > 0, NULL);
    if (b->reading_index + num > b->length)
        return NULL;
    else
        b->reading_index += num;
    return (b->buffer + b->reading_index - num);
}

char *save_token(TDynamic_buffer *b) {
    args_assert(b != NULL, NULL);
    static char *start = NULL;
    b->writing_index++;
    char* previous = start;
    start = &b->buffer[b->writing_index];
    if (previous == NULL) 
        return b->buffer;
    else 
        return previous;
}

int empty_buffer(TDynamic_buffer *b) {
    args_assert(b != NULL, INTERNAL_ERROR);
    b->writing_index = 0;
    b->reading_index = 0;
    b->buffer[b->writing_index] = '\0';
    return 0;
}
