#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_buffer.h"

/** Allocs memory for buffer, sets initial index and length.
 * @param  b pointer to TDynamic_buffer
 * @param  initial_length initial length of buffer
 * @return Zero on success, one on malloc error.
 */
int init_buffer(TDynamic_buffer *b, unsigned initial_length) {
    b->buffer = malloc(initial_length);
    if (b->buffer == NULL) {
        fprintf(stderr, "Failed to alloc initial memory for buffer\n");
        return 1;
    }
    b->length = initial_length;
    b->index = 0;
    return 0;
}

/** Realllocs buffer memory to new size.
 * @param b pointer to TDynamic_buffer
 * @param size new size of memory block
 * @return Zero on success, one on realloc error
 */
int realloc_buffer(TDynamic_buffer *b, unsigned size) {
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

/** Free memory block, pointed to by b
 * @param b Pointer to TDynamic_buffer
 */
void free_buffer(TDynamic_buffer *b) {
    free(b->buffer);
}

/** Adds char to index of buffer
 * @param b pointer to TDynamic_buffer
 * @param c character to be add to current buffer index
 * @return Zero on success, one on malloc error
 */
int add_char(TDynamic_buffer *b, char c) {
    if (b->index + 2 > b->length) 
        if (realloc_buffer(b, 2))
            return 1;
    b->buffer[b->index] = c;
    b->index++;
    b->buffer[b->index] = '\0';
    return 0;
}

/** Adds string to buffer
 * @param b pointer ro TDynamic_buffer
 * @param word pointer to string to be add to current buffer index
 * @return Zero on success, one on malloc error
 */
int add_str(TDynamic_buffer *b, char* word) {
    unsigned len = strlen(word);
    if (b->index + len + 1 > b->length)
        if (realloc_buffer(b, len))
            return 1;
    strcpy(&b->buffer[b->index], word);
    b->index += len;
    return 0;
}

/** Reads current content of buffer
 * @oaram b pointer to TDynamic_buffer
 * @return Pointer to string stored in buffer
 */ 
char *read_buffer(TDynamic_buffer *b) {
    return b->buffer;
}

/** Reinitialeze buffer to start writing to the begginig.
 * @param b pointer to TDynamic buffer
 * @param  initial_length new inital length of buffer
 * @return Zero on success, one on malloc error
 */
int empty_buffer(TDynamic_buffer *b, unsigned initial_length) {
    char *tmp;
    tmp = realloc(b->buffer, initial_length);
    if (tmp == NULL) {
        fprintf(stderr, "Failed to realloc buffer memoty\n");
        return 1;
    }
    b->buffer = tmp;
    b->length = initial_length;
    b->index = 0;
    return 0;
}
