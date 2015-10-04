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
    unsigned writing_index;
    unsigned reading_index;
    char *buffer;
} TDynamic_buffer;

/** Allocs memory for buffer, sets initial index and length.
 * @param  b pointer to TDynamic_buffer
 * @param  initial_length initial length of buffer
 * @return Zero on success, INTERNAL_ERROR on malloc error.
 */
int init_buffer(TDynamic_buffer *b, size_t initial_length);

/** Realllocs buffer memory to new size.
 * @param b pointer to TDynamic_buffer
 * @param size new size of memory block
 * @return Zero on success, INTERNAL_ERROR on realloc error
 */
int realloc_buffer(TDynamic_buffer *b, size_t size);

/** Free memory block, pointed to by b
 * @param b pointer to TDynamic_buffer
 */
void free_buffer(TDynamic_buffer *b);

/** Adds char to index of buffer
 * @param b pointer to TDynamic_buffer
 * @param c character to be add to current buffer index
 * @return Zero on success, INTERNAL_ERROR on malloc error
 */
int add_char(TDynamic_buffer *b, char c);

/** Adds string to buffer
 * @param b pointer ro TDynamic_buffer
 * @param word pointer to string to be add to current buffer index
 * @return Zero on success, INTERNAL_ERROR on malloc error
 */
int add_str(TDynamic_buffer *b, char* word);

/** Reads current content of buffer
 * @oaram b pointer to TDynamic_buffer
 * @return Pointer to string stored in buffer, INTERNAL_ERROR on error
 */ 
char *read_buffer(TDynamic_buffer *b);

/** Reads one character from current readnig index in buffer
 * @param b pointer to TDynamic_buffer
 * @return char from current reading index, or NULL on error.
 */
char get_char(TDynamic_buffer *b);

/**Reads string of length num from buffer
 * @oaram b pointer to TDyanmic buffer
 * @param num number of characters to be read
 * @return pointer to beggining of str or NULL on error
 */
char *get_str(TDynamic_buffer *b, unsigned num);

/** Reinitialeze buffer to start writing to the begginig.
 * @param b pointer to TDynamic buffer
 * @return Zero on success, INTERNAL_ERROR on error
 */
int empty_buffer(TDynamic_buffer *b);
