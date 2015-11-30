#include <instructions.h>



int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Resources *resources, TInstruction *instruction) = { 
        eq_int_mem_mem,
        eq_dbl_mem_mem,
        g_int_mem_mem,
        g_dbl_mem_mem,
        l_int_mem_mem,
        l_dbl_mem_mem,
        ge_int_mem_mem,
        ge_dbl_mem_mem,
        le_int_mem_mem,
        le_dbl_mem_mem,
        add_int_mem_mem,
        add_dbl_mem_mem,
        sub_int_mem_mem,
        sub_dbl_mem_mem,
        mul_int_mem_mem,
        mul_dbl_mem_mem,
        div_int_mem_mem,
        div_dbl_mem_mem,
        ne_int_mem_mem,
        ne_dbl_mem_mem,
        mov_int_mem,
        mov_int_const,
        mov_dbl_mem,
        mov_dbl_const,
        cast_int_mem,
        cast_dbl_mem,
        push_empty,
        push_int_const,
        push_dbl_const,
        push_index_const,
        push_int_mem,
        push_dbl_mem,
        push_index_mem,
        pop_empty,
        jmp_mem,
        jmp_const,
        jmp_true_mem_mem,
        jmp_true_const_const,
        jmp_true_const_mem,
        jmp_true_mem_const,
        function_call,
        function_return,
        halt
    };

