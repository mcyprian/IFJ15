#include <stack.h>
#include <dynamic_structure_buffer.h>
#include <token.h>
#include <error_macros.h>

#define NUM 10

int main() {
    TDynamic_structure_buffer b;
    init_structure_buffer(&b, 5, sizeof(TToken));
    TStack stack = {.top = ZERO_INDEX, .length = 0};
    TToken *item;
    index_t index;
    int values[NUM];

    for (int i = 0; i < NUM; i++) {
        new_item(&b, index, item);
        item->token_type = i*10;
        push(&b, &stack, index);
    }
   
    get_types(&b, &stack, NUM, values);

    for (int i = 0; i < NUM; i++)
        printf("%d ", values[i]);
    putchar('\n');

    for (int i = 0; i < NUM; i++) {
        dereference_structure(&b, pop(&b, &stack), (void**)(&item));
        printf("Top of stack: %d\n", item->token_type);
        printf("Stack length: %lu\n", stack.length);
    }
    free_structure_buffer(&b);

    return 0;
}
