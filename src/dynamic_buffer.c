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
#include <limits.h>
#include <string.h>
#include <dynamic_buffer.h>
#include <error_macros.h>
#include <debug.h>

int init_buffer(TDynamic_buffer *b, size_t initial_length) {
    debug_print("%s\n", "BUFFER INIT");
    args_assert(b != NULL && initial_length > 0, INTERNAL_ERROR);
    b->buffer = calloc(initial_length, CHAR_BIT);
    catch_internal_error(b->buffer, NULL, "Failed to allocate memory for buffer."); 
    b->length = initial_length;
    b->writing_index = 1;
    b->reading_index = 1;
    b->buffer[b->writing_index] = '\0';
    return 0;
}

int realloc_buffer(TDynamic_buffer *b, size_t size) {
    args_assert(b != NULL, INTERNAL_ERROR);
    char *tmp = NULL;
    debug_print("%s: %lu\n", "REALLOCATED TO", b->length);
    tmp = realloc(b->buffer, b->length * 2 + size);	
	catch_internal_error(tmp, NULL, "Failed to realloc memory for buffer.");
	b->buffer = tmp;
    memset((char *)b->buffer + b->length, 0, b->length + size);
    b->length *= 2;
    b->length += size;
    return 0;
}

void free_buffer(TDynamic_buffer *b) {
    debug_print("%s\n", "FREE BUFFER");
    if(b != NULL)
		free(b->buffer);
}

int add_char(TDynamic_buffer *b, char c) {
    args_assert(b != NULL && c != '\0', INTERNAL_ERROR);
    if (b->writing_index + 1 > b->length) 
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

int add_str_index(TDynamic_buffer *b, index_t index) {
    args_assert(b != NULL && index != ZERO_INDEX, INTERNAL_ERROR);
    char *word = load_token(b, index);
    unsigned len = strlen(word);
    if (b->writing_index + len + 1 > b->length)
        catch_internal_error(realloc_buffer(b, len), INTERNAL_ERROR, "Failed to realloc buffer.");
    
    strcpy(&b->buffer[b->writing_index], load_token(b, index));
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

index_t save_token(TDynamic_buffer *b) {
    args_assert(b != NULL, INTERNAL_ERROR);
    static index_t start = 1;
    debug_print("%s %ld\n", "SAVE TOKEN INDEX:", start);
    b->writing_index++;
    index_t previous = start;
    start = b->writing_index;
    return previous;
}

char *load_token(TDynamic_buffer *b, index_t index) {
    args_assert(b != NULL, NULL);
    debug_print("%s %ld\n", "LOAD TOKEN INDEX:", index);
    return b->buffer + index;
}

int empty_buffer(TDynamic_buffer *b) {
    debug_print("%s\n", "EMPTY BUFFER");
    args_assert(b != NULL, INTERNAL_ERROR);
    b->writing_index = 1;
    b->reading_index = 1;
    b->buffer[b->writing_index] = '\0';
    return 0;
}
