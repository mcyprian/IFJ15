#include <stack.h>

int main() {
    TDynamic_buffer b;
    init_structure_buffer(&b, 5, sizeof(TItem));
    TToken *item;
    index_t index;
    index_t items[3];
    int values[5];

    for (int i = 0; i < 3; i++) {
        new_item(&b, index, item)
        item->token_type = i*10;
        push(&b, &stack, index);
        items[i] = index;
    }
    
    get_types(&b, &stack, 3, values);

    for (int i = 0; i < 3; i++)
        printf("%d ", values[i]);"
    putchar('\n');

    dereferance_structure(&b, pop(&b, &stack), (void**)(&item));
    printf("Top of stack: %d\n", item->token_type);
    printf("Stack length: %d\n", stack.length);

    dereferance_structure(&b, pop(&b, &stack), (void**)(&item));
    printf("Top of stack: %d\n", item->token_type);

    printf("Stack length: %d\n", stack.length);

    dereferance_structure(&b, pop(&b, &stack), (void**)(&item));
    printf("Top of stack: %d\n", item->token_type);
    printf("Stack length: %d\n", stack.length);

    return 0;
}
