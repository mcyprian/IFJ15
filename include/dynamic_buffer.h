/**
 * @file dynamic_buffer.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for dynamic_buffer.c module
 */

typedef struct {
    unsigned length;
    unsigned index;
    char *buffer;
} TDynamic_buffer;

int init_buffer(TDynamic_buffer *b, size_t initial_length);
int realloc_buffer(TDynamic_buffer *b, size_t size);
void free_buffer(TDynamic_buffer *b);
int add_char(TDynamic_buffer *b, char c);
int add_str(TDynamic_buffer *b, char* word);
char *read_buffer(TDynamic_buffer *b);
int empty_buffer(TDynamic_buffer *b, size_t initial_length);
