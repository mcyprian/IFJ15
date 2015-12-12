/**
 * @file instrucrions.c
 * @author Michal Cyprian
 *
 * @section DESCRIPTION
 *
 * Definition and initialization of array of pointers to functions, which is
 * used to run instrucrions in interpreter
 *
 */

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
        ne_int_mem_mem,
        ne_dbl_mem_mem,
        add_int_mem_mem,
        add_dbl_mem_mem,
        sub_int_mem_mem,
        sub_dbl_mem_mem,
        mul_int_mem_mem,
        mul_dbl_mem_mem,
        div_int_mem_mem,
        div_dbl_mem_mem,
        eq_str_mem_mem,
        g_str_mem_mem,
        l_str_mem_mem,
        ge_str_mem_mem,
        le_str_mem_mem,
        ne_str_mem_mem,
        mov_top_mem,
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
        jmp_true_mem,
        jmp_false_mem,
        jmp_func,
    	function_call,
        function_return,
        cin_i,
        cin_d,
        cin_s,
        concat_mem_mem,
        substr_mem_mem,
        length_mem,
        find_mem_mem,
        sort_mem,
        cout_mem_type,
        no_return,
        set_type,
        halt
    };

